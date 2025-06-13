#include <nameof.hpp>
#include <iostream>

enum class Color { RED, GREEN, BLUE };

int main() {
    int x = 10;
    std::cout << "The value of x is " << x << std::endl;
    std::cout << "The type of x is " << NAMEOF(x) << std::endl;
    auto color = Color::RED;
    std::cout << nameof::nameof_enum(color) << std::endl; // 'RED'
    std::cout << NAMEOF_ENUM(color) << std::endl; // 'RED'
    std::cout << nameof::nameof_enum<Color::GREEN>() << std::endl; // 'GREEN'
    return 0;
}