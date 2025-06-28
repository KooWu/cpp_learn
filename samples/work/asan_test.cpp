#include <iostream>
#include <cstdlib>
#include <cstring>

int main(int argc, char **argv) {
    std::cout << "Hello, ASan" << std::endl; 
    char *buftest = (char *)malloc(10);
    strcpy(buftest, "Hello, ASan!");
    free(buftest);
    return 0;
}