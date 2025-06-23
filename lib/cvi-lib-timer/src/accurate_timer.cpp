#include "accurate_timer.h"
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <mutex>
#include <unordered_map>
#include <thread>
#include <cstring>
#include <iostream>
#include <atomic>
#include <pthread.h>

class AccurateTimer::Impl {
    static constexpr int32_t MaxEvents = 64;
    static constexpr int32_t EpollTimeout = -1;

    struct TimerContext {
        int32_t fd;                         // timerfd描述符
        int32_t intervalUs;                // 定时间隔（微秒）
        bool repeat;                    // 是否循环
        bool active;       // 激活状态
        AccurateTimer::TimerCallback callback;
        void* userData;                // 用户数据
        uint64_t triggerCount; // 触发计数器
    };

    int32_t epollFd_;
    int32_t wakeFd_;
    int32_t eventFd_;
    std::unordered_map<int32_t, TimerContext> timers_;
    mutable std::mutex mutex_;
    std::atomic<int32_t> idGenerator_{0};
    std::thread eventThread_;
    std::atomic<bool> running_{true};

public:
    Impl() : epollFd_(epoll_create1(0)) {
        // 创建一个管道用于手动触发 epoll_wait 的退出
        int32_t pipefd[2];
        std::ignore = pipe2(pipefd, TFD_NONBLOCK);
        wakeFd_ = pipefd[0];
        eventFd_ = pipefd[1];

        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = wakeFd_;
        std::ignore = epoll_ctl(epollFd_, EPOLL_CTL_ADD, wakeFd_, &ev);
        eventThread_ = std::thread([this]() {
            pthread_setname_np(pthread_self(), "high-precision_timer");
            struct epoll_event events[MaxEvents];
            while(running_) {
                std::cout << "event thread wait" << std::endl;
                int32_t n = epoll_wait(epollFd_, events, MaxEvents, EpollTimeout);
                if(n == -1 && errno != EINTR) break;
                processEvents(events, n);
            }
            std::cout << "event thread exit" << std::endl;
        });
    }

    ~Impl() {
        running_ = false;
        uint64_t val = 1;
        std::ignore = write(eventFd_, &val, sizeof(val));
        {
            std::lock_guard<std::mutex> lock(mutex_);
            for(auto& [id, ctx] : timers_) {
                close(ctx.fd);
            }
        }
        if(eventThread_.joinable()) eventThread_.join();
        close(epollFd_);
        close(wakeFd_);
        close(eventFd_);
    }

    int32_t CreateTimer(int32_t intervalUs, AccurateTimer::TimerCallback callback, void* userData, bool repeat) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        int32_t timerId = ++idGenerator_;
        int32_t fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
        if(fd == -1) {
            std::cerr << "timerfd_create failed" << std::endl;
            return -1;
        }

        timers_.emplace(timerId, TimerContext{
            fd, intervalUs, repeat, false,
            std::move(callback), userData, 0
        });
        return timerId;
    }

    bool DestroyTimer(int32_t timerId) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = timers_.find(timerId);
        if(it == timers_.end()) {
            std::cerr << "destroy timer not found" << std::endl;
            return false;
        }

        stopTimerImpl(it->second);
        close(it->second.fd);
        timers_.erase(it);
        return true;
    }

    bool StartTimer(int32_t timerId) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = timers_.find(timerId);
        if(it == timers_.end()) {
            std::cerr << "start timer not found" << std::endl;
            return false;
        }

        auto& ctx = it->second;
        if(ctx.active) {
            std::cerr << "timer already active" << std::endl;
            return true;
        }
        

        struct itimerspec ts = createTimespec(ctx.intervalUs);
        std::cout << "timerfd_settime ts " << ts.it_value.tv_sec << " " << ts.it_value.tv_nsec << std::endl;
        if(timerfd_settime(ctx.fd, 0, &ts, nullptr) == -1) {
            std::cerr << "timerfd_settime failed" << std::endl;
            return false;
        }

        struct epoll_event event{};
        event.events = EPOLLIN;
        event.data.fd = ctx.fd;
        if(epoll_ctl(epollFd_, EPOLL_CTL_ADD, ctx.fd, &event) == -1) {
            std::cerr << "epoll_ctl failed" << std::endl;
            return false;
        }
        ctx.active = true;
        return true;
    }

    bool StopTimer(int32_t timerId) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = timers_.find(timerId);
        if(it == timers_.end()) {
            std::cerr << "stop timer not found" << std::endl;
            return false;
        }
        return stopTimerImpl(it->second);
    }

    bool ResetTimer(int32_t timerId, int32_t newIntervalUs, bool newRepeat) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = timers_.find(timerId);
        if(it == timers_.end()) {
            std::cerr << "reset timer not found" << std::endl;
            return false;
        }

        auto& ctx = it->second;
        bool wasActive = ctx.active;
        
        // 更新参数
        if(newIntervalUs > 0) ctx.intervalUs = newIntervalUs;
        if(newRepeat != ctx.repeat) ctx.repeat = newRepeat;
        
        if(wasActive) {
            stopTimerImpl(ctx);
            struct itimerspec ts = createTimespec(ctx.intervalUs);
            if(timerfd_settime(ctx.fd, 0, &ts, nullptr) == -1) {
                std::cerr << "timerfd_settime failed" << std::endl;
                return false;
            }

            struct epoll_event event{};
            event.events = EPOLLIN;
            event.data.fd = ctx.fd;
            if(epoll_ctl(epollFd_, EPOLL_CTL_ADD, ctx.fd, &event) == -1) {
                std::cerr << "epoll_ctl failed" << std::endl;
                return false;
            }

            ctx.active = true;
        }
        return true;
    }

    AccurateTimer::TimerInfo GetTimerInfo(int32_t timerId) const {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = timers_.find(timerId);
        if(it == timers_.end()) return {};

        const auto& ctx = it->second;
        TimerInfo info{};
        info.isActive = ctx.active;
        info.isRepeat = ctx.repeat;
        info.intervalUs = ctx.intervalUs;
        info.triggerCount = ctx.triggerCount;
        return info;
    }

    bool TimerIsActive(int32_t timerId) const {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = timers_.find(timerId);
        if(it == timers_.end()) return false;
        return it->second.active;
    }

    uint64_t GetTriggerCount(int32_t timerId) const {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = timers_.find(timerId);
        return it != timers_.end() ? it->second.triggerCount : 0;
    }

