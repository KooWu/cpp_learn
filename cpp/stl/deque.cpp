#include <iostream>
#include <deque>
#include <algorithm>
#include <string>
#include <iterator>
#include <numeric>
#include <chrono>
#include <vector>

// 辅助函数：打印deque信息
template<typename T>
void print_deque(const std::deque<T>& deq, const std::string& name = "") {
    if (!name.empty()) std::cout << name << ": ";
    std::cout << "[ ";
    for (const auto& elem : deq) {
        std::cout << elem << " ";
    }
    std::cout << "] (size=" << deq.size() << ")\n";
}

int main() {
    // ===== 1. 初始化方式 =====
    std::cout << "\n=== 1. 初始化方式 ===\n";
    
    // 默认构造，空deque
    std::deque<int> deq1;
    print_deque(deq1, "deq1");

    // 指定大小，默认初始化
    std::deque<int> deq2(5);
    print_deque(deq2, "deq2");

    // 指定大小和初始值
    std::deque<int> deq3(5, 42);
    print_deque(deq3, "deq3");

    // 初始化列表
    std::deque<int> deq4 = {1, 2, 3, 4, 5};
    print_deque(deq4, "deq4");

    // 从其他容器复制
    std::deque<int> deq5(deq4.begin(), deq4.end());
    print_deque(deq5, "deq5");

    // 拷贝构造
    std::deque<int> deq6(deq4);
    print_deque(deq6, "deq6");

    // ===== 2. 元素访问 =====
    std::cout << "\n=== 2. 元素访问 ===\n";
    
    // 访问首尾元素
    std::cout << "deq4.front(): " << deq4.front() << std::endl;
    std::cout << "deq4.back(): " << deq4.back() << std::endl;

    // 随机访问
    std::cout << "deq4[2]: " << deq4[2] << std::endl;
    std::cout << "deq4.at(3): " << deq4.at(3) << std::endl;

    // ===== 3. 双端操作 =====
    std::cout << "\n=== 3. 双端操作 ===\n";
    
    // 头部插入
    deq4.push_front(0);
    print_deque(deq4, "push_front(0)后");

    // 尾部插入
    deq4.push_back(6);
    print_deque(deq4, "push_back(6)后");

    // 头部删除
    deq4.pop_front();
    print_deque(deq4, "pop_front()后");

    // 尾部删除
    deq4.pop_back();
    print_deque(deq4, "pop_back()后");

    // ===== 4. 插入与删除 =====
    std::cout << "\n=== 4. 插入与删除 ===\n";
    
    // 指定位置插入
    auto it = deq4.begin() + 2;
    deq4.insert(it, 10);
    print_deque(deq4, "insert(10)后");

    // 插入多个元素
    deq4.insert(deq4.end(), {7, 8, 9});
    print_deque(deq4, "插入多个元素后");

    // 删除指定位置元素
    deq4.erase(deq4.begin() + 3);
    print_deque(deq4, "erase()后");

    // 删除范围元素
    deq4.erase(deq4.begin() + 1, deq4.begin() + 3);
    print_deque(deq4, "删除范围元素后");

    // ===== 5. 迭代器使用 =====
    std::cout << "\n=== 5. 迭代器使用 ===\n";
    
    // 正向迭代
    std::cout << "正向迭代: ";
    for (auto it = deq4.begin(); it != deq4.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 反向迭代
    std::cout << "反向迭代: ";
    for (auto it = deq4.rbegin(); it != deq4.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // ===== 6. 算法应用 =====
    std::cout << "\n=== 6. 算法应用 ===\n";
    
    // 排序
    std::sort(deq4.begin(), deq4.end());
    print_deque(deq4, "排序后");

    // 反转
    std::reverse(deq4.begin(), deq4.end());
    print_deque(deq4, "反转后");

    // 查找
    it = std::find(deq4.begin(), deq4.end(), 7);
    if (it != deq4.end()) {
        std::cout << "找到元素7，位置: " << std::distance(deq4.begin(), it) << std::endl;
    }

    // ===== 7. 性能对比 =====
    std::cout << "\n=== 7. 性能对比 ===\n";
    
    // 准备数据
    std::deque<int> deq_for_perf;
    std::vector<int> vec_for_perf;
    
    // 测试头部插入性能
    const int N = 100000;
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        deq_for_perf.push_front(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "deque 头部插入 " << N << " 元素耗时: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        vec_for_perf.insert(vec_for_perf.begin(), i);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "vector 头部插入 " << N << " 元素耗时: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms" << std::endl;

    return 0;
}    