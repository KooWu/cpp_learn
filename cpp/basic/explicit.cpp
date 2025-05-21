#include <iostream>

class Fraction {
public:
    explicit Fraction(int num) : numerator(num), denominator(1) {}
    int getNumerator() const {
        return numerator;
    }
    
    int getDenominator() const {
        return denominator;
    }
private:
    int numerator;
    int denominator;
};

void printFraction(const Fraction& f) {
    std::cout << "Fraction: " << f.getNumerator() << "/" << f.getDenominator() << std::endl;
}

class Temperature {
public:
    // 构造函数
    explicit Temperature(double celsius) : tempCelsius(celsius) {}

    // 转换操作符，允许隐式的转换为 double (摄氏度)
    operator double() const {
        return tempCelsius;
    }

    // 显式转换操作符，防止隐式的转换为 int (华氏度)
    explicit operator int() const {
        return static_cast<int>(tempCelsius * 9.0 / 5.0 + 32);
    }

private:
    double tempCelsius; // 存储摄氏度温度
};

int main() {
    // printFraction(5); // 在没有explicit的情况下，这里会发生隐式转换，5 被转换成了 Fraction(5)，有explicit的情况下，没有办法隐式转换
    printFraction(Fraction(5));

    Temperature t(25.0); // 创建一个 Temperature 对象，表示 25 摄氏度

    // 隐式转换为 double 是允许的
    double celsius = t; // 使用 operator double()
    std::cout << "Temperature in Celsius: " << celsius << std::endl;

    // 隐式转换为 int 是不允许的，会导致编译错误
    // int fahrenheit = t; // 错误：不能隐式转换

    // 必须使用显式转换
    int fahrenheit = static_cast<int>(t); // 使用 operator int()
    std::cout << "Temperature in Fahrenheit: " << fahrenheit << std::endl;

    // 另一种显式转换方式
    fahrenheit = int(t); // 也可以这样写
    std::cout << "Temperature in Fahrenheit again: " << fahrenheit << std::endl;
}