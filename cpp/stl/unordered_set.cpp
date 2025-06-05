#include <iostream>
#include <unordered_set>
#include <string>
#include <functional> // 提供 std::hash

int main() {
    // === 基本用法示例 ===
    std::cout << "=== 基本用法 ===\n";
    std::unordered_set<int> numbers;

    // 插入元素（多种方式）
    numbers.insert(10);                 // 插入单个元素
    numbers.insert({20, 30, 40});       // 插入初始化列表
    numbers.emplace(50);                // 使用 emplace 就地构造

    // 检查插入结果
    auto result = numbers.insert(30);   // 尝试插入重复元素
    if (!result.second) {
        std::cout << "元素 30 已存在\n";
    }

    // 查找元素
    if (numbers.find(40) != numbers.end()) {
        std::cout << "找到元素 40\n";
    }

    // 删除元素
    numbers.erase(20);
    std::cout << "删除 20 后，集合大小: " << numbers.size() << "\n";

    // 遍历集合（顺序不确定）
    std::cout << "集合中的元素: ";
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << "\n\n";


    // === 性能优化示例 ===
    std::cout << "=== 性能优化 ===\n";
    std::unordered_set<std::string> words;

    // 预先分配足够的桶，避免多次扩容
    words.reserve(1000);

    // 设置较低的负载因子，减少冲突
    words.max_load_factor(0.5);

    // 插入大量元素
    for (int i = 0; i < 100; ++i) {
        words.insert("word_" + std::to_string(i));
    }

    // 检查负载因子和桶数量
    std::cout << "负载因子: " << words.load_factor() << "\n";
    std::cout << "桶数量: " << words.bucket_count() << "\n";
    std::cout << "元素数量: " << words.size() << "\n\n";


    // === 处理哈希冲突示例 ===
    std::cout << "=== 哈希冲突 ===\n";
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

    std::unordered_set<BadHashExample, BadHash> bad_set;
    bad_set.insert({1});
    bad_set.insert({2});
    bad_set.insert({3});

    // 检查冲突情况
    std::cout << "糟糕哈希函数的桶数量: " << bad_set.bucket_count() << "\n";
    std::cout << "桶 42 的元素数量: " << bad_set.bucket_size(42) << "\n\n";

    return 0;
}