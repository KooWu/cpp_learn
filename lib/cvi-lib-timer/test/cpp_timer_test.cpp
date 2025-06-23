#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <cassert>
#include "accurate_timer.h"

int64_t GetCurrentTimeUs() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

void test_reset_functions() {
    std::cout << "\n=== Reset Functions Test ===\n";
    
    std::atomic_int counter{0};
    int timer_id = AccurateTimer::Instance().CreateTimer(
        1000000, // 1秒
        [](int32_t timerId, void* data) { 
            (*static_cast<std::atomic_int*>(data))++; 
            std::cout << "Timer callback called!, " << timerId << std::endl;
        },
        &counter,
        true
    );

    // 测试基本重置
    AccurateTimer::Instance().StartTimer(timer_id);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    AccurateTimer::Instance().ResetTimer(timer_id, 500000, true); // 重置但不修改参数
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // 测试带参数重置
    AccurateTimer::Instance().ResetTimer(timer_id, 200000, false); // 改为200ms单次
    auto info = AccurateTimer::Instance().GetTimerInfo(timer_id);
    assert(info.intervalUs == 200000);
    assert(info.isRepeat == false);
    std::cout << "Timer info: " << info.intervalUs << "us, " << info.isRepeat << std::endl;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    assert(counter == 2); // 200ms后触发
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    assert(counter == 2); // 单次触发后不再触发
    
    std::cout << "Reset functions work correctly!\n";
}

void test_baisc_functions() {
    std::cout << "\n=== Basic Functions Test ===\n";
    
    std::atomic_int counter{0};
    int timer_id = AccurateTimer::Instance().CreateTimer(
        1000000, // 1秒
        [](int32_t timerId, void* data) {
            int64_t now_us = GetCurrentTimeUs();
            (*static_cast<std::atomic_int*>(data))++;
            std::cout << "Timer callback called!, " << timerId << std::endl;
            std::cout << "Timer callback called!, " << (*static_cast<std::atomic_int*>(data)) << ", " << now_us << std::endl;
        },
        &counter,
        true
    );
    auto info = AccurateTimer::Instance().GetTimerInfo(timer_id);
    std::cout << "Timer info: " << info.intervalUs << "us, " << info.isRepeat << std::endl;

    // 测试基本重置
    AccurateTimer::Instance().StartTimer(timer_id);
    std::this_thread::sleep_for(std::chrono::seconds(4));
    int64_t now_us = GetCurrentTimeUs();
    std::cout << "current time: " << now_us << std::endl;
    AccurateTimer::Instance().ResetTimer(timer_id, 800000, false); // 改为200ms单次
    std::this_thread::sleep_for(std::chrono::seconds(3));
    now_us = GetCurrentTimeUs();
    std::cout << "current time: " << now_us << std::endl;
}


int main() {
    test_baisc_functions();
    test_reset_functions();
    return 0;
}