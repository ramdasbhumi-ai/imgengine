import argparse
import json
import mimetypes
import os
import statistics
import threading
import time
from concurrent.futures import ThreadPoolExecutor, as_completed
from dataclasses import asdict, dataclass
from pathlib import Path
from typing import Optional

import requests


@dataclass
class JobResult:
    index: int
    accepted: bool
    final_status: str
    submit_status_code: int
    submit_seconds: float
    end_to_end_seconds: float
    queue_seconds: Optional[float]
    processing_seconds: Optional[float]
    poll_count: int
    error: str
    job_id: str


class Stats:
    @staticmethod
    def percentile(values: list[float], pct: float) -> float:
        if not values:
            return 0.0

        ordered = sorted(values)
        rank = pct * (len(ordered) - 1)
        idx = int(rank + 0.5)
        if idx >= len(ordered):
            idx = len(ordered) - 1
        return ordered[idx]

    @staticmethod
    def print_block(label: str, values: list[float]) -> None:
        if not values:
            print(f"{label}: no data")
            return

        avg = statistics.fmean(values)
        p50 = Stats.percentile(values, 0.50)
        p95 = Stats.percentile(values, 0.95)
        p99 = Stats.percentile(values, 0.99)
        print(label)
        print(f"  avg: {avg:.4f}s")
        print(f"  p50: {p50:.4f}s")
        print(f"  p95: {p95:.4f}s")
        print(f"  p99: {p99:.4f}s")
        print(f"  min: {min(values):.4f}s")
        print(f"  max: {max(values):.4f}s")


