#include <iostream>
#include <typeinfo>

class Base {
public:
    virtual void print() { std::cout << "Base" << std::endl; }
};

class Derived : public Base {
public:
    void print() override { std::cout << "Derived" << std::endl; }
};

class A { virtual void f() {} };
class B : public A {};
class C : public A {};
class D : public B, public C {};

class NonVirtualBase {
    // 无虚函数
};

class NonVirtualDerived : public NonVirtualBase {
    // 无虚函数
};

int main() {
    Base* ptr = new Derived();
    
    // 虚函数动态绑定
    ptr->print();  // 输出 "Derived"
    
    // RTTI: typeid
    std::cout << typeid(*ptr).name() << std::endl;  // 输出 "Derived"
    std::cout << typeid(ptr).name() << std::endl;  // 输出 "base"
    
    // RTTI: dynamic_cast
    Derived* derived_ptr = dynamic_cast<Derived*>(ptr);
    if (derived_ptr) {
        std::cout << "Dynamic cast successful!" << std::endl;
    }
    
    delete ptr;

    Derived d1;
    Base& ref = d1;
    std::cout << typeid(ref).name() << std::endl; // Derived

    D* d = new D();
    B* b = d;           // B 子对象指针（偏移量 0）
    C* c = dynamic_cast<C*>(b);  // 正确调整指针偏移（偏移量 sizeof(B)）
    NonVirtualBase* ptr1 = new NonVirtualDerived();
    
    // 输出 "NonVirtualBase"（静态类型），而非实际类型
    std::cout << typeid(*ptr1).name() << std::endl; // NonVirtualBase


    NonVirtualDerived d2;
    NonVirtualBase& ref2 = d2;
    std::cout << typeid(ref2).name() << std::endl; // NonVirtualBase
    return 0;
}