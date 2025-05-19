#include <type_traits>
#include <utility>
#include <iostream>

// 默认情况下，假定没有 foo 方法
//这个“无名”的模板参数只是为了在模板特化时提供一个钩子（hook），使得我们可以有条件地启用某些特化版本
template<typename T, typename = void>
struct has_foo_method : std::false_type {};

// 如果 T 有 foo 方法，并且 foo 方法返回 int 类型，则特化此模板
//std::is_same 是一个结构体模板，定义于 <type_traits> 头文件中。它用来检查两个类型是否相同，并提供一个名为 value 的静态成员常量，其值为 true 或 false。
//decltype(std::declval<T>().foo(), void())这是逗号运算，先判断是否foo方法，然后编程void
template<typename T>
struct has_foo_method<T, decltype(std::declval<T>().foo(), void())>
    : std::is_same<decltype(std::declval<T>().foo()), int> {};

// 只有当 T 有返回 int 的 foo 方法时才启用该函数
template<typename T>
typename std::enable_if<has_foo_method<T>::value, int>::type
call_foo(T& obj) {
    static_assert(has_foo_method<T>::value, "T must have a foo() method returning int");
    return obj.foo();
}

struct A {
    int foo() { return 42; }
};

struct B {
    double foo() { return 3.14; }
};

int main() {
    A a;
    static_assert(has_foo_method<A>::value, "A should have foo method");
    std::cout << call_foo(a) << "\n"; // 正确，输出 42

    B b;
    // 下面这行会编译失败，因为 B 不满足 enable_if 条件
    // std::cout << call_foo(b) << "\n";
}