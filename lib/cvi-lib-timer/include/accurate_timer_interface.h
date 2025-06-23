#ifndef ACCURATE_TIMER_INTERFACE_H
#define ACCURATE_TIMER_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*TimerCallback)(int32_t timerId, void* userData);

typedef struct {
    bool isActive;
    bool isRepeat;
    int32_t intervalUs;
    uint64_t triggerCount;
} AccurateTimerInfo;

bool accurate_timer_create(int32_t intervalUs, TimerCallback callback, void* userData, bool repeat);
bool accurate_timer_destroy(int32_t timerId);
bool accurate_timer_start(int32_t timerId);
bool accurate_timer_stop(int32_t timerId);
bool accurate_timer_reset(int32_t timerId, int32_t newIntervalUs, bool newRepeat);
bool accurate_timer_is_active(int32_t timerId);
AccurateTimerInfo accurate_timer_get_info(int32_t timerId);
uint64_t accurate_timer_get_trigger_count(int32_t timerId);

#ifdef __cplusplus
}
#endif

#endif