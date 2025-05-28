#include <iostream>
#include <string>

// 1. 基本用法：区分同名变量
class Point {
private:
    int x, y;
public:
    // 使用this区分成员变量和参数
    void set(int x, int y) {
        this->x = x;  // this->x 是成员变量，x 是参数
        this->y = y;
    }

    // 返回当前对象的引用（链式调用）
    Point& scale(int factor) {
        this->x *= factor;
        this->y *= factor;
        return *this;
    }

    // 打印对象地址
    void printAddress() const {
        std::cout << "Point address: " << this << std::endl;
    }
};

// 2. this在常量成员函数中的类型
class Data {
private:
    int value;
public:
    Data(int v) : value(v) {}

    // this类型为 const Data* const
    int getValue() const {
        // *this 是 const 对象
        // this->value = 10;  // 错误：不能修改const对象
        return this->value;
    }

    // 非const版本
    int& getValue() {
        return this->value;
    }
};

// 3. 空指针调用成员函数
class Empty {
public:
    void safe() const {
        std::cout << "Safe call" << std::endl;
    }

    void unsafe() const {
        // std::cout << data << std::endl;  // 错误：访问空指针的成员
    }

    int data;
};

// 4. this与继承
class Base {
public:
    void printSize() const {
        std::cout << "Base size: " << sizeof(*this) << ", "<< this<< std::endl;
    }

    virtual void whoami() const {
        std::cout << "Base" << std::endl;
    }
};

class Derived : public Base {
private:
    int data[10];  // 增加类的大小
public:
    void whoami() const override {
        std::cout << "Derived:" << sizeof(*this) << ", "<< this<< std::endl;
    }
};

// 5. 多继承中的this指针调整
class A {
public:
     void printA() const { std::cout << "A: " << this << std::endl; }
private:
    int a;
};

class B {
public:
     void printB() const { std::cout << "B: " << this << std::endl; }
private:
int b;
};

class C : public A, public B {
public:
     void printC() const { std::cout << "C: " << this << std::endl; }
private:
int c;
};

// 6. 返回this的引用实现链式调用
class StringBuilder {
private:
    std::string content;
public:
    StringBuilder& append(const std::string& text) {
        content += text;
        return *this;
    }

    StringBuilder& addSpace() {
        content += " ";
        return *this;
    }

    std::string toString() const {
        return content;
    }
};

int main() {
    // 测试1：基本用法
    Point p;
    p.set(3, 4);
    p.scale(2).scale(3);  // 链式调用
    std::cout << "Point address in main: " << &p << std::endl;
    p.printAddress();

    // 测试2：const与非const的this
    Data d(10);
    const Data cd(20);
    d.getValue() = 15;    // 调用非const版本
    // cd.getValue() = 25;  // 错误：const对象只能调用const版本

    // 测试3：空指针调用
    Empty* ptr = nullptr;
    ptr->safe();  // 安全：不访问成员
    // ptr->unsafe();  // 未定义行为：访问成员

    // 测试4：继承中的this
    Derived derived;
    Base* basePtr = &derived;
    basePtr->printSize();  // 输出Derived的大小
    basePtr->whoami();     // 动态绑定：输出Derived

    // 测试5：多继承中的指针调整
    C c;
    std::cout << "\n多继承中的指针调整:" << std::endl;
    c.printC();    // C的起始地址
    c.printA();    // 与C相同（第一个基类）
    c.printB();    // 与C不同（偏移后的地址）

    // 测试6：链式调用
    std::cout << "\n链式调用结果:" << std::endl;
    std::cout << StringBuilder()
        .append("Hello")
        .addSpace()
        .append("World")
        .toString() << std::endl;

    return 0;
}