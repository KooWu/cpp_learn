#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <spdlog/fmt/chrono.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/ranges.h>
#include "spdlog/spdlog.h"

extern std::shared_ptr<spdlog::logger> g_logger;

#define CVI_LOG( _LEVEL_, ...) \
    do { \
        if (g_logger != nullptr) { \
            SPDLOG_LOGGER_CALL(g_logger, _LEVEL_, __VA_ARGS__); \
        } \
    } while(0)

#define CVI_TRACE(...) CVI_LOG(spdlog::level::trace, ##__VA_ARGS__)
#define CVI_DEBUG(...) CVI_LOG(spdlog::level::debug, ##__VA_ARGS__)
#define CVI_INFO(...) CVI_LOG(spdlog::level::info, ##__VA_ARGS__)
#define CVI_WARN(...) CVI_LOG(spdlog::level::warn, ##__VA_ARGS__)
#define CVI_NOTICE(...) CVI_LOG(spdlog::level::notice, ##__VA_ARGS__)
#define CVI_ERROR(...) CVI_LOG(spdlog::level::err, ##__VA_ARGS__)
#define CVI_FATAL(...) CVI_LOG(spdlog::level::critical, ##__VA_ARGS__)

void CviLogInit(const std::string& appName) noexcept;
void CviLogSetConsoleLevel(spdlog::level::level_enum level) noexcept;
void CviLogSetFileLevel(spdlog::level::level_enum level) noexcept;
