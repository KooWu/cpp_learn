#ifndef NORMAL_TIMER_INTERFACE_H
#define NORMAL_TIMER_INTERFACE_H

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
} NormalTimerInfo;


bool normal_timer_start(int32_t timerId, int32_t intervalUs, TimerCallback callback, void* userData, bool repeat);
bool normal_timer_stop(int32_t timerId);
bool normal_timer_reset(int32_t timerId, int32_t newIntervalUs, bool newRepeat);
NormalTimerInfo normal_timer_get_info(int32_t timerId);
uint64_t normal_timer_get_trigger_count(int32_t timerId);
bool normal_timer_is_active(int32_t timerId);

#ifdef __cplusplus
}
#endif

#endif