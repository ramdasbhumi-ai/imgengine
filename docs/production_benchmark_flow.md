# Production Benchmark Flow

This repo now has three different benchmark layers. They answer different questions and should not be mixed together.

## 1. Engine truth

Use `bench_lat` when you want to measure the engine itself, especially the prepared hot path.

From `/workspaces/imgengine/imgengine`:

```bash
./scripts/production_benchmark.sh
```

Or run `bench_lat` directly:

```bash
cmake -S /workspaces/imgengine/imgengine -B /workspaces/imgengine/imgengine/build
cmake --build /workspaces/imgengine/imgengine/build --target bench_lat
/workspaces/imgengine/imgengine/build/bench_lat /workspaces/imgengine/imgengine/photo.jpg
```

Optional encode tradeoff sweep:

```bash
/workspaces/imgengine/imgengine/build/bench_lat --encode-sweep /workspaces/imgengine/imgengine/photo.jpg
```

What to watch:

- `Prepared render stage` is the closest number to the RFC render-core claim.
- `Prepared decode ingress` isolates decoder cost.
- `Prepared encode only` isolates JPEG encode cost using the current engine defaults.
- `RFC scorecard` gives a fast pass/fail read against the RFC targets.

## 2. CLI process overhead

Use `imgengine_cli_bench` when you want to measure the cost of spawning `imgengine_cli` per job.

```bash
cmake --build /workspaces/imgengine/imgengine/build --target imgengine_cli_bench
/workspaces/imgengine/imgengine/build/imgengine_cli_bench --runs 100
```

Raw-RGB24 CLI benchmark path:

```bash
/workspaces/imgengine/imgengine/build/imgengine_cli_bench --runs 100 --prepare-raw-rgb24
```

What this includes:

- `fork` / `exec`
- CLI argument parsing
- image decode and encode
- file output

What `--prepare-raw-rgb24` changes:

- the benchmark decodes the source image once before the run
- the CLI is then executed in `raw-rgb24` mode
- this isolates CLI/process/output cost from repeated decode cost

What this does not prove:

- It does not prove kernel-grade hot-path behavior.
- It does not represent a long-lived service worker.

## 3. Service path benchmark

Use the SaaS benchmark when you want to measure HTTP submit latency, queueing, worker time, and end-to-end completion.

The public API route is rate-limited. For benchmark runs, override the limiter in the benchmark environment:

```bash
cd /workspaces/imgengine/imgengine-saas/api-service
API_GENERATE_RATE_LIMIT=100000/minute uv run python ../data/testing_scripts/service_benchmark.py --requests 20
```

Default input file:

- `/workspaces/imgengine/imgengine-saas/data/testing_scripts/test.jpg`

Useful options:

```bash
API_GENERATE_RATE_LIMIT=100000/minute uv run python ../data/testing_scripts/service_benchmark.py \
  --base-url http://127.0.0.1:8000 \
  --requests 50 \
  --concurrency 8 \
  --poll-interval 0.20 \
  --job-timeout 180 \
  --output-json /workspaces/imgengine/imgengine-saas/data/logs/service-benchmark.json
```

What this reports:

- submission latency
- end-to-end latency
- queued-to-processing latency
- processing-to-complete latency
- completed jobs/sec
- top failure reasons

## Reading the numbers

Use the results in this order:

1. `bench_lat` hot path tells you whether the core engine is even close to the RFC goals.
2. `imgengine_cli_bench` shows how much single-shot CLI process overhead costs.
3. `service_benchmark.py` shows queueing, API overhead, worker behavior, and production bottlenecks.

The prepared `bench_lat` metric is not a full decode+render+encode number.
It measures prepared render-stage work with final-cache hits disabled, so it is useful for engine-core comparison but should still be read alongside decode and cold-path results.

If `bench_lat` is fast but the service benchmark is slow, the bottleneck is outside the engine.

Common examples:

- high submit latency: API, upload, DB, or limiter overhead
- high queue latency: too few workers or a backlog
- high processing latency with low queue latency: worker path or engine path is slow
- much slower CLI than hot path: process spawn and file I/O are dominating
