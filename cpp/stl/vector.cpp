#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include <iterator>
#include <chrono>
#include <random>

// 辅助函数：打印vector信息
template<typename T>
void print_vector(const std::vector<T>& vec, const std::string& name = "") {
    if (!name.empty()) std::cout << name << ": ";
    std::cout << "[ ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << "] (size=" << vec.size() << ", capacity=" << vec.capacity() << ")\n";
}

// 辅助函数：测量代码执行时间
template<typename Func>
auto measure_time(Func&& func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

int main() {
    // ===== 1. 初始化方式 =====
    std::cout << "\n=== 1. 初始化方式 ===\n";
    
    // 默认构造，空vector
    std::vector<int> vec1;
    print_vector(vec1, "vec1");

    // 指定大小，默认初始化
    std::vector<int> vec2(5);
    print_vector(vec2, "vec2");

    // 指定大小和初始值
    std::vector<int> vec3(5, 42);
    print_vector(vec3, "vec3");

    // 初始化列表
    std::vector<int> vec4 = {1, 2, 3, 4, 5};
    print_vector(vec4, "vec4");

    // 从其他容器复制
    std::vector<int> vec5(vec4.begin(), vec4.end());
    print_vector(vec5, "vec5");

    // 移动构造
    std::vector<int> vec6(std::vector<int>{10, 20, 30});
    print_vector(vec6, "vec6");

    // ===== 2. 元素访问 =====
    std::cout << "\n=== 2. 元素访问 ===\n";
    
    // 下标操作符（无边界检查）
    std::cout << "vec4[2]: " << vec4[2] << std::endl;
    
    // at() 方法（有边界检查）
    try {
        std::cout << "vec4.at(10): " << vec4.at(10) << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    // 首尾元素
    std::cout << "vec4.front(): " << vec4.front() << std::endl;
    std::cout << "vec4.back(): " << vec4.back() << std::endl;

    // 原始指针访问
    int* ptr = vec4.data();
    std::cout << "ptr[3]: " << ptr[3] << std::endl;

    // ===== 3. 动态扩容机制 =====
    std::cout << "\n=== 3. 动态扩容机制 ===\n";
    
    std::vector<int> growth_vec;
    std::cout << "扩容过程：\n";
    for (int i = 0; i < 20; ++i) {
        growth_vec.push_back(i);
        std::cout << "添加 " << i << " 后: size=" << growth_vec.size() 
                  << ", capacity=" << growth_vec.capacity() << std::endl;
    }

    // 预分配空间
    std::vector<int> reserved_vec;
    reserved_vec.reserve(20);
    std::cout << "\n使用reserve预分配：\n";
    for (int i = 0; i < 20; ++i) {
        reserved_vec.push_back(i);
        std::cout << "添加 " << i << " 后: size=" << reserved_vec.size() 
                  << ", capacity=" << reserved_vec.capacity() << std::endl;
    }

    // ===== 4. 迭代器操作 =====
    std::cout << "\n=== 4. 迭代器操作 ===\n";
    
    // 正向迭代
    std::cout << "正向迭代: ";
    for (auto it = vec4.begin(); it != vec4.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 反向迭代
    std::cout << "反向迭代: ";
    for (auto it = vec4.rbegin(); it != vec4.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // C++11 范围for循环
    std::cout << "范围for循环: ";
    for (const auto& elem : vec4) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // ===== 5. 修改操作 =====
    std::cout << "\n=== 5. 修改操作 ===\n";
    
    // 尾部插入
    vec4.push_back(6);
    print_vector(vec4, "push_back(6)后");

    // 构造并插入（避免拷贝）
    std::vector<std::string> str_vec;
    str_vec.emplace_back("hello");
    str_vec.emplace_back(5, 'a');  // 构造 "aaaaa"
    print_vector(str_vec, "emplace_back后");

    // 插入元素
    vec4.insert(vec4.begin() + 2, 100);
    print_vector(vec4, "insert 100后");

    // 删除元素
    vec4.erase(vec4.begin() + 2);
    print_vector(vec4, "erase后");

    // 清空容器
    vec4.clear();
    print_vector(vec4, "clear后");

    // ===== 6. STL 算法应用 =====
    std::cout << "\n=== 6. STL 算法应用 ===\n";
    
    vec4 = {5, 3, 4, 1, 2};
    
    // 排序
    std::sort(vec4.begin(), vec4.end());
    print_vector(vec4, "排序后");

    // 查找
    auto it = std::find(vec4.begin(), vec4.end(), 3);
    if (it != vec4.end()) {
        std::cout << "找到元素3在位置: " << std::distance(vec4.begin(), it) << std::endl;
    }

    // 求和
    int sum = std::accumulate(vec4.begin(), vec4.end(), 0);
    std::cout << "元素和: " << sum << std::endl;

    // ===== 7. 性能优化 =====
    std::cout << "\n=== 7. 性能优化 ===\n";
    
    // 对比push_back与emplace_back
    struct ExpensiveToCopy {
        int data[1000];
        ExpensiveToCopy(int value) {
            for (int i = 0; i < 1000; ++i) data[i] = value;
        }
    };

    std::vector<ExpensiveToCopy> v1, v2;
    
    auto push_time = measure_time([&] {
        for (int i = 0; i < 1000; ++i) {
            v1.push_back(ExpensiveToCopy(i));
        }
    });

    auto emplace_time = measure_time([&] {
        for (int i = 0; i < 1000; ++i) {
            v2.emplace_back(i);
        }
    });

    std::cout << "push_back耗时: " << push_time << " 微秒" << std::endl;
    std::cout << "emplace_back耗时: " << emplace_time << " 微秒" << std::endl;

    // ===== 8. 迭代器失效 =====
    std::cout << "\n=== 8. 迭代器失效 ===\n";
    
    std::vector<int> nums = {1, 2, 3, 4, 5};
    
    // 错误示例：可能导致未定义行为
    /*
    for (auto it = nums.begin(); it != nums.end(); ++it) {
        if (*it % 2 == 0) {
            nums.erase(it);  // 迭代器失效
        }
    }
    */

    // 正确示例：处理迭代器失效
    for (auto it = nums.begin(); it != nums.end();) {
        if (*it % 2 == 0) {
            it = nums.erase(it);  // erase返回下一个有效迭代器
        } else {
            ++it;
        }
    }
    print_vector(nums, "删除偶数后");

    return 0;
}