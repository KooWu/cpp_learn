#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <memory>
#include <chrono>
#include <mutex>

// ===== 1. 原子计数器（relaxed顺序） =====
std::atomic<int> counter(0);

void atomicCounterDemo() {
    auto worker = [](int id) {
        for (int i = 0; i < 100000; ++i) {
            counter.fetch_add(1, std::memory_order_relaxed);
        }
        std::cout << "线程 " << id << " 完成" << std::endl;
    };

    std::thread t1(worker, 1);
    std::thread t2(worker, 2);
    t1.join();
    t2.join();

    std::cout << "最终计数: " << counter << std::endl; // 应输出200000
}

// ===== 2. release-acquire内存顺序 =====
std::atomic<int> data(0);
std::atomic<bool> flag(false);

void releaseAcquireDemo() {
    auto producer = []() {
        data.store(42, std::memory_order_relaxed);
        flag.store(true, std::memory_order_release); // 释放屏障
    };

    auto consumer = []() {
        while (!flag.load(std::memory_order_acquire)); // 获取屏障
        std::cout << "消费者读取数据: " << data.load(std::memory_order_relaxed) << std::endl;
    };

    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join();
    t2.join();
}

// ===== 3. 全序列一致性(seq_cst) =====
std::atomic<int> x(0), y(0);
std::atomic<int> r1(0), r2(0);

void sequentialConsistencyDemo() {
    auto threadA = []() {
        x.store(1, std::memory_order_seq_cst);
        r1.store(y.load(std::memory_order_seq_cst), std::memory_order_seq_cst);
    };

    auto threadB = []() {
        y.store(1, std::memory_order_seq_cst);
        r2.store(x.load(std::memory_order_seq_cst), std::memory_order_seq_cst);
    };

    x = 0; y = 0; r1 = 0; r2 = 0;
    std::thread t1(threadA);
    std::thread t2(threadB);
    t1.join();
    t2.join();

    std::cout << "r1 = " << r1 << ", r2 = " << r2 << std::endl;
    // 在seq_cst下，不可能同时r1==0 && r2==0
}

// ===== 5. 原子标志位(atomic_flag) =====
std::atomic_flag spinlock = ATOMIC_FLAG_INIT;

void atomicFlagDemo() {
    auto worker = [](int id) {
        for (int i = 0; i < 1000; ++i) {
            // 获取锁
            while (spinlock.test_and_set(std::memory_order_acquire));
            
            // 临界区
            std::cout << "线程 " << id << " 进入临界区" << std::endl;
            
            // 释放锁
            spinlock.clear(std::memory_order_release);
        }
    };

    std::thread t1(worker, 1);
    std::thread t2(worker, 2);
    t1.join();
    t2.join();
}

// ===== 6. 原子操作性能对比 =====
void performanceComparison() {
    const int iterations = 10000000;
    std::atomic<int> atomicCounter(0);
    int nonAtomicCounter = 0;
    std::mutex mtx;

    // 测试原子操作
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        atomicCounter.fetch_add(1, std::memory_order_relaxed);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "原子操作耗时: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms" << std::endl;

    // 测试互斥锁
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        nonAtomicCounter++;
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "互斥锁操作耗时: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms" << std::endl;
}

int main() {
    std::cout << "=== 1. 原子计数器(relaxed) ===" << std::endl;
    atomicCounterDemo();

    std::cout << "\n=== 2. release-acquire内存顺序 ===" << std::endl;
    releaseAcquireDemo();

    std::cout << "\n=== 3. 全序列一致性(seq_cst) ===" << std::endl;
    sequentialConsistencyDemo();

    std::cout << "\n=== 5. 原子标志位(spinlock) ===" << std::endl;
    atomicFlagDemo();

    std::cout << "\n=== 6. 性能对比 ===" << std::endl;
    performanceComparison();

    return 0;
}