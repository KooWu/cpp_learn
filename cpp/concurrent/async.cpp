#include <iostream>
#include <future>
#include <thread>
#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <coroutine>
#include <fstream>
#include <string>
#include <stdexcept>
#include <chrono>

// ===== 1. 线程池实现 =====
class ThreadPool {
public:
    explicit ThreadPool(size_t threads) : stop(false) {
        for (size_t i = 0; i < threads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, [this] {
                            return this->stop || !this->tasks.empty();
                        });
                        if (this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread &worker : workers) {
            worker.join();
        }
    }

    template<class F, class... Args>
    auto enqueue(F &&f, Args &&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type> {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            if (stop)
                throw std::runtime_error("enqueue on stopped ThreadPool");
            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};


// ===== 3. 异步操作示例 =====
class AsyncDemo {
public:
    // 3.1 std::async 示例
    static void async_example() {
        std::cout << "\n=== std::async 示例 ===" << std::endl;
        auto future = std::async(std::launch::async, []() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return 42;
        });

        std::cout << "async结果: " << future.get() << std::endl;
    }

    // 3.2 packaged_task 示例
    static void packaged_task_example() {
        std::cout << "\n=== packaged_task 示例 ===" << std::endl;
        std::packaged_task<int(int, int)> task([](int a, int b) {
            return a + b;
        });

        std::future<int> future = task.get_future();
        std::thread(std::move(task), 3, 4).detach();

        std::cout << "packaged_task结果: " << future.get() << std::endl;
    }

    // 3.4 线程池示例
    static void thread_pool_example() {
        std::cout << "\n=== 线程池示例 ===" << std::endl;
        ThreadPool pool(2);
        auto future1 = pool.enqueue([]() {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return "任务1完成";
        });

        auto future2 = pool.enqueue([]() {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            return "任务2完成";
        });

        std::cout << future1.get() << std::endl;
        std::cout << future2.get() << std::endl;
    }

    // 3.5 异步IO示例
    static std::future<std::string> async_io_example(const std::string& filename) {
        std::cout << "\n=== 异步IO示例 ===" << std::endl;
        return std::async(std::launch::async, [filename]() {
            std::ifstream file(filename);
            if (!file.is_open())
                throw std::runtime_error("文件打开失败: " + filename);
            
            std::string content((std::istreambuf_iterator<char>(file)), 
                                 std::istreambuf_iterator<char>());
            return content;
        });
    }

    // 3.6 异常处理示例
    static void exception_example() {
        std::cout << "\n=== 异常处理示例 ===" << std::endl;
        auto future = std::async([]() {
            throw std::runtime_error("测试异常");
            return 0;
        });

        try {
            future.get();
        } catch (const std::exception& e) {
            std::cerr << "捕获异常: " << e.what() << std::endl;
        }
    }

    // 3.7 共享future示例
    static void shared_future_example() {
        std::cout << "\n=== 共享future示例 ===" << std::endl;
        std::promise<int> prom;
        auto shared_fut = prom.get_future().share();

        auto worker = [](std::shared_future<int> fut, int id) {
            try {
                int value = fut.get();
                std::cout << "Worker " << id << " got: " << value << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Worker " << id << " error: " << e.what() << std::endl;
            }
        };

        std::thread t1(worker, shared_fut, 1);
        std::thread t2(worker, shared_fut, 2);

        // 设置异常（取消注释以下行测试正常结果）
        // prom.set_value(42);
        prom.set_exception(std::make_exception_ptr(std::runtime_error("共享异常")));

        t1.join();
        t2.join();
    }
};

int main() {
    AsyncDemo::async_example();
    AsyncDemo::packaged_task_example();

    AsyncDemo::thread_pool_example();
    
    // 异步IO（需要创建测试文件）
    try {
        auto io_future = AsyncDemo::async_io_example("test.txt");
        std::cout << "文件内容: " << io_future.get() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    
    AsyncDemo::exception_example();
    AsyncDemo::shared_future_example();

    return 0;
}    