
// 统一使用using，不要使用typedef
// 主要用于为非模板类型创建别名，不能直接用于模板，using可以适用于任何类型
// 宏也可以，但是有如下问题
// 类型安全性：宏只是简单的文本替换，不会检查语法或类型正确性。
// 作用域问题：宏的作用范围比 typedef 或 using 更广，可能导致意外的名称冲突。
// 调试困难：由于宏在预处理阶段被替换，调试器中看到的是替换后的代码，增加了调试难度
#include <iostream>
#include <vector>

using namespace std;

// 第一种使用
// type alias
// 等价于typedef void(*func)(int, int);
using fun=void (*)(int, int);

void example(int, int) {}

// 第二种使用
template<typename T>
struct Container {
    using value_type=T;  // 等价于typedef T value_type;
};

template<typename Cn>
void func2(const Cn &cn) {
    typename Cn::value_type n;
}

// 第三种使用
// alias template
template<typename CharT>
using string=basic_string<CharT, char_traits<CharT>>;
// 等价于 typedef basic_string<char>    string;

// 综上:type alias等价于typedef,没有什么不同
// using 的所有用法拓展如下:
// 1.using-directives
// using namespace std; or using std::cout;
// 2. using-declaration
// protected:
//    using _Base::_M_alloc;
// 3.c++2.0特性 type alias and alias template
int main() {
    fun fn = example;   // 函数指针
    return 0;
}