#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <random>
#include <string>
#include <cstring>
#include <jemalloc/jemalloc.h>


// ===== 1. 基本使用示例 =====
void basic_usage_example() {
    std::cout << "\n=== 基本使用示例 ===\n" << std::endl;
    
    // 使用标准内存分配函数（已被 jemalloc 替换）
    std::vector<int> vec(1000000);
    std::cout << "Vector size: " << vec.size() << std::endl;
    std::cout << "Vector capacity: " << vec.capacity() << std::endl;
    
    // 使用 jemalloc 特定函数，没有--with-jemalloc-prefix=je_ 进行重命名，直接同名劫持
    void* ptr = malloc(1024);
    if (ptr) {
        std::cout << "Allocated 1024 bytes at " << ptr << std::endl;
        
        // 检查分配大小
        size_t allocated_size = malloc_usable_size(ptr);
        std::cout << "Usable size: " << allocated_size << " bytes" << std::endl;
        
        // 重新分配
        ptr = realloc(ptr, 2048);
        std::cout << "Reallocated to 2048 bytes at " << ptr << std::endl;
        
        // 释放内存
        free(ptr);
        std::cout << "Memory freed" << std::endl;
    }
    
    // 使用对齐内存分配
    void* aligned_ptr;
    if (posix_memalign(&aligned_ptr, 64, 4096) == 0) {
        std::cout << "Allocated 4096 bytes aligned to 64 bytes at " << aligned_ptr << std::endl;
        free(aligned_ptr);
    }
}

// ===== 3. 性能测试示例 =====
void benchmark_example() {
    std::cout << "\n=== 性能测试示例 ===\n" << std::endl;
    
    const int num_threads = 8;
    const int iterations = 10000;
    
    std::vector<std::thread> threads;
    
    // 线程函数：执行内存分配和释放
    auto thread_function = [iterations](int id) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1024, 1024 * 1024); // 1KB 到 1MB
        
        std::vector<void*> pointers;
        
        for (int i = 0; i < iterations; i++) {
            // 随机分配大小
            size_t size = dis(gen);
            void* ptr = malloc(size);
            
            if (ptr) {
                pointers.push_back(ptr);
                
                // 随机释放一些内存
                if (pointers.size() > 100 && i % 10 == 0) {
                    size_t index = i % pointers.size();
                    free(pointers[index]);
                    pointers.erase(pointers.begin() + index);
                }
            }
        }
        
        // 释放所有剩余内存
        for (void* ptr : pointers) {
            free(ptr);
        }
        
        std::cout << "Thread " << id << " completed" << std::endl;
    };
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // 创建多个线程同时进行内存分配和释放
    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back(thread_function, i);
    }
    
    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    
    std::cout << "Total time: " << elapsed.count() << " seconds" << std::endl;
}

// ===== 4. 内存统计和分析 =====
void memory_stats_example() {
    std::cout << "\n=== 内存统计示例 ===\n" << std::endl;
    
    // 分配一些内存
    std::vector<void*> allocations;
    for (int i = 0; i < 100; i++) {
        void* ptr = malloc(1024 * 1024); // 1MB
        if (ptr) {
            allocations.push_back(ptr);
        }
    }
    
    std::cout << "Allocated " << allocations.size() << " blocks of 1MB each" << std::endl;
    
    // 打印内存统计信息
    std::cout << "\n内存统计信息:\n" << std::endl;
    malloc_stats_print(NULL, NULL, NULL);
    
    // 释放部分内存
    for (size_t i = 0; i < allocations.size() / 2; i++) {
        free(allocations[i]);
    }
    
    std::cout << "\n释放一半内存后...\n" << std::endl;
    malloc_stats_print(NULL, NULL, NULL);
    
    // 释放所有内存
    for (size_t i = allocations.size() / 2; i < allocations.size(); i++) {
        free(allocations[i]);
    }
    
    std::cout << "\n释放所有内存后...\n" << std::endl;
    malloc_stats_print(NULL, NULL, NULL);
}

// ===== 5. 内存分析工具 =====
void memory_profiling_example() {
    std::cout << "\n=== 内存分析示例 ===\n" << std::endl;
    
    std::cout << "此示例需要通过环境变量配置内存分析：" << std::endl;
    std::cout << "MALLOC_CONF=\"prof:true,lg_prof_interval:29,prof_prefix:jeprof.out\" ./your_program" << std::endl;
    std::cout << "然后可以使用 jeprof 工具分析生成的文件：" << std::endl;
    std::cout << "jeprof --text ./your_program jeprof.out.*" << std::endl;
    std::cout << "jeprof --pdf ./your_program jeprof.out.* > memory_profile.pdf" << std::endl;
    
    // 为了生成分析数据，这里执行一些内存操作
    std::vector<std::vector<int>> matrices;
    for (int i = 0; i < 100; i++) {
        std::vector<int> matrix(1000 * 1000);
        matrices.push_back(matrix);
        
        if (i % 10 == 0) {
            matrices.resize(matrices.size() / 2);
        }
    }
    
    std::cout << "执行了内存分配和释放操作，请使用 jeprof 分析生成的数据" << std::endl;
}

// ===== 6. 内存泄漏检测 =====
void memory_leak_detection_example() {
    std::cout << "\n=== 内存泄漏检测示例 ===\n" << std::endl;
    
    // 这个函数故意泄漏内存，用于演示
    std::cout << "警告：这个示例会故意泄漏内存用于演示目的" << std::endl;
    
    // 分配内存但不释放
    void* leak1 = malloc(1024 * 1024); // 1MB
    void* leak2 = malloc(2 * 1024 * 1024); // 2MB
    
    std::cout << "已分配但未释放的内存块: " << leak1 << " 和 " << leak2 << std::endl;
    std::cout << "使用 MALLOC_CONF=\"abort_conf:true,abort:false\" 运行程序可以检测内存泄漏" << std::endl;
    
    // 打印内存统计，应该显示这些泄漏的内存
    malloc_stats_print(NULL, NULL, NULL);
}

// ===== 主函数 =====
int main() {
    std::cout << "===== jemalloc 完整示例 =====" << std::endl;
    
    // 打印 jemalloc 版本信息
    std::cout << "jemalloc 版本: " << JEMALLOC_VERSION << std::endl;
    
    // 显示当前 jemalloc 配置
    const char* config_env = getenv("MALLOC_CONF");
    std::cout << "当前配置: " << (config_env ? config_env : "未设置环境变量") << std::endl;
    
    // 运行各个示例
    basic_usage_example();
    benchmark_example();
    // memory_stats_example();
    memory_profiling_example();
    // memory_leak_detection_example();
    
    std::cout << "\n===== 示例执行完毕 =====" << std::endl;
    return 0;
}
