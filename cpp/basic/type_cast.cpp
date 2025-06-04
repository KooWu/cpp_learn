#include <iostream>
#include <string>

// 基础类层次结构用于演示dynamic_cast
class Animal {
public:
    virtual ~Animal() = default; // 必须为虚析构函数以启用RTTI
    virtual void speak() const { std::cout << "Animal speaks" << std::endl; }
};

class Dog : public Animal {
public:
    void speak() const override { std::cout << "Dog barks" << std::endl; }
    void fetch() const { std::cout << "Dog fetches" << std::endl; }
};

class Cat : public Animal {
public:
    void speak() const override { std::cout << "Cat meows" << std::endl; }
};

// 用于演示const_cast的类
class Data {
public:
    explicit Data(int value) : m_value(value) {}
    int getValue() const { return m_value; }
    void setValue(int value) { m_value = value; }
private:
    mutable int m_value;
};

int main() {
    // ===== 1. static_cast 示例 =====
    std::cout << "=== static_cast ===" << std::endl;
    
    // 基本数据类型转换
    double d = 3.14;
    int i = static_cast<int>(d); // 截断小数部分，i = 3
    std::cout << "double to int: " << i << std::endl;
    
    // 类层次结构中的上行转换（安全）
    Dog dog;
    Animal* animalPtr = static_cast<Animal*>(&dog); // 子类→父类
    animalPtr->speak(); // 输出: Dog barks
    
    // 类层次结构中的下行转换（不安全）
    Animal* animalPtr2 = new Animal();
    Dog* dogPtr = static_cast<Dog*>(animalPtr2); // 编译通过，但运行时行为未定义
    // dogPtr->fetch(); // 危险！可能导致程序崩溃
    
    // 非相关类型指针转换（编译警告但允许）
    int num = 42;
    void* voidPtr = static_cast<void*>(&num); // int* → void*
    int* numPtr = static_cast<int*>(voidPtr); // void* → int*
    std::cout << "*numPtr: " << *numPtr << std::endl;
    
    // ===== 2. dynamic_cast 示例 =====
    std::cout << "\n=== dynamic_cast ===" << std::endl;
    
    Animal* animalPtr3 = new Dog();
    Dog* dogPtr2 = dynamic_cast<Dog*>(animalPtr3); // 安全的下行转换
    if (dogPtr2) {
        dogPtr2->fetch(); // 输出: Dog fetches
    }
    
    Animal* animalPtr4 = new Cat();
    Dog* dogPtr3 = dynamic_cast<Dog*>(animalPtr4); // 转换失败，返回nullptr
    if (!dogPtr3) {
        std::cout << "dynamic_cast from Cat* to Dog* failed" << std::endl;
    }
    
    // ===== 3. const_cast 示例 =====
    std::cout << "\n=== const_cast ===" << std::endl;
    
    const Data constData(10);
    // constData.setValue(20); // 错误：无法修改const对象
    
    // 移除const属性（危险！）
    Data* nonConstData = const_cast<Data*>(&constData);
    nonConstData->setValue(20);
    std::cout << "constData value: " << constData.getValue() << std::endl; // 输出: 20
    
    // 向非const对象添加const属性
    Data originalData(30);
    const Data* anotherConstData = const_cast<const Data*>(&originalData);
    // anotherConstData->setValue(40); // 错误：无法通过const指针修改
    
    // ===== 4. reinterpret_cast 示例 =====
    std::cout << "\n=== reinterpret_cast ===" << std::endl;
    
    int intValue = 12345;
    long longValue = reinterpret_cast<long>(&intValue); // 指针→整数
    std::cout << "Address of intValue as long: " << std::hex << longValue << std::endl;
    
    // 函数指针转换（极其危险！）
    typedef void (*VoidFuncPtr)();
    int (*intFuncPtr)() = []() { return 42; };
    // VoidFuncPtr voidFunc = reinterpret_cast<VoidFuncPtr>(intFuncPtr); // 未定义行为！
    
    // ===== 5. C风格强制类型转换 =====
    std::cout << "\n=== C-Style Cast ===" << std::endl;
    
    double pi = 3.14159;
    int truncated = (int)pi; // C风格强制转换
    std::cout << "C-style cast: " << truncated << std::endl;
    
    // 清理资源
    delete animalPtr2;
    delete animalPtr3;
    delete animalPtr4;
    
    return 0;
}    