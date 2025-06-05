#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <shared_mutex>
#include <chrono>

// ===== 1. std::condition_variable 示例 =====
std::condition_variable cv;
std::mutex mtx;
bool dataReady = false;

void cvProducer() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    {
        std::lock_guard<std::mutex> lock(mtx);
        dataReady = true;
        std::cout << "cvProducer: 数据已就绪" << std::endl;
    }
    
    cv.notify_one();  // 通知等待线程
}

void cvConsumer() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []{ return dataReady; });  // 等待数据就绪
    std::cout << "cvConsumer: 处理数据" << std::endl;
}

// ===== 2. std::condition_variable_any 示例 =====
std::condition_variable_any cv_any;
std::shared_mutex sharedMtx;  // 可与shared_mutex配合
bool resourceReady = false;

void anyProducer() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    {
        std::unique_lock<std::shared_mutex> lock(sharedMtx);
        resourceReady = true;
        std::cout << "anyProducer: 资源已就绪" << std::endl;
    }
    
    cv_any.notify_one();  // 通知等待线程
}

void anyConsumer() {
    // 可使用不同类型的锁，如std::shared_lock
    std::shared_lock<std::shared_mutex> lock(sharedMtx);
    
    // 使用自定义锁类型（需满足BasicLockable要求）
    class CustomLock {
    private:
        std::mutex& mtx;
    public:
        explicit CustomLock(std::mutex& m) : mtx(m) { mtx.lock(); }
        ~CustomLock() { mtx.unlock(); }
    };
    
    std::mutex customMtx;
    CustomLock customLock(customMtx);
    
    // 等待时释放锁，唤醒后重新加锁
    cv_any.wait(lock, []{ return resourceReady; });
    std::cout << "anyConsumer: 使用资源" << std::endl;
}

// ===== 3. 性能对比 =====
#include <atomic>

void performanceTest() {
    constexpr int iterations = 1000000;
    std::condition_variable cv_perf;
    std::condition_variable_any cv_any_perf;
    std::mutex mtx_perf;
    std::atomic<bool> ready(false);
    
    // 测试std::condition_variable
    auto start = std::chrono::high_resolution_clock::now();
    {
        std::thread t([&] {
            std::unique_lock<std::mutex> lock(mtx_perf);
            cv_perf.wait(lock, [&]{ return ready.load(); });
        });
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        {
            std::lock_guard<std::mutex> lock(mtx_perf);
            ready = true;
        }
        cv_perf.notify_one();
        t.join();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "std::condition_variable 耗时: " 
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
              << " 微秒" << std::endl;
    
    // 测试std::condition_variable_any
    ready = false;
    start = std::chrono::high_resolution_clock::now();
    {
        std::thread t([&] {
            std::unique_lock<std::mutex> lock(mtx_perf);
            cv_any_perf.wait(lock, [&]{ return ready.load(); });
        });
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        {
            std::lock_guard<std::mutex> lock(mtx_perf);
            ready = true;
        }
        cv_any_perf.notify_one();
        t.join();
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "std::condition_variable_any 耗时: " 
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
              << " 微秒" << std::endl;
}

int main() {
    std::cout << "=== 1. std::condition_variable 示例 ===" << std::endl;
    std::thread t1(cvProducer);
    std::thread t2(cvConsumer);
    t1.join();
    t2.join();
    
    std::cout << "\n=== 2. std::condition_variable_any 示例 ===" << std::endl;
    std::thread t3(anyProducer);
    std::thread t4(anyConsumer);
    t3.join();
    t4.join();
    
    std::cout << "\n=== 3. 性能对比 ===" << std::endl;
    performanceTest();
    
    return 0;
}    