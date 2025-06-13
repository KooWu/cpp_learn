#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <signal.h>
#include <malloc.h>

#include "jemalloc/jemalloc.h"

#include <gperftools/heap-checker.h>
#include <gperftools/heap-profiler.h>

// 信号处理函数
void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    exit(1);
}

static void mem_leak11() {
    int *q = new int[20];
    q[0] = 1;
}

static void mem_req() {
    int *q = new int[40];
    q[0] = 1;
    delete[] q;
}

static void LeakThread() {
    int32_t koo = 100;
    std::string name = "hello";
    int *q = new int[20];
    q[0] = 1;
}

static void NormalThread() {
    int32_t koo = 100;
    std::string name = "hello";
    int *q = new int[80];
    q[0] = 1;
    delete[] q;
}

int main(int argc, char **argv) {
    // 注册信号处理函数
#ifdef ENABLE_TC_DUMP
    HeapProfilerStart("before_leak");
#endif
    signal(SIGINT, signalHandler); // 捕获Ctrl+C信号
    signal(SIGTERM, signalHandler); // 捕获终止信号
    
    std::cout << "Starting pid..." << getpid() << std::endl;
    int32_t i = 0;
#ifndef ENABLE_TC_DUMP
    mallctl("prof.dump", NULL, NULL, NULL, 0);
#endif
    mem_leak11();
#ifndef ENABLE_TC_DUMP
    mallctl("prof.dump", NULL, NULL, NULL, 0);
#endif
#ifdef ENABLE_TC_DUMP
    HeapProfilerDump("after_leak1");
#endif
    mem_req();
#ifdef ENABLE_TC_DUMP
    HeapProfilerDump("after_leak2");
#endif
    std::thread leakThread = std::thread(LeakThread);
    std::thread normalThread = std::thread(NormalThread);
    while (i < 10) {
        int *p = new int[20];  // 持续泄漏
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "i = " << i << std::endl;
        i++;
    }
#ifndef ENABLE_TC_DUMP
    mallctl("prof.dump", NULL, NULL, NULL, 0);
#endif
    leakThread.join();
    normalThread.join();
#ifdef ENABLE_TC_DUMP
    HeapProfilerDump("after_leak3");
#endif
    return 0;
}