#include <iostream>
#include <set>
#include <string>

// 自定义结构体 Person
struct Person {
    std::string name;
    int age;
    
    // 重载输出运算符，便于打印 Person 对象
    friend std::ostream& operator<<(std::ostream& os, const Person& p) {
        os << "{" << p.name << ", " << p.age << "}";
        return os;
    }
};

// 自定义比较器：按 age 升序，age 相同时按 name 升序
struct ComparePerson {
    bool operator()(const Person& a, const Person& b) const {
        return a.age < b.age || (a.age == b.age && a.name < b.name);
    }
};

int main() {
    // 1. 基础用法：整数集合
    std::cout << "=== 整数集合示例 ===\n";
    std::set<int> intSet;
    intSet.insert(30);
    intSet.insert(10);
    intSet.insert(20);
    intSet.insert(10); // 重复元素，插入无效

    std::cout << "插入 30, 10, 20, 10 后的集合内容：";
    for (int num : intSet) {
        std::cout << num << " "; // 输出: 10 20 30
    }
    std::cout << "\n";

    // 查找元素
    if (intSet.find(20) != intSet.end()) {
        std::cout << "找到元素 20\n";
    }

    // 删除元素
    intSet.erase(20);
    std::cout << "删除 20 后的集合大小：" << intSet.size() << "\n"; // 输出: 2


    // 2. 自定义结构体集合
    std::cout << "\n=== 自定义结构体集合示例 ===\n";
    std::set<Person, ComparePerson> personSet;
    personSet.insert({"Alice", 25});
    personSet.insert({"Bob", 20});
    personSet.insert({"Charlie", 25});

    std::cout << "插入 Alice(25), Bob(20), Charlie(25) 后的集合内容：\n";
    for (const auto& person : personSet) {
        std::cout << person << "\n"; // 按 age 和 name 排序输出
    }


    // 3. 降序集合
    std::cout << "\n=== 降序集合示例 ===\n";
    std::set<int, std::greater<int>> descendingSet = {3, 1, 4, 1, 5};

    std::cout << "降序集合内容：";
    for (int num : descendingSet) {
        std::cout << num << " "; // 输出: 5 4 3 1
    }
    std::cout << "\n";


    // 4. 范围查询
    std::cout << "\n=== 范围查询示例 ===\n";
    std::set<int> rangeSet = {10, 20, 30, 40, 50};

    // 查找 >=20 且 <40 的元素
    auto low = rangeSet.lower_bound(20);  // 指向 20
    auto high = rangeSet.upper_bound(40); // 指向 50

    std::cout << "范围 [20, 40) 的元素：";
    for (auto it = low; it != high; ++it) {
        std::cout << *it << " "; // 输出: 20 30 40
    }
    std::cout << "\n";


    // 5. 统计单词频率（去重功能）
    std::cout << "\n=== 去重功能示例 ===\n";
    std::set<std::string> wordSet;
    std::string words[] = {"apple", "banana", "apple", "cherry"};

    for (const auto& word : words) {
        wordSet.insert(word);
    }

    std::cout << "去重后的单词集合：";
    for (const auto& word : wordSet) {
        std::cout << word << " "; // 输出: apple banana cherry
    }
    std::cout << "\n";

    return 0;
}