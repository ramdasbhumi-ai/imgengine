// /* cmd/bench/lat_bench.c */
// #include <x86intrin.h>
// #include <stdio.h>
// #include "api/v1/img_core.h"

// void run_latency_test(img_engine_t engine, const char *img)
// {
//     const int iterations = 1000;
//     uint64_t results[iterations];

//     for (int i = 0; i < iterations; i++)
//     {
//         uint64_t start = __rdtsc();

//         // Execute heavy SIMD task
//         img_api_process_fast(engine, img, "/dev/null", 1920, 1080);

//         results[i] = __rdtsc() - start;
//     }

//     // Google-grade output: P50, P99 reporting logic here...
//     printf("[BENCH] Latency Test Complete. P99 Target: < 2.0ms\n");
// }


/* cmd/bench/lat_bench.c */
#include <x86intrin.h>
#include <stdio.h>
#include "api/v1/img_core.h"

int main() {
    img_engine_t engine = img_api_init(1); // Pin to Core 0
    const char* test_img = "tests/samples/4k_test.jpg";
    
    printf("[BENCH] Starting 1000 iteration stress test...\n");

    for (int i = 0; i < 1000; i++) {
        uint64_t start = __rdtsc();
        
        // Full Hot Path Execution
        img_api_process_fast(engine, test_img, "/dev/null", 1920, 1080);
        
        uint64_t end = __rdtsc();
        // Log cycles to a result array for P99 calculation...
    }

    img_api_shutdown(engine);
    return 0;
}
