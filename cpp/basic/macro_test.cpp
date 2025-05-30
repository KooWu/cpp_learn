#include <iostream>

// 常量宏
#define PI 3.14159
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// 调试宏
#ifdef DEBUG
    #define LOG(msg) std::cout << "[DEBUG] " << msg << std::endl
#else
    #define LOG(msg)
#endif

// 字符串化和连接
#define STR(x) #x
#define CONCAT(a, b) a##b

int main() {
    // 使用常量宏
    double radius = 5.0;
    std::cout << "圆面积: " << PI * radius * radius << std::endl;

    // 使用函数式宏
    std::cout << "最大值: " << MAX(10, 20) << std::endl;

    // 调试日志
    LOG("计算完成");

    // 字符串化和连接
    std::cout << "字符串化: " << STR(hello world) << std::endl;
    int num1 = 10, num2 = 20;
    int num1num2 = 1020; 
    std::cout << "连接: " << CONCAT(num1, num2) << std::endl;

    // 预定义宏
    std::cout << "文件: " << __FILE__ << ", 行: " << __LINE__ << std::endl;

    return 0;
}