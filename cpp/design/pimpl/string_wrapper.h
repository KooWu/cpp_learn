#ifndef STRING_WRAPPER_H
#define STRING_WRAPPER_H


// 核心思想
// 通过不透明指针将类的实现细节完全隐藏在另一个实现类中，接口类仅保留一个指向实现的指针。
// 技术手段
// 前向声明：在接口头文件中仅声明实现类
// 智能指针：管理实现类对象的生命周期
// 编译防火墙：实现变更仅需重新编译实现文件

#include <memory>
#include <string>

class StringWrapper {
public:
    StringWrapper();
    StringWrapper(const char* str);
    ~StringWrapper();
    
    // 拷贝和移动操作
    StringWrapper(const StringWrapper& other);
    StringWrapper& operator=(const StringWrapper& other);
    StringWrapper(StringWrapper&& other) noexcept;
    StringWrapper& operator=(StringWrapper&& other) noexcept;
    
    const char* c_str() const;
    size_t length() const;
    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

#endif