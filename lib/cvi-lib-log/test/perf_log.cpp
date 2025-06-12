#include <benchmark/benchmark.h>
#include <iostream>
#include "log_service.h"
#include "MWPLog.h"

static bool g_initialized = false;

static void BM_SingleThreadLogging(benchmark::State& state) {
    // 设置测试环境
    std::string appName = "test_log_app";

    if (!g_initialized) {
        log_system_init(appName.c_str());
        g_initialized = true;
    }

    for (auto _ : state) {
        CVI_ERROR("This is a test log message for performance testing");
    }
}
BENCHMARK(BM_SingleThreadLogging);

static void BM_MultiThreadLogging(benchmark::State& state) {
    std::string appName = "test_log_app";
    if (!g_initialized) {
        log_system_init(appName.c_str());
        g_initialized = true;
    }
    
    for (auto _ : state) {
        state.PauseTiming();
        std::vector<std::thread> threads;
        const int num_threads = state.range(0);
        
        state.ResumeTiming();
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back([]{
                CVI_ERROR("Multi-thread test log message");
            });
        }
        
        for (auto& t : threads) {
            t.join();
        }
    }
}
BENCHMARK(BM_MultiThreadLogging)->Range(1, 8);

static void BM_DifferentLevels(benchmark::State& state) {
    std::string appName = "test_log_app";
    if (!g_initialized) {
        log_system_init(appName.c_str());
        g_initialized = true;
    }
    
    for (auto _ : state) {
        switch(state.range(0)) {
            case 0: CVI_TRACE("Trace message"); break;
            case 1: CVI_DEBUG("Debug message"); break;
            case 2: CVI_INFO("Info message"); break;
            case 3: CVI_WARN("Warn message"); break;
            case 4: CVI_ERROR("Error message"); break;
            case 5: CVI_FATAL("Critical message"); break;
        }
    }
}
BENCHMARK(BM_DifferentLevels)->DenseRange(0, 5);

BENCHMARK_MAIN();