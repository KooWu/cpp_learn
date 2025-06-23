#ifndef NORMAL_TIMER_H
#define NORMAL_TIMER_H

#include <functional>
#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <unordered_map>
#include <thread>
#include <cstdint>

class NormalTimer {
public:
    using TimerCallback = std::function<void(int32_t timerId, void*)>;

    struct TimerInfo {
        bool isActive;
        bool isRepeat;
        int32_t intervalMs;
        uint64_t triggerCount;
    };
    static NormalTimer& Instance();

    bool StartTimer(int32_t timerId, int32_t intervalMs, NormalTimer::TimerCallback callback, void* userData, bool repeat);
    bool StopTimer(int32_t timerId);
    bool ResetTimer(int32_t timerId, int32_t newIntervalMs, bool newRepeat);
    TimerInfo GetTimerInfo(int32_t timerId) const;
    uint64_t GetTimerTriggerCount(int32_t timerId) const;
    bool TimerIsActive(int32_t timerId) const;

private:
    struct TimerTask {
        int32_t id;
        TimerCallback callback;
        void* userData;
        int32_t intervalMs;
        std::chrono::steady_clock::time_point nextTriggerMs;
        bool active;
        bool repeat;
        uint64_t triggerCount;
        
        bool operator<(const TimerTask& other) const {
            return nextTriggerMs > other.nextTriggerMs;
        }
    };

    NormalTimer();
    ~NormalTimer();
    void workerThread();

    std::atomic<bool> running_{true};
    std::thread worker_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::priority_queue<TimerTask> queue_;
    std::unordered_map<int32_t, TimerTask> timers_;
};

#endif