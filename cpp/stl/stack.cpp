#include <iostream>
#include <stack>
#include <vector>
#include <string>

// 1. 自定义类型：用于栈存储
struct Point {
    int x, y;
    
    // 构造函数
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    
    // 重载输出运算符，方便打印
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << "(" << p.x << ", " << p.y << ")";
    }
};

int main() {
    // === 基本用法示例 ===
    std::cout << "=== 基本用法 ===\n";
    std::stack<int> intStack;

    // 入栈操作
    intStack.push(10);
    intStack.push(20);
    intStack.emplace(30);  // 直接构造元素

    // 访问栈顶元素
    std::cout << "栈顶元素: " << intStack.top() << "\n";  // 输出: 30

    // 出栈操作
    intStack.pop();
    std::cout << "弹出后栈顶: " << intStack.top() << "\n";  // 输出: 20

    // 检查栈状态
    std::cout << "栈大小: " << intStack.size() << "\n";  // 输出: 2
    std::cout << "是否为空: " << (intStack.empty() ? "是" : "否") << "\n\n";


    // === 使用自定义类型 ===
    std::cout << "=== 使用自定义类型 ===\n";
    std::stack<Point> pointStack;

    // 入栈自定义类型
    pointStack.push({1, 2});
    pointStack.emplace(3, 4);  // 直接构造对象

    // 访问栈顶
    std::cout << "栈顶元素: " << pointStack.top() << "\n";  // 输出: (3, 4)

    // 遍历栈（需弹出元素）
    std::cout << "遍历栈: ";
    while (!pointStack.empty()) {
        std::cout << pointStack.top() << " ";
        pointStack.pop();
    }
    std::cout << "\n\n";


    // === 使用 vector 作为底层容器 ===
    std::cout << "=== 使用 vector 作为底层容器 ===\n";
    std::stack<double, std::vector<double>> vecStack;

    // 入栈操作
    vecStack.push(1.1);
    vecStack.push(2.2);
    vecStack.push(3.3);

    // 访问栈顶
    std::cout << "栈顶元素: " << vecStack.top() << "\n";  // 输出: 3.3

    // 清空栈
    while (!vecStack.empty()) {
        vecStack.pop();
    }
    std::cout << "清空后栈大小: " << vecStack.size() << "\n\n";


    // === 括号匹配示例 ===
    std::cout << "=== 括号匹配示例 ===\n";
    std::string expr = "{[()]}";
    std::stack<char> brackets;
    bool valid = true;

    for (char c : expr) {
        if (c == '{' || c == '[' || c == '(') {
            brackets.push(c);  // 左括号入栈
        } else {
            if (brackets.empty()) {
                valid = false;
                break;
            }
            char top = brackets.top();
            brackets.pop();
            // 检查括号匹配
            if ((c == '}' && top != '{') ||
                (c == ']' && top != '[') ||
                (c == ')' && top != '(')) {
                valid = false;
                break;
            }
        }
    }

    std::cout << "括号匹配结果: " << (valid ? "合法" : "非法") << "\n\n";


    // === 逆波兰表达式求值 ===
    std::cout << "=== 逆波兰表达式求值 ===\n";
    // 表达式: (3 + 4) * 2 → 后缀表达式: 3 4 + 2 *
    std::vector<std::string> tokens = {"3", "4", "+", "2", "*"};
    std::stack<int> evalStack;

    for (const auto& token : tokens) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            // 操作符：弹出两个操作数，计算后压栈
            int b = evalStack.top(); evalStack.pop();
            int a = evalStack.top(); evalStack.pop();
            if (token == "+") evalStack.push(a + b);
            if (token == "-") evalStack.push(a - b);
            if (token == "*") evalStack.push(a * b);
            if (token == "/") evalStack.push(a / b);
        } else {
            // 操作数：直接压栈
            evalStack.push(std::stoi(token));
        }
    }

    std::cout << "表达式结果: " << evalStack.top() << "\n\n";


    // === 模拟递归调用 ===
    std::cout << "=== 模拟递归调用 ===\n";
    // 模拟计算阶乘 n! 的递归调用栈
    int n = 5;
    std::stack<int> callStack;
    
    // 压入初始参数
    callStack.push(n);
    int result = 1;

    while (!callStack.empty()) {
        int current = callStack.top();
        callStack.pop();
        
        if (current == 0) {
            // 基线条件：0! = 1
            result *= 1;
        } else {
            // 递归条件：n! = n * (n-1)!
            result *= current;
            callStack.push(current - 1);
        }
    }

    std::cout << "5! = " << result << "\n";

    return 0;
}