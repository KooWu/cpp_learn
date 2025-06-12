#pragma once

#include <cstdint>
#include <string>
#include "spdlog/spdlog.h"

namespace cvi::log {

const char* const LOG_CFG_FILE_NAME_ENV = "LOG_CFG_FILE_NAME_ENV";
const char* const LOG_CFG_FILE_NAME_DEFAULT = "/etc/log_cfg.toml";

enum class level_enum : int {
    trace = SPDLOG_LEVEL_TRACE,
    debug = SPDLOG_LEVEL_DEBUG,
    info = SPDLOG_LEVEL_INFO,
    warn = SPDLOG_LEVEL_WARN,
    err = SPDLOG_LEVEL_ERROR,
    critical = SPDLOG_LEVEL_CRITICAL,
    off = SPDLOG_LEVEL_OFF,
    n_levels
};

// 定义单个应用配置的结构体
struct AppConfig {
    std::string name;
    level_enum consoleLevel = level_enum::info;
    level_enum fileLevel = level_enum::trace;
    bool enableConsole = true;
    bool enableFile = true;
};

// 定义日志文件配置的结构体
struct LogFileConfig {
    std::string fileName = "/tmp/usb.log";
    int fileSize = (10*1024*1024);
    int fileNum = 3;
};

// 定义整个配置的结构体
struct LogConfig {
    LogFileConfig logFile;
    AppConfig app;
};

// 定义整个配置的结构体
struct LogConfigsFile {
    LogFileConfig logFile;
    std::vector<AppConfig> apps;
};

// 转换函数，将 level_enum 转换为 spdlog::level::level_enum
spdlog::level::level_enum to_spdlog_level(level_enum lvl);
LogConfig LoadLogCfg(const std::string &appName) noexcept;

}