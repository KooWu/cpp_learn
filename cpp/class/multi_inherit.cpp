#include <iostream>
#include <cstdint>

// 基类Base1
class Base1 {
public:
    virtual void func1() { std::cout << "Base1::func1()" << std::endl; }
    virtual void func2() { std::cout << "Base1::func2()" << std::endl; }
    int data1;
};

// 基类Base2
class Base2 {
public:
    virtual void func3() { std::cout << "Base2::func3()" << std::endl; }
    virtual void func4() { std::cout << "Base2::func4()" << std::endl; }
    int data2;
};

// 派生类Derived（多继承）
class Derived : public Base1, public Base2 {
public:
    void func1() override { std::cout << "Derived::func1()" << std::endl; }
    void func3() override { std::cout << "Derived::func3()" << std::endl; }
    virtual void func5() { std::cout << "Derived::func5()" << std::endl; }
    int data3;
};

// +----------------+
// | vptr1          | --> Derived的Base1虚表
// +----------------+
// | data1          |
// +----------------+
// | vptr2          | --> Derived的Base2虚表
// +----------------+
// | data2          |
// +----------------+
// | data3          |
// +----------------+

// Derived的Base1虚表:
// +----------------+
// | Derived::func1 |
// +----------------+
// | Base1::func2   |
// +----------------+
// | Derived::func5 |  // 派生类新增的虚函数
// +----------------+

// Derived的Base2虚表:
// +----------------+
// | Derived::func3 |
// +----------------+
// | Base2::func4   |
// +----------------+

int main() {
    // 创建Derived对象
    Derived d;
    d.data1 = 10;
    d.data2 = 20;
    d.data3 = 30;
    
    // 通过不同基类指针调用虚函数
    // Base1* ptr1 = &d; → ptr1 指向对象起始地址（&d）。
    // Base2* ptr2 = &d; → ptr2 指向对象起始地址 + sizeof(Base1)。
    Base1* ptr1 = &d;
    Base2* ptr2 = &d;
    
    std::cout << "ptr1地址: " << ptr1 << std::endl;
    std::cout << "ptr2地址: " << ptr2 << std::endl;
    
    ptr1->func1();  // 输出: Derived::func1()
    ptr2->func3();  // 输出: Derived::func3()

    ptr1->func2();  // 输出: Base1::func1()
    ptr2->func4();  // 输出: Base2::func3()
    
    // 内存布局分析
    std::cout << "\n=== 内存布局 ===" << std::endl;
    std::cout << "对象地址: " << &d << std::endl;
    
    // Base1子对象
    std::cout << "\nBase1子对象:" << std::endl;
    uintptr_t* vptr1 = reinterpret_cast<uintptr_t*>(&d);
    std::cout << "vptr1地址: " << vptr1 << std::endl;
    std::cout << "vptr1值: " << *vptr1 << std::endl;
    
    // Base2子对象
    std::cout << "\nBase2子对象:" << std::endl;
    uintptr_t* vptr2 = reinterpret_cast<uintptr_t*>(reinterpret_cast<char*>(&d) + sizeof(Base1));
    std::cout << "vptr2地址: " << vptr2 << std::endl;
    std::cout << "vptr2值: " << *vptr2 << std::endl;
    
    return 0;
}