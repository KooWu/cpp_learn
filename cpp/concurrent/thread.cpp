#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <functional>

// ===== 1. 线程管理基础 =====
void threadFunction() {
    std::cout << "普通函数线程 ID: " << std::this_thread::get_id() << std::endl;
}

// ===== 2. 使用 Lambda 表达式 =====
void lambdaThread() {
    int x = 10;
    int y = 20;
    
    // 值捕获
    std::thread t1([x, y]() {
        std::cout << "Lambda1 值捕获: " << x + y << std::endl;
    });
    
    // 引用捕获
    std::thread t2([&x, &y]() {
        x += 5;
        y += 5;
        std::cout << "Lambda2 引用捕获: " << x + y << std::endl;
    });
    
    t1.join();
    t2.join();
}

// ===== 3. 使用普通函数带参数 =====
void printSum(int a, int b) {
    std::cout << "普通函数参数和: " << a + b << std::endl;
}

void functionThread() {
    std::thread t(printSum, 3, 4);
    t.join();
}

// ===== 4. 使用类成员函数 =====
class Calculator {
public:
    void add(int a, int b) {
        std::cout << "Calculator::add: " << a + b << std::endl;
    }
    
    static void multiply(int a, int b) {
        std::cout << "Calculator::multiply: " << a * b << std::endl;
    }
};

void memberFunctionThread() {
    Calculator calc;
    
    // 非静态成员函数 - 对象指针
    std::thread t1(&Calculator::add, &calc, 5, 6);
    
    // 非静态成员函数 - 引用
    std::thread t2(&Calculator::add, std::ref(calc), 7, 8);
    
    // 静态成员函数
    std::thread t3(&Calculator::multiply, 3, 4);
    
    t1.join();
    t2.join();
    t3.join();
}

// ===== 5. 使用函数对象 =====
class Adder {
public:
    void operator()(int a, int b) const {
        std::cout << "函数对象加法: " << a + b << std::endl;
    }
};

void functorThread() {
    Adder adder;
    std::thread t(adder, 10, 20);
    t.join();
}

// ===== 6. 使用 std::bind =====
void printProduct(int a, int b) {
    std::cout << "绑定函数乘积: " << a * b << std::endl;
}

void bindThread() {
    auto boundFunc = std::bind(printProduct, std::placeholders::_1, 5);
    std::thread t(boundFunc, 3);
    t.join();
}

// ===== 7. 传递参数的注意事项 =====
void takeString(std::string str) {
    std::cout << "线程接收字符串: " << str << std::endl;
}

void parameterPassing() {
    std::string msg = "Hello";
    
    // 拷贝传递
    std::thread t1(takeString, msg);
    
    // 移动传递
    std::thread t2(takeString, std::move(msg));
    
    t1.join();
    t2.join();
}

// ===== 8. 线程异常安全 =====
class ThreadGuard {
private:
    std::thread& t;
public:
    explicit ThreadGuard(std::thread& t) : t(t) {}
    ~ThreadGuard() {
        if (t.joinable()) t.join();
    }
    ThreadGuard(const ThreadGuard&) = delete;
    ThreadGuard& operator=(const ThreadGuard&) = delete;
};

void safeThread() {
    std::thread t([]{
        std::cout << "安全线程执行中" << std::endl;
    });
    
    ThreadGuard guard(t);
    // 自动管理线程生命周期
}

// ===== 9. C++20 的 std::jthread =====
#include <stop_token>

void jthreadExample() {
    std::jthread t([](std::stop_token token) {
        int count = 0;
        while (!token.stop_requested() && count < 3) {
            std::cout << "jthread 计数: " << count++ << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
    
    // 无需手动join，自动管理
}

int main() {
    std::cout << "=== 1. 线程管理基础 ===" << std::endl;
    std::thread t(threadFunction);
    t.join();
    
    std::cout << "\n=== 2. Lambda 表达式 ===" << std::endl;
    lambdaThread();
    
    std::cout << "\n=== 3. 普通函数带参数 ===" << std::endl;
    functionThread();
    
    std::cout << "\n=== 4. 类成员函数 ===" << std::endl;
    memberFunctionThread();
    
    std::cout << "\n=== 5. 函数对象 ===" << std::endl;
    functorThread();
    
    std::cout << "\n=== 6. std::bind ===" << std::endl;
    bindThread();
    
    std::cout << "\n=== 7. 参数传递 ===" << std::endl;
    parameterPassing();
    
    std::cout << "\n=== 8. 异常安全 ===" << std::endl;
    safeThread();
    
    std::cout << "\n=== 9. std::jthread ===" << std::endl;
    jthreadExample();
    
    return 0;
}    