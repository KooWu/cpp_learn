#ifndef AccurateTimer_H
#define AccurateTimer_H

#include <functional>
#include <memory>
#include <chrono>
#include <cstdint>

class AccurateTimer {
public:
    using TimerCallback = std::function<void(int32_t timerId, void*)>;
    
    struct TimerInfo {
        bool isActive;          // 是否激活状态
        bool isRepeat;       // 是否循环定时器
        int32_t intervalUs;         // 定时间隔（微秒）
        uint64_t triggerCount;  // 已触发次数
        int64_t startTime;      // 最近启动时间戳（微秒）
    };

    static AccurateTimer& Instance();
    
    // 定时器生命周期管理
    int32_t CreateTimer(int32_t intervalUs, TimerCallback callback, void* userData, bool repeat);
    bool DestroyTimer(int32_t timerId);
    
    // 定时器控制
    bool StartTimer(int32_t timerId);
    bool StopTimer(int32_t timerId);
    bool ResetTimer(int32_t timerId, int32_t newIntervalUs, bool newRepeat);
    
    // 状态查询
    TimerInfo GetTimerInfo(int32_t timerId) const;
    uint64_t GetTriggerCount(int32_t timerId) const;
    bool TimerIsActive(int32_t timerId) const;

    AccurateTimer(const AccurateTimer&) = delete;
    AccurateTimer& operator=(const AccurateTimer&) = delete;

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
    
    AccurateTimer();
    ~AccurateTimer();
};

#endif
