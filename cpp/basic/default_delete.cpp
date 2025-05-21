#include <iostream>
class NonCopyable {
public:
    NonCopyable() = default;  // 默认构造函数

    // 删除拷贝构造函数和拷贝赋值操作符
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

    // 移动构造函数和移动赋值操作符保持可用
    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(NonCopyable&&) = default;
};

// 防止隐式类型转换的例子
class OnlyInt {
public:
    OnlyInt(int) {}  // 允许从 int 构造

    // 禁止从 double 构造，防止隐式类型转换
    OnlyInt(double) = delete;
};

//default：告诉编译器生成该成员函数的默认实现，适用于你希望保留默认行为但可能因为其他原因需要显式声明的情况。
//delete：明确地禁用某个成员函数，常用于：
//禁止拷贝或移动操作。
//防止不期望的隐式类型转换

int main() {
    NonCopyable nc1;
    // NonCopyable nc2 = nc1; // 错误：拷贝构造函数已被删除

    OnlyInt oi1(42);  // 正确
    // OnlyInt oi2(3.14); // 错误：从 double 构造已被删除

    return 0;
}