#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <shared_mutex>  // C++17 读写锁
#include <string>
#include <chrono>
#include <random>
#include <memory>

// ===== 1. 生产者-消费者模式 =====
template<typename T, size_t Capacity = 10>
class ThreadSafeQueue {
private:
    std::queue<T> queue_;
    mutable std::shared_mutex mutex_;  // 读写锁
    std::condition_variable_any not_full_;
    std::condition_variable_any not_empty_;
    std::atomic<bool> stopped_{false};

public:
    // 生产者：添加元素
    bool enqueue(const T& item, std::chrono::milliseconds timeout = std::chrono::milliseconds(0)) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        
        // 等待队列非满或超时或停止
        if (!not_full_.wait_for(lock, timeout, 
            [this] { return !this->is_full() || stopped_; })) {
            return false;  // 超时
        }
        
        if (stopped_) return false;  // 已停止
        
        queue_.push(item);
        not_empty_.notify_one();  // 通知消费者
        return true;
    }

    // 消费者：获取元素
    bool dequeue(T& item, std::chrono::milliseconds timeout = std::chrono::milliseconds(0)) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        
        // 等待队列非空或超时或停止
        if (!not_empty_.wait_for(lock, timeout, 
            [this] { return !this->is_empty() || stopped_; })) {
            return false;  // 超时
        }
        
        if (stopped_) return false;  // 已停止
        
        item = queue_.front();
        queue_.pop();
        not_full_.notify_one();  // 通知生产者
        return true;
    }

    // 停止队列（唤醒所有等待线程）
    void stop() {
        {
            std::unique_lock<std::shared_mutex> lock(mutex_);
            stopped_ = true;
        }
        not_full_.notify_all();
        not_empty_.notify_all();
    }

    // 检查队列状态
    bool is_empty() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return queue_.empty();
    }

    bool is_full() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return queue_.size() >= Capacity;
    }

    size_t size() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return queue_.size();
    }
};

// ===== 2. 读写分离模式 =====
template<typename T>
class ReadWriteCache {
private:
    T data_;
    mutable std::shared_mutex rw_mutex_;
    std::atomic<bool> cache_valid_{false};

public:
    // 写入数据（排他锁）
    void write(const T& new_data) {
        std::unique_lock<std::shared_mutex> lock(rw_mutex_);
        data_ = new_data;
        cache_valid_ = true;
        std::cout << "写入数据: " << data_ << std::endl;
    }

    // 读取数据（共享锁）
    T read() const {
        std::shared_lock<std::shared_mutex> lock(rw_mutex_);
        
        // 模拟缓存失效
        if (!cache_valid_) {
            // 实际应用中可能需要从数据库或其他源加载
            std::cout << "缓存失效，重新加载数据..." << std::endl;
            // 此处需升级为写锁（示例略，实际需 unlock 后重新 lock）
        }
        
        return data_;
    }

    // 使缓存失效（用于演示）
    void invalidate_cache() {
        std::unique_lock<std::shared_mutex> lock(rw_mutex_);
        cache_valid_ = false;
    }
};

// ===== 3. 演示程序 =====
void producer_consumer_demo() {
    std::cout << "\n=== 生产者-消费者模式演示 ===" << std::endl;
    ThreadSafeQueue<int, 5> queue;
    std::atomic<bool> running(true);

    // 生产者线程
    auto producer = [&queue, &running]() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);
        
        while (running) {
            int value = dis(gen);
            if (queue.enqueue(value)) {
                std::cout << "生产者生产: " << value << " (队列大小: " << queue.size() << ")" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    };

    // 消费者线程
    auto consumer = [&queue, &running]() {
        while (running) {
            int value;
            if (queue.dequeue(value, std::chrono::milliseconds(500))) {
                std::cout << "消费者消费: " << value << " (队列大小: " << queue.size() << ")" << std::endl;
            }
        }
    };

    // 启动线程
    std::thread p1(producer);
    std::thread p2(producer);
    std::thread c1(consumer);
    std::thread c2(consumer);

    // 运行一段时间后停止
    std::this_thread::sleep_for(std::chrono::seconds(3));
    running = false;
    queue.stop();

    p1.join();
    p2.join();
    c1.join();
    c2.join();
}

void read_write_lock_demo() {
    std::cout << "\n=== 读写分离模式演示 ===" << std::endl;
    ReadWriteCache<std::string> cache;
    std::atomic<bool> running(true);

    // 写线程
    auto writer = [&cache, &running]() {
        int count = 0;
        while (running) {
            cache.write("Data version " + std::to_string(++count));
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    };

    // 读线程
    auto reader = [&cache, &running](int id) {
        while (running) {
            std::string data = cache.read();
            std::cout << "读者 " << id << " 读取: " << data << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    };

    // 启动线程
    std::thread w(writer);
    std::thread r1(reader, 1);
    std::thread r2(reader, 2);
    std::thread r3(reader, 3);

    // 运行一段时间后停止
    std::this_thread::sleep_for(std::chrono::seconds(5));
    running = false;

    w.join();
    r1.join();
    r2.join();
    r3.join();
}

int main() {
    producer_consumer_demo();
    read_write_lock_demo();
    return 0;
}