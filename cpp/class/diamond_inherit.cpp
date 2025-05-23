#include <iostream>
#include <cstdint>

// 顶层基类
class Base {
public:
    virtual void func() { std::cout << "Base::func()" << std::endl; }
    int baseData;
};

// 中间层基类
class Derived1 : public Base {
public:
    void func() override { std::cout << "Derived1::func()" << std::endl; }
    int derived1Data;
};

class Derived2 : public Base {
public:
    void func() override { std::cout << "Derived2::func()" << std::endl; }
    int derived2Data;
};

// 底层派生类
class Final : public Derived1, public Derived2 {
public:
    void func() override { std::cout << "Final::func()" << std::endl; }
    int finalData;
};

static void test_dianond_public() {
    // 创建Final对象
    Final f;
    f.Derived1::baseData = 10;  // 必须指定路径，否则有二义性
    f.Derived2::baseData = 20;
    
    // 通过不同路径调用func()
    // Base* ptr1 = &f;  // 错误：二义性，无法确定是Derived1还是Derived2的Base
    // 必须显式转换
    Base* ptr1 = static_cast<Derived1*>(&f);
    Base* ptr2 = static_cast<Derived2*>(&f);
    
    ptr1->func();  // 输出: Final::func()
    ptr2->func();  // 输出: Final::func()
    
    // 内存布局分析
    std::cout << "\n=== 内存布局 ===" << std::endl;
    std::cout << "Final对象大小: " << sizeof(f) << " 字节" << std::endl;
    std::cout << "Final对象地址: " << &f << std::endl;
    
    // Derived1子对象
    std::cout << "\nDerived1子对象:" << std::endl;
    uintptr_t* vptr1 = reinterpret_cast<uintptr_t*>(&f);
    std::cout << "vptr1地址: " << vptr1 << std::endl;
    std::cout << "Derived1::baseData地址: " << &f.Derived1::baseData << std::endl;
    
    // Derived2子对象
    std::cout << "\nDerived2子对象:" << std::endl;
    uintptr_t* vptr2 = reinterpret_cast<uintptr_t*>(reinterpret_cast<char*>(&f) + sizeof(Derived1));
    std::cout << "vptr2地址: " << vptr2 << std::endl;
    std::cout << "Derived2::baseData地址: " << &f.Derived2::baseData << std::endl;
}

// 顶层基类（虚基类）
class Base1 {
public:
    virtual void func() { std::cout << "Base::func()" << std::endl; }
    int baseData;
};

// 中间层基类（虚继承）
class Derived11 : virtual public Base1 {
public:
    void func() override { std::cout << "Derived1::func()" << std::endl; }
    int derived1Data;
};

class Derived21 : virtual public Base1 {
public:
    void func() override { std::cout << "Derived2::func()" << std::endl; }
    int derived2Data;
};

// 底层派生类
class Final1 : public Derived11, public Derived21 {
public:
    void func() override { std::cout << "Final::func()" << std::endl; }
    int finalData;
};

//public
// +----------------+
// | Derived1::vptr | --> Final的Derived1虚表
// +----------------+
// | Derived1::baseData |
// +----------------+
// | derived1Data   |
// +----------------+
// | Derived2::vptr | --> Final的Derived2虚表
// +----------------+
// | Derived2::baseData |
// +----------------+
// | derived2Data   |
// +----------------+
// | finalData      |
// +----------------+

//virtual
// +----------------+
// | Derived1::vptr | --> Final的Derived1虚表
// +----------------+
// | derived1Data   |
// +----------------+
// | Derived2::vptr | --> Final的Derived2虚表
// +----------------+
// | derived2Data   |
// +----------------+
// | finalData      |
// +----------------+
// | Base::vptr     | --> Final的Base虚表
// +----------------+
// | baseData       |
// +----------------+

static void test_dianond_virtual() {
    // 创建Final对象
    Final1 f;
    f.baseData = 10;  // 无二义性，只有一份Base
    
    // 通过不同路径调用func()
    Base1* ptr1 = &f;  // 正确：只有一份Base
    Base1* ptr2 = &f;
    
    ptr1->func();  // 输出: Final::func()
    ptr2->func();  // 输出: Final::func()
    
    // 内存布局分析
    std::cout << "\n=== 内存布局 ===" << std::endl;
    std::cout << "Final对象大小: " << sizeof(f) << " 字节" << std::endl;
    std::cout << "Final对象地址: " << &f << std::endl;
    
    // Derived1子对象
    std::cout << "\nDerived1子对象:" << std::endl;
    uintptr_t* vptr1 = reinterpret_cast<uintptr_t*>(&f);
    std::cout << "vptr1地址: " << vptr1 << std::endl;
    
    // Derived2子对象
    std::cout << "\nDerived2子对象:" << std::endl;
    uintptr_t* vptr2 = reinterpret_cast<uintptr_t*>(reinterpret_cast<char*>(&f) + sizeof(Derived11) - sizeof(Base1));
    std::cout << "vptr2地址: " << vptr2 << std::endl;
    
    // 共享的Base子对象
    std::cout << "\n共享的Base子对象:" << std::endl;
    uintptr_t* vptrBase = reinterpret_cast<uintptr_t*>(reinterpret_cast<char*>(&f) + (sizeof(f) - sizeof(Base1)));
    std::cout << "Base::vptr地址: " << vptrBase << std::endl;
    std::cout << "baseData地址: " << &f.baseData << std::endl;
}

int main() {
    test_dianond_public();
    test_dianond_virtual();
    return 0;
}