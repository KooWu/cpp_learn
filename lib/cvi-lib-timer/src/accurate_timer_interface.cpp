#include "accurate_timer_interface.h"
#include "accurate_timer.h"

namespace {
    AccurateTimer::TimerCallback wrap_callback(TimerCallback cb, void* data) {
        return [cb](int32_t timerId, void* d) { if(cb) cb(timerId, d); };
    }
}

bool accurate_timer_create(int32_t intervalUs, TimerCallback callback, void* userData, bool repeat) {
    return AccurateTimer::Instance().CreateTimer(
        intervalUs,
        wrap_callback(callback, userData),
        userData,
        repeat
    );
}

bool accurate_timer_destroy(int32_t timerId) {
    return AccurateTimer::Instance().DestroyTimer(timerId);
}

bool accurate_timer_start(int32_t timerId) {
    return AccurateTimer::Instance().StartTimer(timerId);
}

bool accurate_timer_stop(int32_t timerId) {
    return AccurateTimer::Instance().StopTimer(timerId);
}

bool accurate_timer_reset(int32_t timerId, int32_t newIntervalUs, bool newRepeat) {
    return AccurateTimer::Instance().ResetTimer(timerId, newIntervalUs, newRepeat);
}

bool accurate_timer_is_active(int32_t timerId) {
    return AccurateTimer::Instance().TimerIsActive(timerId);
}

AccurateTimerInfo accurate_timer_get_info(int32_t timerId) {
    try {
        auto info = AccurateTimer::Instance().GetTimerInfo(timerId);
        return {
            info.isActive,
            info.isRepeat,
            info.intervalUs,
            info.triggerCount,
        };
    } catch(...) {
        return {0};
    }
}

uint64_t accurate_timer_get_trigger_count(int32_t timerId) {
    try {
        return AccurateTimer::Instance().GetTriggerCount(timerId);
    } catch(...) {
        return 0;
    }
}