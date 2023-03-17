#ifndef H_BENCHMARK
#define H_BENCHMARK

#ifdef DEBUG
#include <stdio.h>
#include <time.h>
#endif // DEBUG

#ifdef DEBUG
#define benchmark(name)                                                        \
    for (clock_t _i_ = 0, _benchmark_start = clock(), _benchmark_end = 0;      \
         !_i_; _i_ += 1, _benchmark_end = clock(),                             \
                 printf("Benchmark: %s time=%.4f sec.\n", #name,               \
                        (double)(_benchmark_end - _benchmark_start) /          \
                            CLOCKS_PER_SEC))
#endif // ifdef DEBUG

#ifndef DEBUG
#define benchmark(name) for (int _i_ = 0; !_i_; _i_++)
#endif // ifndef DEBUG

#endif // !H_BENCHMARK
