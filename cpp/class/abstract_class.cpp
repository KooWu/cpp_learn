#include <iostream>
#include <memory>

// 抽象类：动物
class Animal {
public:
    // 纯虚函数：发出声音
    virtual void sound() const = 0;
    
    // 普通虚函数：移动
    virtual void move() const {
        std::cout << "Animal moves." << std::endl;
    }
    
    // 虚析构函数
    virtual ~Animal() = default;
};

// 具体实现类：狗
class Dog : public Animal {
public:
    void sound() const override {
        std::cout << "Woof!" << std::endl;
    }
    
    void move() const override {
        std::cout << "Dog runs." << std::endl;
    }
};

// 具体实现类：鱼
class Fish : public Animal {
public:
    void sound() const override {
        std::cout << "Fish makes no sound." << std::endl;
    }
    
    void move() const override {
        std::cout << "Fish swims." << std::endl;
    }
};

int main() {
    // 通过智能指针创建多态对象
    std::unique_ptr<Animal> dog = std::make_unique<Dog>();
    std::unique_ptr<Animal> fish = std::make_unique<Fish>();
    
    // 调用纯虚函数和普通虚函数
    dog->sound();   // 输出：Woof!
    fish->sound();  // 输出：Fish makes no sound.
    
    dog->move();    // 输出：Dog runs.
    fish->move();   // 输出：Fish swims.
    
    return 0;
}