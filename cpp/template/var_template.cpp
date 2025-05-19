#include <iostream>
#include <bitset>

// 基准情形
void print() { std::cout << "end\n"; }

// 递归展开
template<typename T, typename... Args>
void print(T head, Args... rest) {
    std::cout << head << " ";
    print(rest...);
}

template<typename... Args>
void print(Args... rest) {
    print(rest...);
}

int main() {
    print();                   // 输出: end
    print(42);                 // 输出: 42 end
    print(1, 3.14, "Hello");   // 输出: 1 3.14 Hello end
    print(1, "hello", std::bitset<16>(377), 42); //1 hello 0000000101111001 42 end
}