#include <iostream>
#include <libunwind.h>
#include <cxxabi.h>

// 打印调用栈信息
void print_backtrace() {
    unw_cursor_t cursor;
    unw_context_t context;
    
    // 获取当前上下文
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);
    
    int depth = 0;
    char buffer[512];
    unw_word_t ip, sp;
    
    std::cout << "\n调用栈信息:\n" << std::endl;
    
    // 遍历调用栈
    while (unw_step(&cursor) > 0) {
        unw_get_reg(&cursor, UNW_REG_IP, &ip);
        unw_get_reg(&cursor, UNW_REG_SP, &sp);
        
        // 获取函数名
        unw_get_proc_name(&cursor, buffer, sizeof(buffer), NULL);
        
        // 解码 C++ 函数名（如果需要）
        char* demangled = abi::__cxa_demangle(buffer, NULL, NULL, NULL);
        const char* func_name = demangled ? demangled : buffer;
        
        std::cout << "#" << depth++ << " 0x" << std::hex << ip 
                  << " sp=0x" << sp << " " << std::dec << func_name << std::endl;
        
        if (demangled) free(demangled);
    }
    
    std::cout << std::endl;
}

// 测试函数
void func3() {
    std::cout << "进入 func3()" << std::endl;
    print_backtrace();
    std::cout << "离开 func3()" << std::endl;
}

void func2() {
    std::cout << "进入 func2()" << std::endl;
    func3();
    std::cout << "离开 func2()" << std::endl;
}

void func1() {
    std::cout << "进入 func1()" << std::endl;
    func2();
    std::cout << "离开 func1()" << std::endl;
}

int main() {
    std::cout << "程序开始" << std::endl;
    func1();
    std::cout << "程序结束" << std::endl;
    return 0;
}
