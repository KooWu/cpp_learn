#include <iostream>
#include <cstdint>

// 基类Base
class Base {
public:
    virtual void func1() { std::cout << "Base::func1()" << std::endl; }
    virtual void func2() { std::cout << "Base::func2()" << std::endl; }
    int baseData;
};

// 派生类Derived
class Derived : public Base {
public:
    void func1() override { std::cout << "Derived::func1()" << std::endl; }
    virtual void func3() { std::cout << "Derived::func3()" << std::endl; }
    int derivedData;
};

// +----------------+
// | vptr           | --> Derived的虚表
// +----------------+
// | baseData       |
// +----------------+
// | derivedData    |
// +----------------+

// Derived的虚表:
// +----------------+
// | Derived::func1 |  // 重写的函数
// +----------------+
// | Base::func2    |  // 未重写的函数
// +----------------+
// | Derived::func3 |  // 派生类新增的虚函数
// +----------------+

int main() {
    // 创建Derived对象
    Derived d;
    d.baseData = 10;
    d.derivedData = 20;
    
    // 通过基类指针调用虚函数
    Base* ptr = &d;
    ptr->func1();  // 输出: Derived::func1()
    
    // 内存布局分析
    std::cout << "\n=== 内存布局 ===" << std::endl;
    std::cout << "对象地址: " << &d << std::endl;
    std::cout << "vptr地址: " << &d << std::endl;
    
    // 注意: 以下代码依赖特定编译器实现，非标准行为
    // 获取vptr的值（虚表地址）
    uintptr_t* vptr = reinterpret_cast<uintptr_t*>(&d);
    std::cout << "vptr值(虚表地址): " << *vptr << std::endl;
    
    // 获取虚表中前两个函数的地址
    uintptr_t* vtable = reinterpret_cast<uintptr_t*>(*vptr);
    std::cout << "虚表[0](func1地址): " << vtable[0] << std::endl;
    std::cout << "虚表[1](func2地址): " << vtable[1] << std::endl;
    std::cout << "虚表[2](func3地址): " << vtable[2] << std::endl;
    
    // 手动调用虚函数（仅作演示，实际编程中不要这样做）
    using FuncPtr = void(*)();
    FuncPtr func = reinterpret_cast<FuncPtr>(vtable[0]);
    func();  // 调用Derived::func1()
    
    return 0;
}