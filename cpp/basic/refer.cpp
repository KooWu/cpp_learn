#include <iostream>
#include <utility>
#include <vector>
#include <cstdio>

// 资源管理类
class Resource {
private:
    int* data;
    size_t size;
    
public:
    // 构造函数
    explicit Resource(size_t s) : size(s), data(new int[s]) {
        std::cout << "Construct: Allocated " << size << " elements" << std::endl;
    }
    
    // 析构函数
    ~Resource() {
        delete[] data;
    }
    
    // 拷贝构造函数
    Resource(const Resource& other) : size(other.size), data(new int[size]) {
        std::cout << "Copy Construct: Allocated and copied " << size << " elements" << std::endl;
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }
    
    // 移动构造函数
    Resource(Resource&& other) noexcept : data(other.data), size(other.size) {
        std::cout << "Move Construct: Transferred ownership of " << size << " elements" << std::endl;
        other.data = nullptr;
    }
    
    // 拷贝赋值运算符
    Resource& operator=(const Resource& other) {
        std::cout << "Copy Assign: Copied " << other.size << " elements" << std::endl;
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    
    // 移动赋值运算符
    Resource& operator=(Resource&& other) noexcept {
        std::cout << "Move Assign: Transferred ownership of " << other.size << " elements" << std::endl;
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
        }
        return *this;
    }
};

// 万能引用与完美转发示例
template<typename T>
void wrapper(T&& arg) {
    std::cout << "Wrapper called with ";
    if constexpr (std::is_lvalue_reference_v<T&&>) {
        std::cout << "lvalue" << std::endl;
    } else {
        std::cout << "rvalue" << std::endl;
    }
    
    // 使用完美转发保持参数的原始值类别
    process(std::forward<T>(arg));
}

// 处理函数（重载版本）
void process(Resource& res) {
    std::cout << "Processing lvalue" << std::endl;
}

void process(Resource&& res) {
    std::cout << "Processing rvalue" << std::endl;
    // 可以安全地将右值引用传递给移动构造函数
    Resource new_res(std::move(res));
}

#include <iostream>
#include <utility>

void printValue(int& x) {
    std::cout << "lvalue: " << x << std::endl;
}

void printValue(int&& x) {
    std::cout << "rvalue: " << x << std::endl;
}

int main() {
    // 测试左值
    Resource res1(1000);
    wrapper(res1);  // 传递左值
    
    // 测试右值
    wrapper(Resource(2000));  // 传递右值
    
    // 测试容器中的移动语义
    std::vector<Resource> vec;
    vec.push_back(Resource(3000));  // 调用移动构造函数

    int&& rref = 42;  // rref是右值引用类型的变量
    printf("%p\n", &rref);
    
    printValue(rref);  // 调用printValue(int&)，因为rref是左值
    
    printValue(100);   // 调用printValue(int&&)，因为100是右值
    
    printValue(std::move(rref));  // 强制转换为右值，调用printValue(int&&)
    
    return 0;
}