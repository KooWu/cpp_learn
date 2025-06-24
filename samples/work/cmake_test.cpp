#include <iostream>

int main() {
    #ifdef MY_VARIABLE
        std::cout << "MY_VARIABLE is defined as: " << MY_VARIABLE << std::endl;
    #else
        std::cout << "MY_VARIABLE is not defined." << std::endl;
    #endif
    return 0;
}