class ServiceBenchmark:
    def __init__(
        self,
        base_url: str,
        api_key: str,
        file_path: Path,
        total_requests: int,
        concurrency: int,
        poll_interval: float,
        request_timeout: float,
        job_timeout: float,
        output_json: Optional[Path],
    ) -> None:
        self.base_url = base_url.rstrip("/")
        self.api_key = api_key
        self.file_path = file_path
        self.total_requests = total_requests
        self.concurrency = concurrency
        self.poll_interval = poll_interval
        self.request_timeout = request_timeout
        self.job_timeout = job_timeout
        self.output_json = output_json
        self._local = threading.local()

    def _session(self) -> requests.Session:
        session = getattr(self._local, "session", None)
        if session is None:
            session = requests.Session()
            session.headers.update({"accept": "application/json", "x-api-key": self.api_key})
            self._local.session = session
        return session

    def _submit_job(self, index: int) -> JobResult:
        submit_started = time.perf_counter()
        session = self._session()
        content_type = mimetypes.guess_type(self.file_path.name)[0] or "application/octet-stream"

        with self.file_path.open("rb") as handle:
            response = session.post(
                f"{self.base_url}/api/generate",
                files={"file": (self.file_path.name, handle, content_type)},
                timeout=self.request_timeout,
            )

        submit_finished = time.perf_counter()
        submit_seconds = submit_finished - submit_started

        if response.status_code != 200:
            return JobResult(
                index=index,
                accepted=False,
                final_status="submit_failed",
                submit_status_code=response.status_code,
                submit_seconds=submit_seconds,
                end_to_end_seconds=submit_seconds,
                queue_seconds=None,
                processing_seconds=None,
                poll_count=0,
                error=response.text.strip(),
                job_id="",
            )

        payload = response.json()
        job_id = payload.get("job_id", "")
        if not job_id:
            return JobResult(
                index=index,
                accepted=False,
                final_status="invalid_response",
                submit_status_code=response.status_code,
                submit_seconds=submit_seconds,
                end_to_end_seconds=submit_seconds,
                queue_seconds=None,
                processing_seconds=None,
                poll_count=0,
                error=f"missing job_id in response: {payload}",
                job_id="",
            )

        status_url = f"{self.base_url}/api/status/{job_id}"
        deadline = submit_finished + self.job_timeout
        poll_count = 0
        queued_seen_at = submit_finished
        processing_seen_at = None
        last_status = payload.get("status", "queued")

        while True:
            if time.perf_counter() > deadline:
                end_to_end = time.perf_counter() - submit_started
                return JobResult(
                    index=index,
                    accepted=True,
                    final_status="timeout",
                    submit_status_code=response.status_code,
                    submit_seconds=submit_seconds,
                    end_to_end_seconds=end_to_end,
                    queue_seconds=(processing_seen_at - queued_seen_at) if processing_seen_at else None,
                    processing_seconds=None,
                    poll_count=poll_count,
                    error=f"job timed out after {self.job_timeout:.1f}s",
                    job_id=job_id,
                )

            poll_response = session.get(status_url, timeout=self.request_timeout)
            poll_count += 1

            if poll_response.status_code != 200:
                end_to_end = time.perf_counter() - submit_started
                return JobResult(
                    index=index,
                    accepted=True,
                    final_status="poll_failed",
                    submit_status_code=response.status_code,
                    submit_seconds=submit_seconds,
                    end_to_end_seconds=end_to_end,
                    queue_seconds=(processing_seen_at - queued_seen_at) if processing_seen_at else None,
                    processing_seconds=None,
                    poll_count=poll_count,
                    error=f"status poll failed: {poll_response.status_code} {poll_response.text.strip()}",
                    job_id=job_id,
                )

            status_payload = poll_response.json()
            last_status = status_payload.get("status", "unknown")
            now = time.perf_counter()

            if last_status == "processing" and processing_seen_at is None:
                processing_seen_at = now

            if last_status in {"completed", "failed"}:
                queue_seconds = None
                processing_seconds = None
                if processing_seen_at is not None:
                    queue_seconds = processing_seen_at - queued_seen_at
                    processing_seconds = now - processing_seen_at

                return JobResult(
                    index=index,
                    accepted=True,
                    final_status=last_status,
                    submit_status_code=response.status_code,
                    submit_seconds=submit_seconds,
                    end_to_end_seconds=now - submit_started,
                    queue_seconds=queue_seconds,
                    processing_seconds=processing_seconds,
                    poll_count=poll_count,
                    error="",
                    job_id=job_id,
                )

            time.sleep(self.poll_interval)

    def run(self) -> int:
        started = time.perf_counter()
        results: list[JobResult] = []

        print(f"Running service benchmark against {self.base_url}")
        print(f"file:        {self.file_path}")
        print(f"requests:    {self.total_requests}")
        print(f"concurrency: {self.concurrency}")
        print(f"poll:        {self.poll_interval:.2f}s")
        print(f"timeout:     {self.job_timeout:.1f}s per job")
        print("")

        with ThreadPoolExecutor(max_workers=self.concurrency) as executor:
            futures = {
                executor.submit(self._submit_job, index): index
                for index in range(1, self.total_requests + 1)
            }

            for future in as_completed(futures):
                result = future.result()
                results.append(result)

                if result.final_status == "completed":
                    print(
                        f"[OK] {result.index} submit={result.submit_seconds:.4f}s "
                        f"e2e={result.end_to_end_seconds:.4f}s polls={result.poll_count}"
                    )
                else:
                    print(
                        f"[ERR] {result.index} status={result.final_status} "
                        f"http={result.submit_status_code} error={result.error}"
                    )

        total_seconds = time.perf_counter() - started
        self._report(results, total_seconds)

        if self.output_json:
            self.output_json.parent.mkdir(parents=True, exist_ok=True)
            self.output_json.write_text(
                json.dumps(
                    {
                        "generated_at_epoch": time.time(),
                        "base_url": self.base_url,
                        "file_path": str(self.file_path),
                        "total_requests": self.total_requests,
                        "concurrency": self.concurrency,
                        "poll_interval": self.poll_interval,
                        "request_timeout": self.request_timeout,
                        "job_timeout": self.job_timeout,
                        "total_wall_seconds": total_seconds,
                        "results": [asdict(result) for result in sorted(results, key=lambda item: item.index)],
                    },
                    indent=2,
                )
            )
            print(f"\nWrote JSON report to {self.output_json}")

        return 0 if all(result.final_status == "completed" for result in results) else 1

    def _report(self, results: list[JobResult], total_seconds: float) -> None:
        completed = [result for result in results if result.final_status == "completed"]
        failed = [result for result in results if result.final_status != "completed"]
        accepted = [result for result in results if result.accepted]
        submit_times = [result.submit_seconds for result in results]
        end_to_end_times = [result.end_to_end_seconds for result in completed]
        queue_times = [result.queue_seconds for result in completed if result.queue_seconds is not None]
        processing_times = [
            result.processing_seconds
            for result in completed
            if result.processing_seconds is not None
        ]
        avg_polls = statistics.fmean([result.poll_count for result in accepted]) if accepted else 0.0
        throughput = len(completed) / total_seconds if total_seconds > 0 else 0.0

        print("\n==================================================")
        print("SERVICE BENCHMARK RESULT")
        print("==================================================")
        print(f"Total Requests : {len(results)}")
        print(f"Accepted       : {len(accepted)}")
        print(f"Completed      : {len(completed)}")
        print(f"Failed         : {len(failed)}")
        print(f"Wall Time      : {total_seconds:.4f}s")
        print(f"Throughput     : {throughput:.2f} completed jobs/sec")
        print(f"Avg Polls      : {avg_polls:.2f}")
        print("==================================================")
        Stats.print_block("Submission latency", submit_times)
        Stats.print_block("End-to-end latency", end_to_end_times)
        if queue_times:
            Stats.print_block("Queued-to-processing latency", queue_times)
        if processing_times:
            Stats.print_block("Processing-to-complete latency", processing_times)

        if failed:
            print("\nFailures:")
            for result in failed[:10]:
                print(
                    f"  #{result.index} status={result.final_status} "
                    f"http={result.submit_status_code} error={result.error}"
                )
            if len(failed) > 10:
                print(f"  ... and {len(failed) - 10} more")


