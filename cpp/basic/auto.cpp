#include <iostream>
#include <list>
#include <algorithm>
#include <vector>
#include <functional>
#include <iterator>

using namespace std;

// iterator  stl_iterator.h里面的源码 使用auto
template<typename _Iterator>
#if __cplusplus < 201103L
inline typename reverse_iterator<_Iterator>::difference_type
    operator-(const reverse_iterator<_Iterator>& __x,
          const reverse_iterator<_Iterator>& __y)
#else
inline auto
operator-(const reverse_iterator<_Iterator> &__x,
          const reverse_iterator<_Iterator> &__y)
-> decltype(__x.base() - __y.base())
#endif
{ return __y.base() - __x.base(); }


// auto 关键字(c++11)  语法糖
// for 循环  ranged-base for
//直接初始化：当使用 auto 进行变量声明时，编译器会分析初始化表达式，并尝试推导出最适合的类型。
//引用和常量：auto 不会自动推导出引用或常量类型。如果你需要保留这些特性，可以使用 auto& 或 const auto 等形式。
//数组和函数指针：对于数组和函数类型的初始化表达式，auto 会退化为相应的指针类型（如同函数参数传递时的行为）
int main() {

    auto i = 42;  // 编译器具备实参推导

    auto ll1 = [](int x) -> int {
        return x + 10;
    };
    // lambda 匿名函数
    function<int(int x)> ll = [](int x) -> int {
        return x + 10;
    };

    cout << ll(10) << endl;
    list<int> l{1, 2, 3};
    list<int>::iterator iterator;
    iterator = find(l.begin(), l.end(), 10);

    auto ite = find(l.begin(), l.end(), 11);    // auto 关键字

    vector<int> vec;
    //=====================range-based for statement=========================
    // (1)
    for (auto elem:vec) // pass  by value
        cout << elem << endl;
    // c++2.0 之前 (2)   编译器会把(1)转换为(2)或(3)
    for (auto _pos = l.begin(), _end = l.end(); _pos != _end; ++_pos)
        cout << *_pos << " ";
    cout << endl;
    // (3)
    // c++2.0之后 全局函数begin(container)与end(container) 可以接受容器
    for (auto _pos = begin(l), _end = end(l); _pos != _end; ++_pos)
        cout << *_pos << " ";
    cout << endl;

    // (4)
    for (auto &elem:vec) // pass by reference
        elem *= 3;

    // (5) 编译器会把(4)转换为(5)
    for (auto _pos = l.begin(), _end = l.end(); _pos != _end; ++_pos) {
        auto &elem = *_pos;
        elem *= 3;
    }
    cout << endl;
    // auto 不利：降低代码可读性、可能得不到你预想的类型、配合decltype有意想不到的结果
    // auto可能得不到你预想的类型，如vector<bool>[]的返回类型。
    // std::vector<bool> 是一个特化的模板类，它并不是真正存储 bool 类型的数组
    vector<bool> v(true);
    auto var = v.at(0);
    cout<< typeid(var).name()<<endl;

    vector<bool> v3(10);
    bool var3 = v3.at(0);
    cout<< typeid(var3).name()<<endl;

    vector<int> v1(10);
    auto var1 = v1.at(0);
    cout<< typeid(var1).name()<<endl;


    vector<int> v2(10);
    int var2 = v2.at(0);
    cout<< typeid(var2).name()<<endl;
    return 0;
}