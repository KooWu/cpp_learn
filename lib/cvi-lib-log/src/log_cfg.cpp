#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <toml++/toml.hpp>
#include "log_cfg.h"

namespace cvi::log
{

// 转换函数，将 level_enum 转换为 spdlog::level::level_enum
spdlog::level::level_enum to_spdlog_level(level_enum lvl) {
    switch (lvl) {
        case level_enum::trace: return spdlog::level::trace;
        case level_enum::debug: return spdlog::level::debug;
        case level_enum::info: return spdlog::level::info;
        case level_enum::warn: return spdlog::level::warn;
        case level_enum::err: return spdlog::level::err;
        case level_enum::critical: return spdlog::level::critical;
        case level_enum::off: return spdlog::level::off;
        default: throw std::invalid_argument("Invalid level_enum value");
    }
}

level_enum string_level_to_level(const std::string& level) {
    if (level == "trace") {
        return level_enum::trace;
    } else if (level == "debug") {
        return level_enum::debug;
    } else if (level == "info") {
        return level_enum::info;
    } else if (level == "warn") {
        return level_enum::warn;
    } else if (level == "err") {
        return level_enum::err;
    } else if (level == "critical") {
        return level_enum::critical;
    } else if (level == "off") {
        return level_enum::off;
    } else {
        return level_enum::info;
    }
}

std::string getLogConfigFilePath() {
    const char *envPath = std::getenv(LOG_CFG_FILE_NAME_ENV);
    if (envPath == nullptr) {
        std::cout << "will use default log config file: " << LOG_CFG_FILE_NAME_DEFAULT << std::endl;
        return std::string(LOG_CFG_FILE_NAME_DEFAULT);
    } else {
        std::cout << "will use log config file from env: " << envPath << std::endl;
        return std::string(envPath);
    }
}

static void PrintConfig(const LogConfig& cfg) {
    std::cout << "=== Log File Config ===" << std::endl;
    std::cout << "File Name: " << cfg.logFile.fileName << std::endl;
    std::cout << "File Size: " << cfg.logFile.fileSize << " KB" << std::endl;
    std::cout << "File Num: " << cfg.logFile.fileNum << std::endl;
    std::cout << "Level: " << static_cast<int>(cfg.logFile.level) << std::endl;

    std::cout << "\n=== Application Log Config ===" << std::endl;
    std::cout << "Console Level: " 
              << static_cast<int>(cfg.app.consoleLevel) << std::endl;
    std::cout << "File Level: " 
              << static_cast<int>(cfg.app.fileLevel) <<  std::endl;
    std::cout << "Enable Console: " << std::boolalpha << cfg.app.enableConsole << std::endl;
    std::cout << "Enable File: " << std::boolalpha << cfg.app.enableFile << std::endl;
}

LogConfig LoadLogCfg(const std::string &appName) noexcept {
    LogConfig cfg;
    cfg.app.name = appName;
    std::string cfgPath = getLogConfigFilePath();
    std::ifstream configFile(cfgPath);
    if (!configFile.is_open()) {
        std::cerr << "Failed to open config file." << std::endl;
        return cfg;
    }
    configFile.close();

    // 解析TOML文件
    auto config = toml::parse_file(cfgPath);
    // 解析日志文件配置
    const auto& logFile = config["logFile"];
    // 解析应用配置
    const auto& appsArray = config["apps"].as_array();
    for (const auto& entry : *appsArray) {
        const auto& app = *entry.as_table();
        std::string curName = app["name"].value_or("");
        if (curName == appName) {
            cfg.logFile.fileName = logFile["fileName"].value_or("/tmp/tbox.log");
            cfg.logFile.fileSize = logFile["fileSize"].value_or(102400);
            cfg.logFile.fileNum = logFile["fileNum"].value_or(3);
            cfg.logFile.level = string_level_to_level(logFile["level"].value_or("trace"));
            cfg.app.consoleLevel = string_level_to_level(app["consoleLevel"].value_or("info"));
            cfg.app.fileLevel = string_level_to_level(app["fileLevel"].value_or("trace"));
            cfg.app.enableConsole = app["enableConsole"].value_or(false);
            cfg.app.enableFile = app["enableFile"].value_or(true);
            PrintConfig(cfg);
            break;
        }
    }
    return cfg;
}

}