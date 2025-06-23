#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <cassert>
#include "normal_timer.h"

int64_t GetCurrentTimeUs() {
    using namespace std::chrono;
    // 使用steady_clock保证单调性
    auto now = steady_clock::now();
    return duration_cast<microseconds>(now.time_since_epoch()).count();
}

void test_reset_functions() {
    std::cout << "\n=== Reset Functions Test ===\n";
    
    int timer_id = 100;
    // 测试基本重置
    int64_t now_us = GetCurrentTimeUs();
    std::cout << "start current time: " << now_us << std::endl;
    NormalTimer::Instance().StartTimer(
        timer_id,
        2000, // 1秒
        [](int32_t timerId, void* data) {
            int64_t now_us = GetCurrentTimeUs();
            std::cout << "Timer callback called!, " << timerId << ", "<< now_us << std::endl;
        },
        nullptr, // 无需传递额外数据
        true // 是否循环
    );
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    int timer_id1 = 156;
    // 测试基本重置
    NormalTimer::Instance().StartTimer(
        timer_id1,
        1000, // 1秒
        [](int32_t timerId, void* data) {
            int64_t now_us = GetCurrentTimeUs();
            std::cout << "Timer callback called!, " << timerId << ", "<< now_us << std::endl;
        },
        nullptr, // 无需传递额外数据
        true // 是否循环
    );
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    now_us = GetCurrentTimeUs();
    std::cout << "ResetTimer current time: " << now_us << std::endl;
    NormalTimer::Instance().ResetTimer(timer_id, 200, false); // 改为200ms单次
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    auto info = NormalTimer::Instance().GetTimerInfo(timer_id);
    std::cout << "Timer info: " << info.intervalMs << "us, " << info.isRepeat << std::endl;
    std::cout << "timer is active: " << NormalTimer::Instance().TimerIsActive(timer_id) << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    NormalTimer::Instance().StopTimer(timer_id); // 停止定时器
    NormalTimer::Instance().StopTimer(timer_id1); // 停止定时器
    
    std::cout << "Reset functions work correctly!\n";
}

void test_baisc_functions() {
    std::cout << "\n=== Basic Functions Test ===\n";

    int timer_id = 101;
    NormalTimer::Instance().StartTimer(
        timer_id,
        2000, // 2秒
        [](int32_t timerId, void* data) {
            int64_t now_us = GetCurrentTimeUs();
            std::cout << "Timer callback called!, " << timerId << std::endl;
            std::cout << "Timer callback called!, " << ", " << now_us << std::endl;
        },
        nullptr,
        true
    );
    auto info = NormalTimer::Instance().GetTimerInfo(timer_id);
    std::cout << "Timer info: " << info.intervalMs << "us, " << info.isRepeat << std::endl;
    std::cout << "timer is active: " << NormalTimer::Instance().TimerIsActive(timer_id) << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    int64_t now_us = GetCurrentTimeUs();
    std::cout << "StopTimer current time: " << now_us << std::endl;
    NormalTimer::Instance().StopTimer(timer_id);
    std::cout << "timer is active: " << NormalTimer::Instance().TimerIsActive(timer_id) << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
}

