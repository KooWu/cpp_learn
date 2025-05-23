#include <iostream>

class Base {
public:
    virtual void func() { /* ... */ }
};

class Derived : public Base {
public:
    void func() override final { /* ... */ }  // 重写基类函数，并禁止后续派生类重写
};

class GrandDerived : public Derived {
public:
    // void func() override { /* ... */ }  // 错误：尝试重写 final 函数
};

int main() {
    Derived test;
    return 0;
}