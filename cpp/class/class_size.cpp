
#include<iostream>

using namespace std;

class A{};

class A1
{
    public:
        char b;
        virtual void fun() {};
        void fun1() {};
        static void fun2() {};
        static int c;
        static int d;
        static int f;
};

class A2
{
    public:
        char a;
        int b;
};


class B2:A2
{
    public:
        short a;
        long b;
};

class C2
{
    A2 a;
    char c;
};

class A3
{
    virtual void fun(){}
};
class C3:public A3
{
};

class A4
{
    virtual void fun() {}
};
class B4
{
    virtual void fun2() {}
};
class C4 : virtual public  A4, virtual public B4
{
    public:
        virtual void fun3() {}
};
class C5 : virtual public  A4, virtual public B4
{
    public:
        void fun3() {}
};



int main()
{
    cout<<sizeof(A)<<endl; //1
    // b+vbptr
    cout<<sizeof(A1)<<endl; //16

    cout<<sizeof(A2)<<endl; // 8
    cout<<sizeof(B2)<<endl; // 16 或 24
    cout<<sizeof(C2)<<endl; // 12
    cout<<sizeof(C3)<<endl; // 8
    //在多继承中，虚表指针的数量等于包含虚函数的直接基类数量，而派生类自身的虚函数会被合并到第一个基类的虚表中
    cout<<sizeof(A4)<<" "<<sizeof(B4)<<" "<<sizeof(C4)<< " "<<sizeof(C5)<<endl;
    return 0;
}