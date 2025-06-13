#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <random>
#include <cmath>

// gperftools 头文件
#include <gperftools/profiler.h>
#include <gperftools/heap-profiler.h>
#include <gperftools/heap-checker.h>

// 模拟计算密集型任务
void cpu_intensive_task(int iterations) {
    double result = 0.0;
    for (int i = 0; i < iterations; i++) {
        result += std::sin(i) * std::cos(i);
    }
    std::cout << "CPU 密集型任务完成，结果: " << result << std::endl;
}

// 模拟内存密集型任务
void memory_intensive_task(int num_vectors, int vector_size) {
    std::vector<std::vector<int>> vectors;
    
    for (int i = 0; i < num_vectors; i++) {
        std::vector<int> vec(vector_size, i);
        vectors.push_back(vec);
    }
    
    // 随机修改一些值
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, num_vectors - 1);
    
    for (int i = 0; i < 10000; i++) {
        int index = dis(gen);
        if (index < vectors.size()) {
            vectors[index][i % vector_size] = i;
        }
    }
    
    std::cout << "内存密集型任务完成，分配了 " << num_vectors 
              << " 个大小为 " << vector_size << " 的向量" << std::endl;
}

// 模拟内存泄漏
void memory_leak_task() {
    // 故意泄漏内存
    for (int i = 0; i < 1000; i++) {
        int* ptr = new int[1000];
        // 不释放内存
    }
    std::cout << "内存泄漏任务完成（已泄漏约 4MB 内存）" << std::endl;
}

// 多线程任务
void thread_task(int id, int iterations) {
    std::cout << "线程 " << id << " 开始执行" << std::endl;
    
    // 分配一些内存
    std::vector<std::vector<int>> data;
    for (int i = 0; i < 100; i++) {
        data.emplace_back(1000, id);
    }
    
    // 执行一些计算
    double result = 0.0;
    for (int i = 0; i < iterations; i++) {
        result += std::sqrt(i) * std::log(i + 1);
    }
    
    std::cout << "线程 " << id << " 完成，结果: " << result << std::endl;
}

int main(int argc, char* argv[]) {
    // 解析命令行参数
    bool enable_cpu_profiler = false;
    bool enable_heap_profiler = false;
    bool enable_heap_checker = false;
    bool run_leak_test = false;
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--cpu-profiler") {
            enable_cpu_profiler = true;
        } else if (arg == "--heap-profiler") {
            enable_heap_profiler = true;
        } else if (arg == "--heap-checker") {
            enable_heap_checker = true;
        } else if (arg == "--leak-test") {
            run_leak_test = true;
        }
    }
    
    // 初始化堆检查器（如果启用）
    HeapLeakChecker* heap_checker = nullptr;
    if (enable_heap_checker) {
        heap_checker = new HeapLeakChecker("my_program");
        std::cout << "堆检查器已启用" << std::endl;
    }
    
    // 启动 CPU 分析器（如果启用）
    if (enable_cpu_profiler) {
        ProfilerStart("my_program.prof");
        std::cout << "CPU 分析器已启动，输出到 my_program.prof" << std::endl;
    }
    
    // 启动堆分析器（如果启用）
    if (enable_heap_profiler) {
        HeapProfilerStart("my_program.heap");
        std::cout << "堆分析器已启动，输出到 my_program.heap" << std::endl;
    }
    
    // 执行 CPU 密集型任务
    std::cout << "\n执行 CPU 密集型任务..." << std::endl;
    cpu_intensive_task(10000000);
    
    // 执行内存密集型任务
    std::cout << "\n执行内存密集型任务..." << std::endl;
    memory_intensive_task(1000, 1000);
    
    // 执行多线程任务
    std::cout << "\n执行多线程任务..." << std::endl;
    std::vector<std::thread> threads;
    for (int i = 0; i < 8; i++) {
        threads.emplace_back(thread_task, i, 1000000);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    // 执行内存泄漏任务（如果启用）
    if (run_leak_test) {
        std::cout << "\n执行内存泄漏测试..." << std::endl;
        memory_leak_task();
    }
    
    // 停止分析器
    if (enable_cpu_profiler) {
        ProfilerStop();
        std::cout << "CPU 分析器已停止" << std::endl;
    }
    
    if (enable_heap_profiler) {
        HeapProfilerStop();
        std::cout << "堆分析器已停止" << std::endl;
    }
    
    // 检查堆泄漏（如果启用）
    if (heap_checker) {
        bool no_leaks = heap_checker->NoLeaks();
        delete heap_checker;
        std::cout << "堆检查完成，" << (no_leaks ? "没有发现泄漏" : "发现泄漏") << std::endl;
    }
    
    std::cout << "\n程序执行完毕" << std::endl;
    return 0;
}
