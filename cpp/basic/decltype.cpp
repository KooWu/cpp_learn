#include <iostream>
#include <vector>
#include <utility>
#include <string>

// 1. 模板函数返回值类型推导（C++11 风格）
template<typename T, typename U>
auto add(T t, U u) -> decltype(t + u) {
    return t + u;
}

// 2. 精确返回引用类型（结合 decltype(auto)）
template<typename Container, typename Index>
decltype(auto) access(Container& c, Index i) {
    return c[i];
}

// 3. 元编程：检查类型是否有特定成员函数
template<typename T>
struct has_toString {
private:
    template<typename U>
    static auto test(int) -> decltype(std::declval<U>().toString(), std::true_type{});
    
    template<typename>
    static std::false_type test(...);
    
public:
    static constexpr bool value = decltype(test<T>(0))::value;
};

// 4. 自定义容器，演示 decltype 在迭代器中的应用
template<typename T>
class MyVector {
private:
    T* data;
    size_t size;
    
public:
    using iterator = T*;
    
    iterator begin() { return data; }
    iterator end() { return data + size; }
    
    // 使用 decltype 推导 operator[] 的返回类型
    decltype(auto) operator[](size_t i) {
        return data[i];
    }
};

// 5. 演示 decltype 的类型推导规则
struct Data {
    int value;
};

void demonstrate_decltype_rules() {
    int x = 10;
    const int& cref = x;
    Data d{42};
    
    // 规则1：变量名直接推导
    decltype(x) a = 20;           // int
    decltype(cref) b = x;         // const int&
    
    // 规则2：表达式推导
    decltype(x + 1) c = 0;        // int（纯右值）
    decltype((x)) d_ref = x;      // int&（带括号的左值）
    
    // 规则3：成员访问推导
    decltype(d.value) val = 0;    // int
    decltype((d.value)) val_ref = d.value;  // int&（带括号的左值）
}

const int& getRef() {
    static int x;
    return x; 
}

int main() {
    // 测试 add 函数的类型推导
    auto sum1 = add(3, 4.5);      // double
    auto sum2 = add(std::string("hello"), " world");  // std::string
    
    std::cout << "Sum1: " << sum1 << ", type: " << typeid(sum1).name() << std::endl;
    std::cout << "Sum2: " << sum2 << ", type: " << typeid(sum2).name() << std::endl;
    
    // 测试 access 函数返回引用
    std::vector<int> vec = {1, 2, 3};
    access(vec, 0) = 10;          // 修改原容器元素
    std::cout << "vec[0]: " << vec[0] << std::endl;
    
    // 测试 has_toString 元函数
    struct A { std::string toString() const { return "A"; } };
    struct B {};
    
    std::cout << "A has toString: " << std::boolalpha << has_toString<A>::value << std::endl;
    std::cout << "B has toString: " << has_toString<B>::value << std::endl;
    
    // 测试自定义容器
    MyVector<int> myVec;
    myVec[0] = 100;               // 使用 decltype 推导的 operator[]
    std::cout << "myVec[0]: " << myVec[0] << std::endl;
    
    // 测试 decltype(auto) 的精确推导

    decltype(auto) ref = getRef();  // const int&
    // ref = 20;  // 错误：不能修改 const 引用
    
    return 0;
}