#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <vector>
#include <string>

// ===== 1. 基础类定义 =====
class Shape {
public:
    virtual void draw() const = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
public:
    void draw() const override { std::cout << "Drawing Circle" << std::endl; }
    ~Circle() { std::cout << "Circle destroyed" << std::endl; }
};

class Rectangle : public Shape {
public:
    void draw() const override { std::cout << "Drawing Rectangle" << std::endl; }
    ~Rectangle() { std::cout << "Rectangle destroyed" << std::endl; }
};

// ===== 2. std::unique_ptr 示例 =====
void uniquePtrDemo() {
    std::cout << "\n=== std::unique_ptr Demo ===" << std::endl;
    
    // 创建方式1：直接构造
    std::unique_ptr<Circle> circle1(new Circle());
    
    // 创建方式2：推荐使用make_unique (C++14)
    auto circle2 = std::make_unique<Circle>();
    
    // 独占特性：无法复制
    // std::unique_ptr<Circle> circle3 = circle1; // 编译错误
    
    // 只能移动
    std::unique_ptr<Circle> circle3 = std::move(circle1);
    if (!circle1) {
        std::cout << "circle1 is null after move" << std::endl;
    }
    
    // 作为容器元素
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>());
    shapes.push_back(std::make_unique<Rectangle>());
    
    // 多态调用
    for (const auto& shape : shapes) {
        shape->draw();
    }
}

// ===== 3. std::shared_ptr 示例 =====
void sharedPtrDemo() {
    std::cout << "\n=== std::shared_ptr Demo ===" << std::endl;
    
    // 创建方式1：直接构造
    std::shared_ptr<Circle> shared1(new Circle());
    
    // 创建方式2：推荐使用make_shared
    auto shared2 = std::make_shared<Circle>();
    
    // 共享特性：可以复制
    std::shared_ptr<Circle> shared3 = shared2;
    std::cout << "shared2 use count: " << shared2.use_count() << std::endl; // 输出2
    
    // 作为容器元素
    std::vector<std::shared_ptr<Shape>> sharedShapes;
    sharedShapes.push_back(std::make_shared<Circle>());
    sharedShapes.push_back(std::make_shared<Rectangle>());
    
    // 循环引用问题（内存泄漏）
    class Parent;
    class Child {
    public:
        std::shared_ptr<Parent> parent; // 注意：这里应该用weak_ptr避免循环
        ~Child() { std::cout << "Child destroyed" << std::endl; }
    };
    
    class Parent {
    public:
        std::shared_ptr<Child> child;
        ~Parent() { std::cout << "Parent destroyed" << std::endl; }
    };
    
    auto parent = std::make_shared<Parent>();
    auto child = std::make_shared<Child>();
    parent->child = child;
    child->parent = parent; // 循环引用：两者引用计数始终为2
    // 离开作用域时，parent和child都不会被销毁（内存泄漏）
}

// ===== 4. std::weak_ptr 示例 =====
void weakPtrDemo() {
    std::cout << "\n=== std::weak_ptr Demo ===" << std::endl;
    
    auto shared = std::make_shared<int>(42);
    std::weak_ptr<int> weak = shared;
    
    // 检查对象是否存活
    if (auto locked = weak.lock()) {
        std::cout << "Weak pointer locked: " << *locked << std::endl;
    }
    
    // 手动释放shared
    shared.reset();
    
    // 再次检查
    if (weak.expired()) {
        std::cout << "Weak pointer expired" << std::endl;
    }
    
    // 修复循环引用问题
    class SafeParent;
    class SafeChild {
    public:
        std::weak_ptr<SafeParent> parent; // 使用weak_ptr打破循环
        ~SafeChild() { std::cout << "SafeChild destroyed" << std::endl; }
    };
    
    class SafeParent {
    public:
        std::shared_ptr<SafeChild> child;
        ~SafeParent() { std::cout << "SafeParent destroyed" << std::endl; }
    };
    
    auto safeParent = std::make_shared<SafeParent>();
    auto safeChild = std::make_shared<SafeChild>();
    safeParent->child = safeChild;
    safeChild->parent = safeParent; // 正确：SafeChild::parent是弱引用
    // 离开作用域时，safeParent和safeChild都会被正确销毁
}

// ===== 5. 自定义删除器示例 =====
void customDeleterDemo() {
    std::cout << "\n=== Custom Deleter Demo ===" << std::endl;
    
    // 自定义删除器（lambda形式）
    auto fileCloser = [](FILE* file) {
        if (file) {
            fclose(file);
            std::cout << "File closed" << std::endl;
        }
    };
    
    // 使用自定义删除器的unique_ptr
    std::unique_ptr<FILE, decltype(fileCloser)> file(fopen("test.txt", "w"), fileCloser);
    if (file) {
        fputs("Hello, World!", file.get());
    }
    
    // 自定义删除器（结构体形式）
    struct ArrayDeleter {
        void operator()(int* arr) const {
            delete[] arr;
            std::cout << "Array deleted" << std::endl;
        }
    };
    
    std::unique_ptr<int, ArrayDeleter> array(new int[10]);
}

// ===== 6. 多线程安全示例 =====
void threadSafetyDemo() {
    std::cout << "\n=== Thread Safety Demo ===" << std::endl;
    
    std::shared_ptr<int> sharedData = std::make_shared<int>(0);
    std::mutex mtx;
    
    auto worker = [&]() {
        for (int i = 0; i < 1000; ++i) {
            // 复制shared_ptr是线程安全的（引用计数原子操作）
            auto local = sharedData;
            
            // 但修改对象需要同步
            std::lock_guard<std::mutex> lock(mtx);
            (*local)++;
        }
    };
    
    std::thread t1(worker);
    std::thread t2(worker);
    
    t1.join();
    t2.join();
    
    std::cout << "Final value: " << *sharedData << std::endl; // 输出2000
}

int main() {
    uniquePtrDemo();
    sharedPtrDemo();
    weakPtrDemo();
    customDeleterDemo();
    threadSafetyDemo();
    return 0;
}    