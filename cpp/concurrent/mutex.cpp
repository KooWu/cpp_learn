#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <chrono>
#include <stdexcept>
#include <atomic>
#include <condition_variable>

// ===== 1. 基础互斥锁：std::mutex =====
std::mutex mtx;
int sharedData = 0;

void basicMutexDemo() {
    auto worker = [](int id) {
        for (int i = 0; i < 1000; ++i) {
            std::lock_guard<std::mutex> lock(mtx);
            sharedData++;
            std::cout << "线程" << id << ": " << sharedData << std::endl;
        }
    };

    std::thread t1(worker, 1);
    std::thread t2(worker, 2);
    t1.join();
    t2.join();
}

// ===== 2. 递归互斥锁：std::recursive_mutex =====
std::recursive_mutex rmtx;

void recursiveFunction(int depth) {
    std::lock_guard<std::recursive_mutex> lock(rmtx);
    if (depth == 0) return;
    std::cout << "递归层级: " << depth << std::endl;
    recursiveFunction(depth - 1);
}

// ===== 3. 带超时的互斥锁：std::timed_mutex =====
std::timed_mutex tmtx;

void timedLockDemo() {
    std::thread t1([]{
        tmtx.lock();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        tmtx.unlock();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    std::thread t2([]{
        if (tmtx.try_lock_for(std::chrono::seconds(1))) {
            std::cout << "线程2获取锁成功" << std::endl;
            tmtx.unlock();
        } else {
            std::cout << "线程2获取锁超时" << std::endl;
        }
    });

    t1.join();
    t2.join();
}

// ===== 4. 读写锁：std::shared_mutex =====
class ThreadSafeCounter {
private:
    mutable std::shared_mutex mtx;
    int count = 0;
public:
    int read() const {
        std::shared_lock<std::shared_mutex> lock(mtx);
        return count;
    }

    void write(int value) {
        std::unique_lock<std::shared_mutex> lock(mtx);
        count = value;
    }
};

// ===== 5. 死锁示例与解决 =====
std::mutex mtx1, mtx2;

void deadlockThread1() {
    std::lock_guard<std::mutex> lock1(mtx1);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::lock_guard<std::mutex> lock2(mtx2); // 等待mtx2
}

void deadlockThread2() {
    std::lock_guard<std::mutex> lock2(mtx2);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::lock_guard<std::mutex> lock1(mtx1); // 等待mtx1
}

void deadlockDemo() {
    std::thread t1(deadlockThread1);
    std::thread t2(deadlockThread2);
    t1.join();
    t2.join(); // 可能死锁
}

void safeThread1() {
    std::scoped_lock lock(mtx1, mtx2); // 原子性获取锁
}

void safeThread2() {
    std::scoped_lock lock(mtx1, mtx2); // 顺序与thread1一致
}

// ===== 6. 锁粒度过大示例 =====
void slowFunction() {
    std::lock_guard<std::mutex> lock(mtx);
    std::this_thread::sleep_for(std::chrono::seconds(1)); // 模拟耗时操作
    sharedData++;
}

void fastFunction() {
    std::this_thread::sleep_for(std::chrono::seconds(1)); // 非关键操作移出锁
    std::lock_guard<std::mutex> lock(mtx);
    sharedData++;
}

// ===== 7. 条件变量使用 =====
std::condition_variable cv;
bool ready = false;

void consumer() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []{ return ready; });
    std::cout << "消费者: 收到数据!" << std::endl;
}

void producer() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_one();
}

// ===== 8. 自定义锁 =====
class MyLock {
private:
    std::atomic<bool> locked{false};
public:
    void lock() {
        while (locked.exchange(true, std::memory_order_acquire)) {
            std::this_thread::yield();
        }
    }

    void unlock() {
        locked.store(false, std::memory_order_release);
    }
};

MyLock myLock;
void useMyLock() {
    std::lock_guard<MyLock> lock(myLock);
    std::cout << "使用自定义锁" << std::endl;
}

int main() {
    std::cout << "=== 1. 基础互斥锁 ===" << std::endl;
    basicMutexDemo();

    std::cout << "\n=== 2. 递归互斥锁 ===" << std::endl;
    recursiveFunction(3);

    std::cout << "\n=== 3. 带超时的互斥锁 ===" << std::endl;
    timedLockDemo();

    std::cout << "\n=== 4. 读写锁 ===" << std::endl;
    ThreadSafeCounter counter;
    auto reader = [&counter]() {
        std::cout << "读取: " << counter.read() << std::endl;
    };
    auto writer = [&counter]() {
        counter.write(42);
    };
    std::thread t1(reader);
    std::thread t2(reader);
    std::thread t3(writer);
    t1.join();
    t2.join();
    t3.join();

    std::cout << "\n=== 5. 死锁示例 ===" << std::endl;
    std::cout << "警告: 此示例可能导致死锁，需手动终止程序" << std::endl;
    // deadlockDemo(); // 取消注释测试死锁

    std::cout << "\n=== 6. 锁粒度过大 ===" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    slowFunction();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "慢函数耗时: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    fastFunction();
    end = std::chrono::high_resolution_clock::now();
    std::cout << "快函数耗时: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    std::cout << "\n=== 7. 条件变量 ===" << std::endl;
    std::thread consumerThread(consumer);
    std::thread producerThread(producer);
    consumerThread.join();
    producerThread.join();

    std::cout << "\n=== 8. 自定义锁 ===" << std::endl;
    useMyLock();

    return 0;
}