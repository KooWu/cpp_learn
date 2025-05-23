#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <functional>

struct Person {
    std::string name;
    int age;
    
    Person(const std::string& n, int a) : name(n), age(a) {}
};

int main() {
    // 1. 简单lambda示例
    auto add = [](int a, int b) { return a + b; };
    std::cout << "3 + 4 = " << add(3, 4) << std::endl;
    
    // 2. 使用lambda排序自定义类型
    std::vector<Person> people = {
        {"Alice", 25},
        {"Bob", 20},
        {"Charlie", 30}
    };
    
    // 按年龄升序排序
    std::sort(people.begin(), people.end(), 
              [](const Person& a, const Person& b) { return a.age < b.age; });
    
    std::cout << "Sorted by age:" << std::endl;
    for (const auto& p : people) {
        std::cout << p.name << " (" << p.age << ")" << std::endl;
    }
    
    // 3. 使用lambda查找元素
    auto it = std::find_if(people.begin(), people.end(),
                           [](const Person& p) { return p.name == "Bob"; });
    
    if (it != people.end()) {
        std::cout << "Found Bob, age: " << it->age << std::endl;
    }
    
    // 4. 使用捕获列表修改外部变量
    int totalAge = 0;
    std::for_each(people.begin(), people.end(),
                 [&totalAge](const Person& p) { totalAge += p.age; });
    
    std::cout << "Total age: " << totalAge << std::endl;
    
    // 5. 泛型lambda（C++14及以后）
    auto printTwice = [](const auto& value) {
        std::cout << value << " " << value << std::endl;
    };
    
    printTwice(42);           // 整数
    printTwice("hello");      // 字符串
    
    // 6. 存储lambda到std::function
    std::function<int(int, int)> multiply = [](int a, int b) { return a * b; };
    std::cout << "5 * 6 = " << multiply(5, 6) << std::endl;

    int x = 10;
    auto lambda = [x]() mutable { x++; return x; };
    std::cout << lambda() << std::endl;  // 输出11
    std::cout << x << std::endl;
    
    return 0;
}