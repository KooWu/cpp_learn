#include "string_wrapper.h"
#include <iostream>

int main() {
    std::cout << "pimpl main" << std::endl;
    StringWrapper test;
    std::cout << test.length() << std::endl;

    StringWrapper test1("yxw");
    std::cout << test1.length() << std::endl;
    std::cout << "pimpl done" << std::endl;
    return 0;
}