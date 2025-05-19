#include <iostream>
#include <bitset>

template<typename... Args>
void print(Args&&... args) {
    // 处理非空参数情况
    if constexpr(sizeof...(args) > 0) {
        // 使用表达式折叠
        ((std::cout << std::forward<Args>(args) << " "), ...);
    }
    std::cout << "end\n";
}

int main() {
    print();                   // 输出: end
    print(42);                 // 输出: 42 end
    print(1, 3.14, "Hello");   // 输出: 1 3.14 Hello end
    print(1, "hello", std::bitset<16>(377), 42); //1 hello 0000000101111001 42 end
}