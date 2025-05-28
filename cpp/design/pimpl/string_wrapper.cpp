#include "string_wrapper.h"
#include <cstring>

class StringWrapper::Impl {
public:
    Impl() : data(nullptr), len(0) {}
    
    Impl(const char* str) {
        if (str) {
            len = std::strlen(str);
            data = new char[len + 1];
            std::strcpy(data, str);
        } else {
            data = nullptr;
            len = 0;
        }
    }
    
    ~Impl() {
        delete[] data;
    }
    
    // 拷贝构造函数
    Impl(const Impl& other) : len(other.len) {
        if (other.data) {
            data = new char[len + 1];
            std::strcpy(data, other.data);
        } else {
            data = nullptr;
        }
    }
    
    // 赋值运算符
    Impl& operator=(const Impl& other) {
        if (this != &other) {
            delete[] data;
            len = other.len;
            if (other.data) {
                data = new char[len + 1];
                std::strcpy(data, other.data);
            } else {
                data = nullptr;
            }
        }
        return *this;
    }
    
    // 移动构造函数
    Impl(Impl&& other) noexcept : data(other.data), len(other.len) {
        other.data = nullptr;
        other.len = 0;
    }
    
    // 移动赋值运算符
    Impl& operator=(Impl&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            len = other.len;
            other.data = nullptr;
            other.len = 0;
        }
        return *this;
    }
    
    const char* c_str() const { return data; }
    size_t length() const { return len; }
    
private:
    char* data;
    size_t len;
};

// StringWrapper实现
StringWrapper::StringWrapper() : pImpl(std::make_unique<Impl>()) {}
StringWrapper::StringWrapper(const char* str) : pImpl(std::make_unique<Impl>(str)) {}
StringWrapper::~StringWrapper() = default;

StringWrapper::StringWrapper(const StringWrapper& other) : pImpl(std::make_unique<Impl>(*other.pImpl)) {}
StringWrapper& StringWrapper::operator=(const StringWrapper& other) {
    *pImpl = *other.pImpl;
    return *this;
}

StringWrapper::StringWrapper(StringWrapper&& other) noexcept = default;
StringWrapper& StringWrapper::operator=(StringWrapper&& other) noexcept = default;

const char* StringWrapper::c_str() const { return pImpl->c_str(); }
size_t StringWrapper::length() const { return pImpl->length(); }