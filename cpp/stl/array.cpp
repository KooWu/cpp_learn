#include <iostream>
#include <array>
#include <algorithm>
#include <numeric>
#include <string>
#include <iterator>
#include <type_traits>

// 辅助函数：打印数组信息
template<typename T, size_t N>
void print_array(const std::array<T, N>& arr, const std::string& name = "") {
    if (!name.empty()) std::cout << name << ": ";
    std::cout << "[ ";
    for (const auto& elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << "] (size=" << arr.size() << ")\n";
}

int main() {
    // ===== 1. 初始化方式 =====
    std::cout << "\n=== 1. 初始化方式 ===\n";
    
    // 默认初始化（元素类型必须支持默认构造）
    std::array<int, 5> arr1;
    print_array(arr1, "arr1");  // 可能包含未初始化值（依赖元素类型）

    // 值初始化
    std::array<int, 5> arr2 = {};
    print_array(arr2, "arr2");  // 全0

    // 聚合初始化
    std::array<int, 5> arr3 = {1, 2, 3};
    print_array(arr3, "arr3");  // 1 2 3 0 0

    // 统一初始化语法
    std::array<int, 5> arr4{10, 20, 30, 40, 50};
    print_array(arr4, "arr4");

    // 字符串字面量初始化
    // std::array<char, 6> arr5 = "hello"; //无法进行初始化
    // print_array(arr5, "arr5");

    // 使用 fill() 填充
    std::array<std::string, 3> arr6;
    arr6.fill("test");
    print_array(arr6, "arr6");

    // ===== 2. 元素访问 =====
    std::cout << "\n=== 2. 元素访问 ===\n";
    
    // 下标访问（无边界检查）
    std::cout << "arr4[0]: " << arr4[0] << std::endl;
    
    // at() 访问（有边界检查）
    try {
        std::cout << "arr4.at(10): " << arr4.at(10) << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    // 首尾元素
    std::cout << "arr4.front(): " << arr4.front() << std::endl;
    std::cout << "arr4.back(): " << arr4.back() << std::endl;

    // 原始指针访问
    int* ptr = arr4.data();
    std::cout << "ptr[2]: " << ptr[2] << std::endl;

    // ===== 3. 迭代器操作 =====
    std::cout << "\n=== 3. 迭代器操作 ===\n";
    
    // 正向迭代
    std::cout << "Forward iteration: ";
    for (auto it = arr4.begin(); it != arr4.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 反向迭代
    std::cout << "Reverse iteration: ";
    for (auto it = arr4.rbegin(); it != arr4.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // C++11 范围for循环
    std::cout << "Range-based for loop: ";
    for (const auto& elem : arr4) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // ===== 4. STL 算法应用 =====
    std::cout << "\n=== 4. STL 算法应用 ===\n";
    
    // 排序
    std::array<int, 5> arr7 = {5, 3, 4, 1, 2};
    std::sort(arr7.begin(), arr7.end());
    print_array(arr7, "Sorted arr7");

    // 查找
    auto it = std::find(arr7.begin(), arr7.end(), 3);
    if (it != arr7.end()) {
        std::cout << "Found 3 at position: " << std::distance(arr7.begin(), it) << std::endl;
    }

    // 求和
    int sum = std::accumulate(arr7.begin(), arr7.end(), 0);
    std::cout << "Sum of arr7: " << sum << std::endl;

    // ===== 5. 作为函数参数 =====
    std::cout << "\n=== 5. 作为函数参数 ===\n";
    
    // 通过引用传递（避免拷贝）
    auto modify_array = [](std::array<int, 5>& arr) {
        for (auto& elem : arr) {
            elem *= 2;
        }
    };
    
    modify_array(arr4);
    print_array(arr4, "Modified arr4");

    // ===== 6. 多维数组 =====
    std::cout << "\n=== 6. 多维数组 ===\n";
    
    std::array<std::array<int, 3>, 2> matrix = {
        { {1, 2, 3}, {4, 5, 6} }
    };
    
    // 访问多维数组
    std::cout << "matrix[0][1]: " << matrix[0][1] << std::endl;
    
    // 遍历多维数组
    for (const auto& row : matrix) {
        for (const auto& elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }

    // ===== 7. 元编程与编译时特性 =====
    std::cout << "\n=== 7. 元编程与编译时特性 ===\n";
    
    // 编译时大小检查
    static_assert(arr4.size() == 5, "Array size must be 5");
    
    // 使用编译时算法
    constexpr std::array<int, 3> const_arr = {1, 2, 3};
    constexpr int first_elem = const_arr[0];
    static_assert(first_elem == 1, "First element must be 1");

    return 0;
}