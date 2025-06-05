#include <iostream>
#include <list>
#include <algorithm>
#include <string>
#include <iterator>
#include <numeric>
#include <chrono>
#include <vector>

// 辅助函数：打印list信息
template<typename T>
void print_list(const std::list<T>& lst, const std::string& name = "") {
    if (!name.empty()) std::cout << name << ": ";
    std::cout << "[ ";
    for (const auto& elem : lst) {
        std::cout << elem << " ";
    }
    std::cout << "] (size=" << lst.size() << ")\n";
}

int main() {
    // ===== 1. 初始化方式 =====
    std::cout << "\n=== 1. 初始化方式 ===\n";
    
    // 默认构造，空list
    std::list<int> lst1;
    print_list(lst1, "lst1");

    // 指定大小，默认初始化
    std::list<int> lst2(5);
    print_list(lst2, "lst2");

    // 指定大小和初始值
    std::list<int> lst3(5, 42);
    print_list(lst3, "lst3");

    // 初始化列表
    std::list<int> lst4 = {1, 2, 3, 4, 5};
    print_list(lst4, "lst4");

    // 从其他容器复制
    std::list<int> lst5(lst4.begin(), lst4.end());
    print_list(lst5, "lst5");

    // 拷贝构造
    std::list<int> lst6(lst4);
    print_list(lst6, "lst6");

    // ===== 2. 元素访问 =====
    std::cout << "\n=== 2. 元素访问 ===\n";
    
    // 访问首尾元素
    std::cout << "lst4.front(): " << lst4.front() << std::endl;
    std::cout << "lst4.back(): " << lst4.back() << std::endl;

    // 使用迭代器访问（不支持随机访问）
    auto it = lst4.begin();
    std::advance(it, 2);  // 移动到第3个元素
    std::cout << "第三个元素: " << *it << std::endl;

    // ===== 3. 修改操作 =====
    std::cout << "\n=== 3. 修改操作 ===\n";
    
    // 头部插入
    lst4.push_front(0);
    print_list(lst4, "push_front(0)后");

    // 尾部插入
    lst4.push_back(6);
    print_list(lst4, "push_back(6)后");

    // 指定位置插入
    it = std::find(lst4.begin(), lst4.end(), 3);
    if (it != lst4.end()) {
        lst4.insert(it, 20);
    }
    print_list(lst4, "insert(20)后");

    // 删除头部元素
    lst4.pop_front();
    print_list(lst4, "pop_front()后");

    // 删除尾部元素
    lst4.pop_back();
    print_list(lst4, "pop_back()后");

    // 删除指定位置元素
    it = std::find(lst4.begin(), lst4.end(), 20);
    if (it != lst4.end()) {
        lst4.erase(it);
    }
    print_list(lst4, "erase(20)后");

    // ===== 4. 特殊操作：splice =====
    std::cout << "\n=== 4. 特殊操作：splice ===\n";
    
    std::list<int> lst7 = {10, 20, 30};
    std::list<int> lst8 = {40, 50};
    
    // 将lst8的所有元素转移到lst7的begin()位置
    lst7.splice(lst7.begin(), lst8);
    print_list(lst7, "lst7 splice后");
    print_list(lst8, "lst8 splice后");  // 此时lst8为空

    // 将lst7中值为20的元素之后的元素转移到lst8
    it = std::find(lst7.begin(), lst7.end(), 20);
    if (it != lst7.end()) {
        ++it;  // 移动到20之后的元素
        lst8.splice(lst8.begin(), lst7, it, lst7.end());
    }
    print_list(lst7, "lst7 第二次splice后");
    print_list(lst8, "lst8 第二次splice后");

    // ===== 5. 操作与算法 =====
    std::cout << "\n=== 5. 操作与算法 ===\n";
    
    // 反转列表
    lst4.reverse();
    print_list(lst4, "reverse()后");

    // 排序
    lst4.sort();
    print_list(lst4, "sort()后");

    // 去重（需要先排序）
    lst4.unique();
    print_list(lst4, "unique()后");

    // 合并两个已排序的列表
    std::list<int> lst9 = {3, 4, 5};
    std::list<int> lst10 = {1, 3, 7};
    lst9.sort();
    lst10.sort();
    lst9.merge(lst10);
    print_list(lst9, "merge()后");

    // ===== 6. 迭代器使用 =====
    std::cout << "\n=== 6. 迭代器使用 ===\n";
    
    // 正向迭代
    std::cout << "正向迭代: ";
    for (auto it = lst4.begin(); it != lst4.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 反向迭代
    std::cout << "反向迭代: ";
    for (auto it = lst4.rbegin(); it != lst4.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // ===== 7. 性能对比 =====
    std::cout << "\n=== 7. 性能对比：插入操作 ===\n";
    
    // 准备数据
    std::list<int> list_for_perf;
    std::vector<int> vec_for_perf;
    
    // 测试头部插入性能
    const int N = 100000;
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        list_for_perf.push_front(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "list 头部插入 " << N << " 元素耗时: " 
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