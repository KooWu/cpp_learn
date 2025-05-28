#include <cassert>

int divide(int a, int b) {
    assert(b != 0 && "Division by zero!");  // 检查除数非零
    return a / b;
}

int main() {
    int result = divide(10, 0);  // 触发断言失败
    return 0;
}