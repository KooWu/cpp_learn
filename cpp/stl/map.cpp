#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <functional>
#include <stdexcept>

// 辅助函数：打印map信息
template<typename K, typename V, typename Compare = std::less<K>>
void print_map(const std::map<K, V, Compare>& m, const std::string& name = "") {
    if (!name.empty()) std::cout << name << ":\n";
    for (const auto& pair : m) {
        std::cout << "  [" << pair.first << "] = " << pair.second << "\n";
    }
    std::cout << std::endl;
}

int main() {
    // ===== 1. 初始化方式 =====
    std::cout << "\n=== 1. 初始化方式 ===\n";
    
    // 默认构造
    std::map<std::string, int> ages1;
    
    // 初始化列表
    std::map<std::string, int> ages2 = {
        {"Alice", 25},
        {"Bob", 30},
        {"Charlie", 22}
    };
    print_map(ages2, "ages2");
    
    // 范围构造
    std::map<std::string, int> ages3(ages2.begin(), ages2.end());
    print_map(ages3, "ages3");
    
    // 拷贝构造
    std::map<std::string, int> ages4(ages2);
    print_map(ages4, "ages4");
    
    // ===== 2. 元素访问 =====
    std::cout << "\n=== 2. 元素访问 ===\n";
    
    // 下标操作符（存在则返回，不存在则插入默认值）
    std::cout << "ages2[\"Bob\"] = " << ages2["Bob"] << std::endl;
    
    // 安全访问（不存在时抛出异常）
    try {
        std::cout << "ages2[\"Frank\"] = " << ages2.at("Frank") << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    
    // 查找元素
    auto it = ages2.find("Charlie");
    if (it != ages2.end()) {
        std::cout << "Found Charlie: " << it->second << std::endl;
    } else {
        std::cout << "Charlie not found" << std::endl;
    }
    
    // ===== 3. 修改操作 =====
    std::cout << "\n=== 3. 修改操作 ===\n";
    
    // 插入元素
    ages2["David"] = 35;
    ages2.insert({"Eve", 28});
    ages2.emplace("Frank", 40);
    print_map(ages2, "ages2 after insertions");
    
    // 插入重复键（被忽略）
    ages2.insert({"Alice", 99});
    std::cout << "Inserting duplicate Alice: " << ages2["Alice"] << std::endl;
    
    // 使用insert的返回值
    auto result = ages2.insert({"Grace", 27});
    if (result.second) {
        std::cout << "Inserted Grace successfully" << std::endl;
    } else {
        std::cout << "Grace already exists" << std::endl;
    }
    
    // 删除元素
    ages2.erase("Charlie");
    print_map(ages2, "ages2 after erase");
    
    // ===== 4. 迭代器与遍历 =====
    std::cout << "\n=== 4. 迭代器与遍历 ===\n";
    
    // 正向遍历（按键升序）
    std::cout << "Forward iteration:\n";
    for (const auto& pair : ages2) {
        std::cout << "  [" << pair.first << "] = " << pair.second << "\n";
    }
    
    // 反向遍历（按键降序）
    std::cout << "Reverse iteration:\n";
    for (auto it = ages2.rbegin(); it != ages2.rend(); ++it) {
        std::cout << "  [" << it->first << "] = " << it->second << "\n";
    }
    
    // ===== 5. 自定义比较器 =====
    std::cout << "\n=== 5. 自定义比较器 ===\n";
    
    // 使用std::greater实现键的降序排列
    std::map<std::string, int, std::greater<std::string>> reversed_ages;
    reversed_ages.insert({"Alice", 25});
    reversed_ages.insert({"Bob", 30});
    reversed_ages.insert({"Charlie", 22});
    print_map(reversed_ages, "reversed_ages");
    
    // 自定义比较器（按字符串长度）
    struct CompareByLength {
        bool operator()(const std::string& a, const std::string& b) const {
            return a.length() < b.length();
        }
    };
    
    std::map<std::string, int, CompareByLength> length_ages;
    length_ages["Alice"] = 25;
    length_ages["Bob"] = 30;
    length_ages["Charlie"] = 22;
    print_map(length_ages, "length_ages");
    
    // ===== 6. 范围查找 =====
    std::cout << "\n=== 6. 范围查找 ===\n";
    
    std::map<int, std::string> numbers = {
        {1, "one"}, {2, "two"}, {3, "three"},
        {4, "four"}, {5, "five"}, {6, "six"}
    };
    
    // lower_bound和upper_bound
    auto low = numbers.lower_bound(3);  // 键>=3的第一个元素
    auto up = numbers.upper_bound(4);   // 键>4的第一个元素
    
    std::cout << "lower_bound(3): " << low->first << " => " << low->second << std::endl;
    std::cout << "upper_bound(4): " << up->first << " => " << up->second << std::endl;
    
    // equal_range
    auto range = numbers.equal_range(3);
    std::cout << "equal_range(3): ";
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << "[" << it->first << "] = " << it->second << " ";
    }
    std::cout << std::endl;
    
    // ===== 7. 性能考量 =====
    std::cout << "\n=== 7. 性能考量 ===\n";
    
    // 高效插入（避免重复查找）
    std::map<std::string, int> scores;
    
    // 低效方式
    if (scores.find("Alice") == scores.end()) {
        scores["Alice"] = 100;  // 两次查找：find和[]
    }
    
    // 高效方式
    scores.emplace("Bob", 95);  // 单次查找
    
    // 另一种高效插入方式
    scores.insert_or_assign("Charlie", 88);
    
    print_map(scores, "scores");
    
    return 0;
}