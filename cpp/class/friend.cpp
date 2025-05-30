#include <iostream>
#include <cmath>

// 前向声明（用于成员函数作为友元的情况）
class B;

// 1. 友元函数示例：计算两点距离
class Point {
private:
    int x;
    int y;

public:
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    
    // 声明友元函数
    friend double distance(const Point& p1, const Point& p2);
    
    // 友元函数：重载 << 运算符
    friend std::ostream& operator<<(std::ostream& os, const Point& p);
};

// 友元函数定义：计算两点距离
double distance(const Point& p1, const Point& p2) {
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + 
                     (p1.y - p2.y) * (p1.y - p2.y));
}

// 友元函数定义：重载 << 运算符
std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}


// 2. 友元类示例：矩形与正方形
class Rectangle {
private:
    int width;
    int height;

public:
    Rectangle(int w = 0, int h = 0) : width(w), height(h) {}
    
    // 声明友元类
    friend class Square;
    
    // 获取面积（用于测试）
    int area() const { return width * height; }
};

class Square {
public:
    // 可以访问 Rectangle 的私有成员
    void setRectangleWidth(Rectangle& rect, int width) {
        rect.width = width;
    }
    
    // 创建正方形对应的矩形
    Rectangle createRectangle(int side) {
        return Rectangle(side, side);
    }
};


// 3. 成员函数作为友元示例
class A {
private:
    int value;

public:
    A(int v = 0) : value(v) {}
    void setValueFromB(B& b);  // 声明但不定义
    int getValue() const { return value; }
};

class B {
private:
    int secret;

public:
    B(int s) : secret(s) {}
    
    // 将 A::setValueFromB 声明为友元
    friend void A::setValueFromB(B& b);
};

// 定义友元函数（必须在 B 类定义后）
void A::setValueFromB(B& b) {
    value = b.secret;  // 可以访问 B 的私有成员
}


// 4. 双向友元示例
class X {
private:
    int xValue;

public:
    X(int v = 0) : xValue(v) {}
    friend class Y;  // Y 是 X 的友元
};

class Y {
private:
    int yValue;

public:
    Y(int v = 0) : yValue(v) {}
    friend class X;  // X 是 Y 的友元
    
    // Y 可以访问 X 的私有成员
    int getXValue(const X& x) { return x.xValue; }
    
    // X 可以访问 Y 的私有成员（通过 X 的友元函数）
    friend int getYValue(const Y& y);
};

// X 的友元函数，可以访问 Y 的私有成员
int getYValue(const Y& y) {
    return y.yValue;
}


int main() {
    // 测试友元函数
    Point p1(1, 2), p2(4, 6);
    std::cout << "Points: " << p1 << " and " << p2 << std::endl;
    std::cout << "Distance: " << distance(p1, p2) << std::endl;
    
    // 测试友元类
    Rectangle rect(3, 4);
    Square square;
    square.setRectangleWidth(rect, 5);
    std::cout << "Rectangle area after modification: " << rect.area() << std::endl;
    
    // 测试成员函数作为友元
    B b(100);
    A a;
    a.setValueFromB(b);
    std::cout << "Value of A after setting from B: " << a.getValue() << std::endl;
    
    // 测试双向友元
    X x(5);
    Y y(15);
    std::cout << "X value accessed by Y: " << y.getXValue(x) << std::endl;
    std::cout << "Y value accessed by X's friend: " << getYValue(y) << std::endl;
    
    return 0;
}