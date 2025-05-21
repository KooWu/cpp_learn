//C++中的统一初始化（也称为列表初始化或大括号初始化）是自C++11起引入的一种初始化方式，
//它使用一对花括号 {} 来初始化变量、对象或数据结构。这种初始化方式旨在提供一种一致的初始化语法，
//并且有助于防止某些类型的问题，比如窄化转换（narrowing conversion）。

#include <iostream>
#include <vector>
#include <initializer_list>

using namespace std;

void printList(std::initializer_list<int> list) {
    for (auto elem : list) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

class MyVector {
public:
    // 构造函数接受一个 initializer_list
    MyVector(std::initializer_list<int> initList) {
        for (auto it = initList.begin(); it != initList.end(); ++it) {
            data.push_back(*it);
        }
    }

    void display() const {
        for (const auto& val : data) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

private:
    std::vector<int> data;
};

int main() {
    int value[]{1, 2, 3};       // initializer_list<T> 会关联一个array<T,n> 里面元素被编译器逐一分解传给函数
    vector<int> v{2, 3, 5};

    for (auto i: value) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    for (auto i: v) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    printList({1, 2, 3, 4});
    MyVector vec{1, 2, 3, 4, 5};
    vec.display();
    return 0;
}