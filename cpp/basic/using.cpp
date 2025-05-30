#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <filesystem>  // C++17

namespace fs = std::filesystem;  // 1. 命名空间别名

// 2. 类型别名（替代typedef）
using IntVector = std::vector<int>;
using StringMap = std::map<std::string, int>;

// 3. 模板别名
template<typename T>
using Pair = std::pair<int, T>;

// 4. 函数指针别名
using PrintFunc = void(*)(const char*);
void printMessage(const char* msg) {
    std::cout << "Message: " << msg << std::endl;
}

// 5. 基类成员访问控制
class Base {
protected:
    int value;
    void protectedFunc() { std::cout << "Base::protectedFunc()" << std::endl; }
private:
    int secret;
};

class Derived : public Base {
public:
    using Base::value;        // 将protected成员变为public
    using Base::protectedFunc; // 将protected方法变为public
    // using Base::secret;    // 错误：无法访问private成员
};

// 6. 构造函数继承
class Parent {
public:
    Parent(int x) : data(x) {}
protected:
    int data;
};

class Child : public Parent {
public:
    using Parent::Parent;  // 继承Parent的构造函数
    
    // 可选：添加自定义构造函数
    Child(const std::string& s) : Parent(s.size()) {}
};

// 7. 重载函数引入
class MathBase {
public:
    void add(int a, int b) { std::cout << "int: " << a + b << std::endl; }
    void add(double a, double b) { std::cout << "double: " << a + b << std::endl; }
};

class MathExtended : public MathBase {
public:
    using MathBase::add;  // 引入基类的所有add()重载
    
    // 添加新的重载，不会隐藏基类方法
    void add(const std::string& a, const std::string& b) {
        std::cout << "string: " << a + b << std::endl;
    }
};

// 8. 命名空间成员引入
namespace Tools {
    void log(const char* msg) {
        std::cout << "[LOG] " << msg << std::endl;
    }
}

int main() {
    // 测试命名空间别名
    fs::path currentPath = fs::current_path();
    std::cout << "Current path: " << currentPath << std::endl;
    
    // 测试类型别名
    IntVector numbers = {1, 2, 3};
    StringMap ages = {{"Alice", 25}, {"Bob", 30}};
    
    // 测试模板别名
    Pair<std::string> namePair(1, "John");
    std::cout << "Pair: " << namePair.first << ", " << namePair.second << std::endl;
    
    // 测试函数指针别名
    PrintFunc printer = printMessage;
    printer("Hello, using!");
    
    // 测试基类成员访问
    Derived d;
    d.value = 42;            // 直接访问基类protected成员
    d.protectedFunc();       // 调用基类protected方法
    
    // 测试构造函数继承
    Child c1(100);           // 使用继承的构造函数
    Child c2("Hello");       // 使用自定义构造函数
    
    // 测试重载函数引入
    MathExtended math;
    math.add(1, 2);          // 调用基类的int版本
    math.add(1.5, 2.5);      // 调用基类的double版本
    math.add("Hello", "World"); // 调用派生类的string版本
    
    // 测试命名空间成员引入
    using Tools::log;
    log("This is a test");

    return 0;
}