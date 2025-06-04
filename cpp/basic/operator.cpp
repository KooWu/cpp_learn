#include <iostream>
using namespace std;

// 复数类示例
class Complex {
private:
    double real;  // 实部
    double imag;  // 虚部

public:
    // 构造函数
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    // 重载+运算符
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    // 重载-运算符
    Complex operator-(const Complex& other) const {
        return Complex(real - other.real, imag - other.imag);
    }

    // 重载前置++运算符
    Complex& operator++() {
        ++real;
        return *this;
    }

    // 重载后置++运算符（通过int参数区分）
    Complex operator++(int) {
        Complex temp = *this;
        ++(*this);
        return temp;
    }

    // 重载赋值运算符
    Complex& operator=(const Complex& other) {
        if (this != &other) {
            real = other.real;
            imag = other.imag;
        }
        return *this;
    }

    // 声明友元函数
    friend Complex operator*(const Complex& c, double scalar);
    friend ostream& operator<<(ostream& os, const Complex& c);
    friend istream& operator>>(istream& is, Complex& c);
};

// 重载*运算符（复数乘以标量）
Complex operator*(const Complex& c, double scalar) {
    return Complex(c.real * scalar, c.imag * scalar);
}

// 重载<<运算符（输出流）
ostream& operator<<(ostream& os, const Complex& c) {
    os << c.real;
    if (c.imag >= 0) os << " + " << c.imag << "i";
    else os << " - " << -c.imag << "i";
    return os;
}

// 重载>>运算符（输入流）
istream& operator>>(istream& is, Complex& c) {
    cout << "Enter real and imaginary parts: ";
    is >> c.real >> c.imag;
    return is;
}

// 向量类示例
class Vector {
private:
    double x, y;

public:
    // 构造函数
    Vector(double x = 0, double y = 0) : x(x), y(y) {}

    // 重载+运算符
    Vector operator+(const Vector& other) const {
        return Vector(x + other.x, y + other.y);
    }

    // 重载-运算符
    Vector operator-(const Vector& other) const {
        return Vector(x - other.x, y - other.y);
    }

    // 重载*运算符（向量点乘）
    double operator*(const Vector& other) const {
        return x * other.x + y * other.y;
    }

    // 重载==运算符
    bool operator==(const Vector& other) const {
        return (x == other.x) && (y == other.y);
    }

    // 声明友元函数
    friend Vector operator*(double scalar, const Vector& v);
    friend ostream& operator<<(ostream& os, const Vector& v);
    friend istream& operator>>(istream& is, Vector& v);
};

// 重载*运算符（标量乘法）
Vector operator*(double scalar, const Vector& v) {
    return Vector(scalar * v.x, scalar * v.y);
}

// 重载<<运算符（输出流）
ostream& operator<<(ostream& os, const Vector& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

// 重载>>运算符（输入流）
istream& operator>>(istream& is, Vector& v) {
    cout << "Enter x and y components: ";
    is >> v.x >> v.y;
    return is;
}

int main() {
    // 测试复数类
    cout << "=== Complex Number Test ===" << endl;
    Complex a(3, 4);
    Complex b(1, -2);
    Complex c = a + b;
    Complex d = a * 2.5;

    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "a + b = " << c << endl;
    cout << "a * 2.5 = " << d << endl;

    ++a;
    cout << "After ++a: a = " << a << endl;

    Complex e;
    cout << "Enter a complex number:" << endl;
    cin >> e;
    cout << "You entered: " << e << endl;

    // 测试向量类
    cout << "\n=== Vector Test ===" << endl;
    Vector v1(2, 3);
    Vector v2(4, -1);
    Vector v3 = v1 + v2;
    double dotProduct = v1 * v2;

    cout << "v1 = " << v1 << endl;
    cout << "v2 = " << v2 << endl;
    cout << "v1 + v2 = " << v3 << endl;
    cout << "v1 * v2 (dot product) = " << dotProduct << endl;

    Vector v4 = 3.0 * v1;
    cout << "3.0 * v1 = " << v4 << endl;

    cout << "v1 == v2: " << (v1 == v2 ? "true" : "false") << endl;

    Vector v5;
    cout << "Enter a vector:" << endl;
    cin >> v5;
    cout << "You entered: " << v5 << endl;

    return 0;
}