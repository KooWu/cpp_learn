#include "normal_timer.h"
#include <chrono>
#include <memory>
#include <pthread.h>
#include <iostream>

NormalTimer& NormalTimer::Instance() {
    static NormalTimer instance;
    return instance;
}

void NormalTimer::workerThread() {
    pthread_setname_np(pthread_self(), "normal_timer");
    using namespace std::chrono;
    
    while(running_) {
        std::unique_lock<std::mutex> lock(mutex_);
        
        if(queue_.empty()) {
            cv_.wait(lock, [this]{ return !running_ || !queue_.empty(); });
            continue;
        }

        const auto& topTask = queue_.top();
        auto waitTime = topTask.nextTriggerMs;
        cv_.wait_until(lock, waitTime);

        if (!running_) break;

        auto now = steady_clock::now();
        std::vector<TimerTask> tasksToExecute;
        
        while(!queue_.empty()) {
            auto& task = const_cast<TimerTask&>(queue_.top());
            if (task.nextTriggerMs > now) {
                break;
            }
            if (!task.active) {
                queue_.pop();
                continue;
            }

            // 将需要执行的任务移出队列并标记为已处理，但不立即执行回调
            tasksToExecute.push_back(task);
            task.triggerCount++;
            // 处理重复任务
            if (task.repeat) {
                auto newTask = task;
                newTask.nextTriggerMs += std::chrono::milliseconds(task.intervalMs);
                queue_.pop();
                queue_.push(newTask);
            } else {
                timers_.erase(task.id);
                queue_.pop();
            }
        }

        // 释放锁后再执行回调
        lock.unlock();

        for (auto& task : tasksToExecute) {
            try {
                task.callback(task.id, task.userData);
            } catch(...) {
                // 异常处理
            }
        }

        // 重新获取锁以处理队列中的其他任务
        lock.lock();
    }
}


NormalTimer::NormalTimer() : running_(true) {
    timers_.clear();
    worker_ = std::thread(&NormalTimer::workerThread, this);
}

NormalTimer::~NormalTimer() {
    running_ = false;
    cv_.notify_all();
    if(worker_.joinable()) worker_.join();
}



bool NormalTimer::StartTimer(int32_t timerId, int32_t intervalMs, NormalTimer::TimerCallback callback, void* userData, bool repeat) {
    std::lock_guard<std::mutex> lock(mutex_);
    TimerTask task{
        timerId,
        callback,
        userData,
        intervalMs,
        std::chrono::steady_clock::now() + std::chrono::milliseconds(static_cast<int64_t>(intervalMs)),
        true,  // 初始激活
        repeat,
        0
    };
    // 检查ID是否已经存在
    if(timers_.find(timerId) != timers_.end()) {
        return false; // ID已存在，创建失败
    }
    timers_.emplace(timerId, task);
    queue_.push(task);
    cv_.notify_one();
    return true;
}

bool NormalTimer::StopTimer(int32_t timerId) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = timers_.find(timerId);
    if (it == timers_.end()) {
        std::cout << "Timer " << timerId << " not found" << std::endl;
        return false;
    }
    it->second.active = false;
    timers_.erase(it);
    // 重建优先队列
    std::priority_queue<TimerTask> newQueue;
    while(!queue_.empty()) {
        auto task = queue_.top();
        if(task.id != timerId) {
            newQueue.push(task);
        }
        queue_.pop();
    }
    queue_ = std::move(newQueue);
    cv_.notify_one();
    return true;
}

bool NormalTimer::ResetTimer(int32_t timerId, int32_t newIntervalMs, bool newRepeat) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = timers_.find(timerId);
    if(it == timers_.end()) return false;

    auto& task = it->second;
    task.intervalMs = newIntervalMs;
    task.repeat = newRepeat;
    auto now = std::chrono::steady_clock::now();
    task.nextTriggerMs = now + std::chrono::milliseconds(newIntervalMs);
    
    // 更新队列
    std::priority_queue<TimerTask> newQueue;
    while(!queue_.empty()) {
        auto t = queue_.top();
        if(t.id == timerId) t = task;
        newQueue.push(t);
        queue_.pop();
    }
    queue_ = std::move(newQueue);
    cv_.notify_one();
    return true;
}

NormalTimer::TimerInfo NormalTimer::GetTimerInfo(int32_t timerId) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = timers_.find(timerId);
    if(it == timers_.end()) return {false, false, 0, 0};

    const auto& task = it->second;
    return {
        task.active,
        task.repeat,
        task.intervalMs,
        task.triggerCount
    };
}

bool NormalTimer::TimerIsActive(int32_t timerId) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = timers_.find(timerId);
    return it != timers_.end() && it->second.active;
}

uint64_t NormalTimer::GetTimerTriggerCount(int32_t timerId) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = timers_.find(timerId);
    return it != timers_.end() ? it->second.triggerCount : 0;
}


