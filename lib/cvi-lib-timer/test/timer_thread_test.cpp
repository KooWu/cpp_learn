#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <cassert>
#include "accurate_timer.h"

using TimerCallback = std::function<void(int32_t timerId, void*)>;

int64_t GetCurrentTimeUs() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

class Counter  {
    public:
        Counter() : count_(0) {}
        int32_t CreateTimer(int32_t intervalMs, TimerCallback callback, void* userData, bool repeat) {
            int timer_id = AccurateTimer::Instance().CreateTimer(
                intervalMs, // 1秒
                callback,
                userData,
                repeat
            );
            return timer_id;
        }

        bool startTimer(int32_t timerId) {
            return AccurateTimer::Instance().StartTimer(timerId);
        }
        bool resetTimer(int32_t timerId, int32_t newIntervalUs, bool newRepeat) {
            return AccurateTimer::Instance().ResetTimer(timerId, newIntervalUs, newRepeat);
        }
    private:
        int count_;
};

void test_baisc_111functions() {
    std::cout << "\n=== Basic Functions Test ===\n";
    
    std::atomic_int counter{0};
    int timer_id = AccurateTimer::Instance().CreateTimer(
        1000000, // 1秒
        [](int32_t timerId, void* data) {
            int64_t now_us = GetCurrentTimeUs();
            (*static_cast<std::atomic_int*>(data))++;
            std::cout << "Timer callback called!, " << timerId << std::endl;
            std::cout << "111Timer callback called!, " << (*static_cast<std::atomic_int*>(data)) << ", " << now_us << std::endl;
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
    AccurateTimer::Instance().ResetTimer(timer_id, 200000, false); // 改为200ms单次
    std::this_thread::sleep_for(std::chrono::seconds(3));
    now_us = GetCurrentTimeUs();
    std::cout << "current time: " << now_us << std::endl;
}

void test_baisc_functions() {
    Counter counter1;
    std::atomic_int counter{0};
    int timer_id = counter1.CreateTimer(1000000, [](int32_t timerId, void* data) { 
        int64_t now_us = GetCurrentTimeUs();
        (*static_cast<std::atomic_int*>(data))++;
        std::cout << "Timer callback called!, " << timerId << std::endl;
        std::cout << "Timer callback called!, " << (*static_cast<std::atomic_int*>(data)) << ", " << now_us << std::endl;
     },
    &counter, true);
    counter1.startTimer(timer_id);
    std::this_thread::sleep_for(std::chrono::milliseconds(4000));
};


int main() {
    test_baisc_functions();
    test_baisc_111functions();
    while(1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return 0;
}