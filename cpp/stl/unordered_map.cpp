#include <iostream>
#include <unordered_map>
#include <string>
#include <functional> // 提供 std::hash

int main() {
    // === 基本用法示例 ===
    std::cout << "=== 基本用法 ===\n";
    std::unordered_map<std::string, int> ages;

    // 插入元素（多种方式）
    ages["Alice"] = 25;                 // 使用 operator[]
    ages.insert({"Bob", 30});           // 使用 insert + 初始化列表
    ages.emplace("Charlie", 35);        // 使用 emplace 就地构造

    // 访问元素
    std::cout << "Bob's age: " << ages["Bob"] << "\n";  // 通过键访问

    // 查找元素
    if (ages.find("Charlie") != ages.end()) {
        std::cout << "Charlie found\n";
    }

    // 删除元素
    ages.erase("Alice");
    std::cout << "After erase Alice, size: " << ages.size() << "\n";

    // 遍历（顺序不确定）
    std::cout << "All elements:\n";
    for (const auto& [name, age] : ages) {  // C++17 结构化绑定
        std::cout << name << ": " << age << "\n";
    }

    // === 性能优化示例 ===
    std::cout << "\n=== 性能优化 ===\n";
    std::unordered_map<int, std::string> performance_map;

    // 预先分配足够的桶，避免多次扩容
    performance_map.reserve(1000);

    // 设置较低的负载因子，减少冲突
    performance_map.max_load_factor(0.5);

    // 插入大量元素
    for (int i = 0; i < 1000; ++i) {
        performance_map[i] = "Value " + std::to_string(i);
    }

    // 检查负载因子和桶数量
    std::cout << "Load factor: " << performance_map.load_factor() << "\n";
    std::cout << "Bucket count: " << performance_map.bucket_count() << "\n";
    std::cout << "Element count: " << performance_map.size() << "\n";


    // === 处理哈希冲突示例 ===
    std::cout << "\n=== 哈希冲突 ===\n";
    struct BadHashExample {
        int value;
        bool operator==(const BadHashExample& other) const {
            return value == other.value;
        }
    };

    // 故意设计一个糟糕的哈希函数（所有值都返回相同哈希）
    struct BadHash {
        std::size_t operator()(const BadHashExample&) const {
            return 42; // 所有对象返回相同哈希值，导致严重冲突
        }
    };

    std::unordered_map<BadHashExample, std::string, BadHash> bad_map;
    bad_map[{1}] = "One";
    bad_map[{2}] = "Two";

    // 检查冲突情况
    std::cout << "Bad map bucket count: " << bad_map.bucket_count() << "\n";
    std::cout << "Bucket 42 size: " << bad_map.bucket_size(42) << "\n"; // 所有元素都在同一个桶

    return 0;
}