def parse_args() -> argparse.Namespace:
    default_concurrency = os.cpu_count() or 1
    parser = argparse.ArgumentParser(
        description="Benchmark the imgengine SaaS path from HTTP submit to completed job."
    )
    parser.add_argument(
        "--base-url",
        default="http://127.0.0.1:8000",
        help="API base URL",
    )
    parser.add_argument(
        "--api-key",
        default="test-key-123",
        help="API key for /api/generate",
    )
    parser.add_argument(
        "--file",
        default=str(Path(__file__).resolve().parent / "test.jpg"),
        help="Input file to upload",
    )
    parser.add_argument(
        "--requests",
        type=int,
        default=20,
        help="Number of jobs to submit",
    )
    parser.add_argument(
        "--concurrency",
        type=int,
        default=default_concurrency,
        help=f"Concurrent request workers (default: {default_concurrency})",
    )
    parser.add_argument(
        "--poll-interval",
        type=float,
        default=0.25,
        help="Seconds between status polls",
    )
    parser.add_argument(
        "--request-timeout",
        type=float,
        default=30.0,
        help="Per-request timeout in seconds",
    )
    parser.add_argument(
        "--job-timeout",
        type=float,
        default=120.0,
        help="Per-job completion timeout in seconds",
    )
    parser.add_argument(
        "--output-json",
        help="Optional path for a JSON result dump",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    file_path = Path(args.file).resolve()
    if not file_path.is_file():
        raise SystemExit(f"input file not found: {file_path}")
    if args.requests <= 0:
        raise SystemExit("--requests must be > 0")
    if args.concurrency <= 0:
        raise SystemExit("--concurrency must be > 0")
    if args.poll_interval <= 0.0:
        raise SystemExit("--poll-interval must be > 0")

    runner = ServiceBenchmark(
        base_url=args.base_url,
        api_key=args.api_key,
        file_path=file_path,
        total_requests=args.requests,
        concurrency=args.concurrency,
        poll_interval=args.poll_interval,
        request_timeout=args.request_timeout,
        job_timeout=args.job_timeout,
        output_json=Path(args.output_json).resolve() if args.output_json else None,
    )
    return runner.run()


if __name__ == "__main__":
    raise SystemExit(main())