private:
    bool stopTimerImpl(TimerContext& ctx) {
        if(!ctx.active) return true;

        if(epoll_ctl(epollFd_, EPOLL_CTL_DEL, ctx.fd, nullptr) == -1) return false;
        
        struct itimerspec disable{};
        if(timerfd_settime(ctx.fd, 0, &disable, nullptr) == -1) return false;
        
        ctx.active = false;
        return true;
    }

    static struct itimerspec createTimespec(int32_t us) {
        struct itimerspec ts{};
        ts.it_value.tv_sec = us / 1000000;
        ts.it_value.tv_nsec = (us % 1000000) * 1000;
        if(ts.it_value.tv_sec == 0 && ts.it_value.tv_nsec == 0) {
            ts.it_value.tv_nsec = 1; // 最小间隔1ns
        }
        ts.it_interval.tv_sec = us / 1000000;
        ts.it_interval.tv_nsec = (us % 1000000) * 1000;
        if(ts.it_interval.tv_sec == 0 && ts.it_interval.tv_nsec == 0) {
            ts.it_interval.tv_nsec = 1; // 最小间隔1ns
        }
        return ts;
    }

    void processEvents(struct epoll_event* events, int32_t count) {
        for(int32_t i = 0; i < count; ++i) {
            std::cout << "process_events" << std::endl;
            int32_t fd = events[i].data.fd;
            uint64_t expirations;
            std::ignore = read(fd, &expirations, sizeof(expirations));

            std::lock_guard<std::mutex> lock(mutex_);
            for(auto& [id, ctx] : timers_) {
                if(ctx.fd == fd && ctx.active) {
                    ctx.triggerCount++;
                    if (ctx.callback != nullptr) {
                        ctx.callback(id, ctx.userData);
                    }
                    if(!ctx.repeat) {
                        std::cout << "stop_timer_impl" << std::endl;
                        stopTimerImpl(ctx);
                    }
                    break;
                }
            }
        }
    }
};

// AccurateTimer接口实现
AccurateTimer& AccurateTimer::Instance() {
    static AccurateTimer instance;
    return instance;
}

AccurateTimer::AccurateTimer() : m_impl(std::make_unique<Impl>()) {}
AccurateTimer::~AccurateTimer() = default;

int32_t AccurateTimer::CreateTimer(int32_t intervalUs, TimerCallback callback, void* userData, bool repeat) {
    return m_impl->CreateTimer(intervalUs, callback, userData, repeat);
}

bool AccurateTimer::DestroyTimer(int32_t timerId) { return m_impl->DestroyTimer(timerId); }
bool AccurateTimer::StartTimer(int32_t timerId) { return m_impl->StartTimer(timerId); }
bool AccurateTimer::StopTimer(int32_t timerId) { return m_impl->StopTimer(timerId); }
bool AccurateTimer::ResetTimer(int32_t timerId, int32_t newIntervalUs, bool newRepeat) {
    return m_impl->ResetTimer(timerId, newIntervalUs, newRepeat);
}
bool AccurateTimer::TimerIsActive(int32_t timerId) const { return m_impl->TimerIsActive(timerId); }
AccurateTimer::TimerInfo AccurateTimer::GetTimerInfo(int32_t timerId) const { 
    return m_impl->GetTimerInfo(timerId); 
}
uint64_t AccurateTimer::GetTriggerCount(int32_t timerId) const { 
    return m_impl->GetTriggerCount(timerId); 
}
