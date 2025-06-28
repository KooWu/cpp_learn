#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/udp_sink.h"
#include "spdlog/async.h"
#include "log_service.h"
#include "log_cfg.h"
#include "log_cvi_sink.h"
#include "cached_pid_formatter.h"
#include <iostream>
#include <cstdarg> // 添加包含头文件
#include "MWPLog.h"
#include <atomic>

std::shared_ptr<spdlog::logger> g_logger = nullptr;
static spdlog::sink_ptr g_consoleSink = nullptr;
static spdlog::sink_ptr g_fileSink = nullptr;
static std::atomic<bool> g_isInit = false;

void CviLogInit(const std::string &appName) noexcept {
    if (g_isInit.load(std::memory_order_relaxed)) {
        return;
    }
    auto cfg = cvi::log::LoadLogCfg(appName);
    
    std::vector<spdlog::sink_ptr> sinks;

    if (cfg.app.enableConsole) {
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        sinks.push_back(consoleSink);
        consoleSink->set_level(to_spdlog_level(cfg.app.consoleLevel));
        g_consoleSink = consoleSink;
    }

    if (cfg.app.enableFile) {
        auto rotatingSink = std::make_shared<cvi::log::CviFileSink>(cfg.logFile.fileName,
                                                                    cfg.logFile.fileSize,
                                                                    cfg.logFile.fileNum);
        sinks.push_back(rotatingSink);
        rotatingSink->set_level(to_spdlog_level(cfg.app.fileLevel));
        g_fileSink = rotatingSink;
    }

    g_logger = std::make_shared<spdlog::logger>(cfg.app.name, sinks.begin(), sinks.end());
    g_logger->set_level(to_spdlog_level(cfg.logFile.level));
    spdlog::register_logger(g_logger);

    // 创建自定义格式化器
    auto formatter = std::make_unique<spdlog::pattern_formatter>();
    formatter->add_flag<spdlog::custom_formatters::cached_pid_formatter>('p');
    formatter->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n:%p:%t] [%s:%#] [%^%l%$] %v");
    g_logger->set_formatter(std::move(formatter));
    // g_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n:%P:%t] [%s:%#] [%^%l%$] %v");
    g_isInit.store(true, std::memory_order_relaxed);
}

void CviLogSetConsoleLevel(spdlog::level::level_enum level) noexcept {
    if (g_logger == nullptr) {
        return;
    }
    for (auto& sink : g_logger->sinks()) {
        if (g_consoleSink == sink) {
            g_consoleSink->set_level(level);
        }
    }
}

void CviLogSetFileLevel(spdlog::level::level_enum level) noexcept {
    if (g_logger == nullptr) {
        return;
    }
    for (auto& sink : g_logger->sinks()) {
        if (g_fileSink == sink) {
            g_fileSink->set_level(level);
        }
    }
}
extern "C" void log_system_init(const char* appName) {
    if (appName != nullptr) {
        CviLogInit(std::string(appName));
    }
}

extern "C" void log_output(enum level_enum level, const char *file, int line, const char *func, const char *format, ...) {
    if (g_logger != nullptr) {
        const size_t max_log_length = 1024; // 最大允许日志长度
        char buffer[max_log_length];
        va_list args; // 定义一个va_list类型的变量args
        va_start(args, format); // 初始化args，使其指向可变参数列表的第一个参数
        size_t cvi_local_ret = vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args); // 清理args
        if (cvi_local_ret < 0) {
            printf("Failed to format log message: %ld, %d, %s", cvi_local_ret, line, file);
            return;
        }

        size_t actual_length = static_cast<size_t>(cvi_local_ret);

        // 截断超过日志长度（含终止符）
        if (actual_length >= max_log_length) {
            printf("truncated log\n");
            buffer[max_log_length - 1] = '\0';
            actual_length = max_log_length - 1;
            
            // 添加截断标识（需确保缓冲区足够）
            constexpr const char* trunc_mark = "...[TRUNCATED]";
            const size_t mark_len = strlen(trunc_mark);
            if (max_log_length > mark_len) {
                // 使用snprintf自动处理结束符
                snprintf(buffer + max_log_length - mark_len - 1, mark_len + 1, "%s", trunc_mark);
            } else if (max_log_length > 0) {
                // 空间不足时，至少保留结束符
                buffer[max_log_length - 1] = '\0';
            }
        }
        auto spdLevel = static_cast<spdlog::level::level_enum>(level);
        // SPDLOG_LOGGER_CALL(g_logger, spdLevel, "{}", buffer);
        g_logger->log(spdlog::source_loc{file, line, func}, spdLevel, buffer);
    }
}

extern "C" void log_system_reload_config(enum level_enum level) {
    if (g_logger == nullptr) {
        return;
    }
    for (auto& sink : g_logger->sinks()) {
        if (g_consoleSink == sink) {
            g_consoleSink->set_level(static_cast<spdlog::level::level_enum>(level));
        }
    }
}