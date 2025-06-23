#include "normal_timer_interface.h"
#include "normal_timer.h"

namespace {
    NormalTimer::TimerCallback wrap_callback(TimerCallback cb, void* data) {
        return [cb](int32_t timerId, void* d) { if(cb) cb(timerId, d); };
    }
}

bool normal_timer_start(int32_t timerId, int32_t intervalUs, TimerCallback callback, void* userData, bool repeat) {
    return NormalTimer::Instance().StartTimer(timerId, intervalUs, wrap_callback(callback, userData), userData, repeat);
}

bool normal_timer_stop(int32_t timerId) {
    return NormalTimer::Instance().StopTimer(timerId);
}

bool normal_timer_reset(int32_t timerId, int32_t newIntervalUs, bool newRepeat) {
    return NormalTimer::Instance().ResetTimer(timerId, newIntervalUs, newRepeat);
}

NormalTimerInfo normal_timer_get_info(int32_t timerId) {
    try {
        auto info = NormalTimer::Instance().GetTimerInfo(timerId);
        return {
            info.isActive,
            info.isRepeat,
            info.intervalMs,
            info.triggerCount,
        };
    } catch(...) {
        return {0};
    }
}

uint64_t normal_timer_get_trigger_count(int32_t timerId) {
    try {
        return NormalTimer::Instance().GetTimerTriggerCount(timerId);
    } catch(...) {
        return 0;
    }
}

bool normal_timer_is_active(int32_t timerId) {
    return NormalTimer::Instance().TimerIsActive(timerId);
}