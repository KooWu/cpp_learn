#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char **argv) {
    std::cout << "hello world" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return 0;
}