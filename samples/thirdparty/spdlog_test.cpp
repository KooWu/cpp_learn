#include "spdlog/spdlog.h"
#include <iostream>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>

void may_throw() {
    throw std::runtime_error("An error occurred.");
}

int test_async()
{
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
        "logs/async.txt",
        true  // truncate file
    );
    file_sink->set_level(spdlog::level::debug);
    file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");

    auto async_logger = std::make_shared<spdlog::async_logger>(
        "async_logger", 
        file_sink,
        spdlog::thread_pool(),
        spdlog::async_overflow_policy::block
    );

    async_logger->set_level(spdlog::level::debug);
    async_logger->info("Hello, world!");

    return 0;
}

int main() {
    spdlog::set_level(spdlog::level::warn); // 设置全局日志级别
    spdlog::enable_backtrace(2); // 启用backtrace并设置缓存大小为10

    try {
        spdlog::debug("This is a debug message.");
        spdlog::info("Starting operation...");
        may_throw(); // 这里将会抛出异常
    } catch (const std::exception& e) {
        spdlog::error("Caught an exception: {}", e.what());
        spdlog::dump_backtrace(); // 输出缓存的回溯日志
    }


//enable_backtrace(20); 打开 backtrace/dump 功能，参数为 dump个数
    spdlog::enable_backtrace(3); // create ring buffer with capacity of 10  messages
    for (int i = 0; i < 5; i++) {
        //注意为设置输出等级之下的日志
        spdlog::debug("Backtrace message {}", i); // not logged..
    }
    // e.g. if some error happened:
    spdlog::dump_backtrace(); // log them now!
    test_async();
    return 0;
}