void test_many_timer_functions() {
    std::cout << "\n=== test_many_timer_functions Test ===\n";
    
    int timer_id = 103;
    NormalTimer::Instance().StartTimer(
        timer_id,
        1000, // 1秒
        [](int32_t timerId, void* data) {
            int64_t now_us = GetCurrentTimeUs();
            std::cout << "1s Timer callback called!, " << timerId << std::endl;
            std::cout << "1s Timer callback called!, " << ", " << now_us << std::endl;
        },
        nullptr,
        true
    );
    int timer_id11 = 189;
    NormalTimer::Instance().StartTimer(
        timer_id11,
        1000, // 1秒
        [](int32_t timerId, void* data) {
            int64_t now_us = GetCurrentTimeUs();
            std::cout << "1s Timer callback called!, " << timerId << std::endl;
            std::cout << "1s Timer callback called!, " << ", " << now_us << std::endl;
        },
        nullptr,
        true
    );
    int timer_id1 = 104;
    NormalTimer::Instance().StartTimer(
        timer_id1,
        100, // 1秒
        [](int32_t timerId, void* data) {
            int64_t now_us = GetCurrentTimeUs();
            std::cout << "100 ms Timer callback called!, " << timerId << std::endl;
            std::cout << "100 ms  Timer callback called!, " << ", " << now_us << std::endl;
        },
        nullptr,
        true
    );
    int timer_id3 = 105;
    NormalTimer::Instance().StartTimer(
        timer_id3,
        100, // 1秒
        [](int32_t timerId, void* data) {
            int64_t now_us = GetCurrentTimeUs();
            std::cout << "100 ms once Timer callback called!, " << timerId << std::endl;
            std::cout << "100 ms once Timer callback called!, "  << ", " << now_us << std::endl;
        },
        nullptr,
        false
    );
    int timer_id2 = 106;
    NormalTimer::Instance().StartTimer(
        timer_id2,
        2000, // 1秒
        [](int32_t timerId, void* data) {
            int64_t now_us = GetCurrentTimeUs();
            std::cout << "2s Timer callback called!, " << timerId << std::endl;
            std::cout << "2s  Timer callback called!, " << ", " << now_us << std::endl;
        },
        nullptr,
        true
    );

    std::this_thread::sleep_for(std::chrono::seconds(4));
    auto now_us = GetCurrentTimeUs();
    std::cout << "current time: " << now_us << std::endl;
    NormalTimer::Instance().StopTimer(timer_id);
    NormalTimer::Instance().StopTimer(timer_id11);
    NormalTimer::Instance().StopTimer(timer_id2);
}

void test_stop_timer_functions() {
    std::cout << "\n=== test_stop_timer_functions ===\n";
    
    int timer_id = 113;
    NormalTimer::Instance().StartTimer(
        timer_id,
        2000, // 1秒
        [](int32_t timerId, void* data) {
            int64_t now_us = GetCurrentTimeUs();
            std::cout << "1s Timer callback called!, " << timerId << std::endl;
            std::cout << "1s Timer callback called!, " << now_us << std::endl;
            NormalTimer::Instance().StopTimer(timerId);
        },
        nullptr,
        true
    );
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    int timer_id1 = 114;
    NormalTimer::Instance().StartTimer(
        timer_id1,
        1000, // 1秒
        [](int32_t timerId, void* data) {
            int64_t now_us = GetCurrentTimeUs();

            std::cout << "100 ms Timer callback called!, " << timerId << std::endl;
            std::cout << "100 ms  Timer callback called!, " << now_us << std::endl;
            NormalTimer::Instance().StopTimer(timerId);
        },
        nullptr,
        false
    );
    auto info = NormalTimer::Instance().GetTimerInfo(timer_id);
    std::cout << "Timer info: " << info.intervalMs << "us, " << info.isRepeat << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "stop timer "<< std::endl;
    NormalTimer::Instance().StopTimer(timer_id);
    auto now_us = GetCurrentTimeUs();
    std::cout << "current time: " << now_us << std::endl;
}

void test_start_stop_destroy_timer_functions() {
    std::cout << "\n=== test_start_stop_destroy_timer_functions ===\n";
    
    int timer_id = 123;
    NormalTimer::Instance().StartTimer(
        timer_id,
        2000, // 1秒
        [](int32_t timerId, void* data) {
            int64_t now_us = GetCurrentTimeUs();
            std::cout << "2s Timer callback called!, " << timerId << std::endl;
            std::cout << "2s Timer callback called!, " << now_us << std::endl;
        },
        nullptr,
        true
    );
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    NormalTimer::Instance().StopTimer(timer_id);
    std::cout << "stop timer, and wait 5s "<< std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    NormalTimer::Instance().StartTimer(
        timer_id,
        2000, // 1秒
        [](int32_t timerId, void* data) {
            int64_t now_us = GetCurrentTimeUs();
            std::cout << "2s Timer callback called!, " << timerId << std::endl;
            std::cout << "2s Timer callback called!, "  << now_us << std::endl;
        },
        nullptr,
        true
    );
    auto now_us = GetCurrentTimeUs();
    std::cout << "current time: " << now_us << std::endl;
}

int main() {
    test_baisc_functions();
    test_reset_functions();
    test_many_timer_functions();
    // test_stop_timer_functions();
    // test_start_stop_destroy_timer_functions();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}