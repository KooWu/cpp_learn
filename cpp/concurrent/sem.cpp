#include <iostream>
#include <thread>
#include <vector>
#include <semaphore>
#include <mutex>
#include <queue>
#include <chrono>
#include <condition_variable>

// ===== 1. std::counting_semaphore 示例 =====
std::counting_semaphore<3> countingSem(3);  // 允许3个线程并发访问

void countingSemDemo(int id) {
    countingSem.acquire();  // 获取信号量
    std::cout << "线程 " << id << " 进入临界区"<< std::endl;
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    countingSem.release();  // 释放信号量
    std::cout << "线程 " << id << " 离开临界区" << std::endl;
}

// ===== 2. std::binary_semaphore 示例 =====
std::binary_semaphore binarySem(1);  // 二进制信号量（等价于互斥锁）

void binarySemDemo(int id) {
    binarySem.acquire();  // 加锁
    std::cout << "二进制信号量: 线程 " << id << " 执行" << std::endl;
    binarySem.release();  // 解锁
}

// ===== 3. 资源池管理示例 =====
template<typename T>
class ResourcePool {
private:
    std::queue<T> resources;
    std::counting_semaphore<5> sem;  // 最多5个资源
    std::mutex mtx;

public:
    ResourcePool(int size) : sem(5) {
        for (int i = 0; i < size; ++i) {
            resources.push(T(i));  // 初始化资源池
        }
    }

    T getResource() {
        sem.acquire();  // 等待可用资源
        std::lock_guard<std::mutex> lock(mtx);
        T resource = resources.front();
        resources.pop();
        return resource;
    }

    void returnResource(T resource) {
        std::lock_guard<std::mutex> lock(mtx);
        resources.push(resource);
        sem.release();  // 归还资源
    }
};

// ===== 4. 生产者-消费者模型示例 =====
std::queue<int> producerConsumerBuffer;
const int BUFFER_SIZE = 5;
std::counting_semaphore<BUFFER_SIZE> emptySlots(BUFFER_SIZE);  // 空槽数量
std::counting_semaphore<0> items(0);                          // 数据项数量
std::mutex bufferMtx;

void producerTask(int id) {
    for (int i = 0; i < 10; ++i) {
        emptySlots.acquire();  // 等待空槽
        
        {
            std::lock_guard<std::mutex> lock(bufferMtx);
            producerConsumerBuffer.push(id * 10 + i);
            std::cout << "生产者 " << id << " 放入: " << id * 10 + i << std::endl;
        }
        
        items.release();  // 通知有新数据
    }
}

void consumerTask(int id) {
    for (int i = 0; i < 10; ++i) {
        items.acquire();  // 等待数据
        
        {
            std::lock_guard<std::mutex> lock(bufferMtx);
            int item = producerConsumerBuffer.front();
            producerConsumerBuffer.pop();
            std::cout << "消费者 " << id << " 取出: " << item << std::endl;
        }
        
        emptySlots.release();  // 通知有空槽
    }
}

// ===== 5. C++20 前自定义信号量实现 =====
class LegacySemaphore {
private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;

public:
    LegacySemaphore(int initialCount) : count(initialCount) {}

    void acquire() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]() { return count > 0; });
        --count;
    }

    void release() {
        std::lock_guard<std::mutex> lock(mtx);
        ++count;
        cv.notify_one();
    }
};

void legacySemaphoreDemo() {
    LegacySemaphore sem(2);
    std::thread t1([&]() { sem.acquire(); std::cout << "LegacySemaphore: 线程1" << std::endl; });
    std::thread t2([&]() { sem.acquire(); std::cout << "LegacySemaphore: 线程2" << std::endl; });
    t1.join();
    t2.join();
    sem.release();
    sem.release();
}

int main() {
    std::cout << "=== 1. counting_semaphore 示例 ===" << std::endl;
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(countingSemDemo, i);
    }
    for (auto& t : threads) t.join();
    threads.clear();

    std::cout << "\n=== 2. binary_semaphore 示例 ===" << std::endl;
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back(binarySemDemo, i);
    }
    for (auto& t : threads) t.join();
    threads.clear();

    std::cout << "\n=== 3. 资源池管理示例 ===" << std::endl;
    ResourcePool<int> pool(5);
    for (int i = 0; i < 8; ++i) {
        threads.emplace_back([&pool]() {
            int res = pool.getResource();
            std::cout << "获取资源: " << res << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            pool.returnResource(res);
            std::cout << "归还资源: " << res << std::endl;
        });
    }
    for (auto& t : threads) t.join();
    threads.clear();

    std::cout << "\n=== 4. 生产者-消费者模型 ===" << std::endl;
    std::thread producer1(producerTask, 1);
    std::thread producer2(producerTask, 2);
    std::thread consumer1(consumerTask, 1);
    std::thread consumer2(consumerTask, 2);
    
    producer1.join();
    producer2.join();
    consumer1.join();
    consumer2.join();

    std::cout << "\n=== 5. 自定义信号量（C++20前） ===" << std::endl;
    legacySemaphoreDemo();

    return 0;
}