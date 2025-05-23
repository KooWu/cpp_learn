#include <iostream>
#include <vector>

class Resource {
private:
    int* data;
    size_t size;
public:
    Resource(size_t s) : size(s), data(new int[s]) {
        std::cout << "构造: 分配" << size << "个元素" << std::endl;
    }
    
    // 复制构造函数
    Resource(const Resource& other) : size(other.size), data(new int[size]) {
        std::cout << "复制构造: 分配并复制" << size << "个元素" << std::endl;
        for(size_t i=0; i<size; i++) {
            data[i] = other.data[i];
        }
    }
    
    // 移动构造函数（标记为noexcept）
    Resource(Resource&& other) noexcept : data(other.data), size(other.size) {
        std::cout << "移动构造: 转移" << size << "个元素的所有权" << std::endl;
        other.data = nullptr;
    }

    // // 移动构造函数（未标记为noexcept）
    // Resource(Resource&& other) : data(other.data), size(other.size) {
    //     std::cout << "移动构造: 转移" << size << "个元素的所有权" << std::endl;
    //     other.data = nullptr;
    // }
    
    ~Resource() {
        delete[] data;
    }
};

#include <chrono>
#include <iostream>
#include <vector>

// 非noexcept函数
int add1(int a, int b) {
    return a + b;
}

// noexcept函数
int add2(int a, int b) noexcept {
    return a + b;
}

int main() {
    std::vector<Resource> vec;
    
    // 触发多次扩容
    for(int i=0; i<3; i++) {
        vec.push_back(Resource(100));
        std::cout << "当前容量: " << vec.capacity() << std::endl;
    }

    const int N = 100000000;
    int result = 0;
    
    // 测试add1
    auto start1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        result += add1(i, i+1);
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count();
    
    // 测试add2
    auto start2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        result += add2(i, i+1);
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count();
    
    std::cout << "add1 (non-noexcept): " << duration1 << " ms" << std::endl;
    std::cout << "add2 (noexcept): " << duration2 << " ms" << std::endl;
    std::cout << "性能提升: " << (1.0 - static_cast<double>(duration2)/duration1) * 100 << "%" << std::endl;
}