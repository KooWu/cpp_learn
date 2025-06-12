#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <array>
#include <map>
#include <vector>
#include <sys/types.h>
#include <signal.h>
#include "log_service.h"
#include "MWPLog.h"

struct Point {
    int x;
    int y;
};

// 为 Point 类型特化 fmt::formatter
template <>
struct fmt::formatter<Point> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.begin();
    }

    // 格式化 Point 对象
    template <typename FormatContext>
    auto format(const Point& p, FormatContext& ctx) const -> decltype(ctx.out()) {
        // 使用解析的格式说明符来决定如何格式化
        return fmt::format_to(ctx.out(), "({},{})", p.x, p.y);
    }
};

enum class Color {
    RED,
    GREEN,
    BLUE
};

void logFunction(int32_t count, int32_t sleepTime, int32_t threadId) {
    for (int i = 0; i <= count; i++) {
        CVI_INFO("Hello from thread {}!", threadId);
        int32_t ret = 32;
        CVI_WARN("Thread {} ret: {}", threadId, ret);
        Point p{1, 2};
        CVI_INFO("Thread {} point: {}", threadId, p);
        Color myColor = Color::BLUE;
        CVI_INFO("Thread {} myColor: {}", threadId, static_cast<int>(myColor));
        CVI_DEBUG("Thread {}: Some error message with arg: {}", threadId, 1);
        CVI_INFO("Thread {}: Some error message with arg: {}", threadId, 1);
        std::array<int, 3> arr = {1, 2, 3};
        CVI_INFO("Thread {}: arr: {}", threadId, arr);
        std::map<std::string, int> my_map = {{"apple", 1}, {"banana", 2}, {"orange", 3}};
        CVI_INFO("Thread {}: my_map: {}", threadId, my_map);
        std::vector<int> vec = {1, 2, 3, 4, 5};
        CVI_INFO("Thread {}: vec: {}", threadId, vec);
        std::string str = "Hello, world!";
        CVI_INFO("Thread {}: str: {}", threadId, str);
            auto now = std::chrono::system_clock::now();
        CVI_INFO("Date and time: {}", now);
        CVI_INFO("Time: {:%H:%M}", now);
        CVI_ERROR("Thread {}: Easy padding in numbers like {:08d}", threadId, 12);
        MWP_TRACE("Thread %d: ret: %d, %s, %f", threadId, ret, "test", 3.1415926)
        MWP_DEBUG("Thread %d: ret: %d, %s, %f", threadId, ret, "test", 3.1415926);
        MWP_INFO("Thread %d: ret: %d, %s, %f", threadId, ret, "test", 3.1415926);
        MWP_WARN("Thread %d: ret: %d, %s, %f", threadId, ret, "test", 3.1415926);
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }
}

void SignalHandlerLogLevel(int32_t signalNum) {
    std::cout << "Caught signal " << signalNum << std::endl;
    if (signalNum == SIGUSR1) {
        CviLogSetConsoleLevel(spdlog::level::debug);
        CviLogSetFileLevel(spdlog::level::warn);
    } else if (signalNum == SIGUSR2) {
        CviLogSetConsoleLevel(spdlog::level::warn);
        CviLogSetFileLevel(spdlog::level::debug);
    }
}

int main(int argc, char* argv[]) {
    struct sigaction sa;

    sa.sa_handler = &SignalHandlerLogLevel;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    // Register the signal handler for SIGUSR1 using sigaction
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    std::string appName = "test_log_app";
    if (argc > 1) {
        appName = argv[1];
    }

    int32_t count = 0;
    if (argc > 2) {
        count = std::atoi(argv[2]);
    }

    int32_t sleepTime = 1000;
    if (argc > 3) {
        sleepTime = std::atoi(argv[3]);
        if (sleepTime < 0 || sleepTime > 10 * 1000) {
            sleepTime = 100;
        }
    }
    log_system_init(appName.c_str());
    // CviLogInit(appName); // 为每个线程初始化日志
    const int threadCount = 5; // 创建5个线程
    std::vector<std::thread> threads;

    for (int i = 0; i < threadCount; i++) {
        threads.emplace_back(logFunction, count, sleepTime, i);
    }

    for (auto& thread : threads) {
        thread.join(); // 等待所有线程完成
    }

    return 0;
}
