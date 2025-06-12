#ifndef CVI_LIB_LOG_INCLUDE_MWPLOG_H_
#define CVI_LIB_LOG_INCLUDE_MWPLOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define SPDLOG_LEVEL_TRACE 0
#define SPDLOG_LEVEL_DEBUG 1
#define SPDLOG_LEVEL_INFO 2
#define SPDLOG_LEVEL_WARN 3
#define SPDLOG_LEVEL_ERROR 4
#define SPDLOG_LEVEL_CRITICAL 5
#define SPDLOG_LEVEL_OFF 6

enum level_enum {
    spd_trace = SPDLOG_LEVEL_TRACE,
    spd_debug = SPDLOG_LEVEL_DEBUG,
    spd_info = SPDLOG_LEVEL_INFO,
    spd_warn = SPDLOG_LEVEL_WARN,
    spd_err = SPDLOG_LEVEL_ERROR,
    spd_critical = SPDLOG_LEVEL_CRITICAL,
    spd_off = SPDLOG_LEVEL_OFF,
    spd_n_levels
};

void log_system_reload_config(enum level_enum level);
void log_system_init(const char *appName);
void log_output(enum level_enum level, const char *file, int line, const char *func, const char *format, ...);

#define LOG(_LEVEL_, format, ...) \
{ \
    log_output(_LEVEL_, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__); \
}

#define MWP_TRACE(format, ...) LOG(spd_trace, format, ##__VA_ARGS__)
#define MWP_DEBUG(format, ...) LOG(spd_debug, format, ##__VA_ARGS__)
#define MWP_INFO(format, ...) LOG(spd_info, format, ##__VA_ARGS__)
#define MWP_WARN(format, ...) LOG(spd_warn, format, ##__VA_ARGS__)
#define MWP_ERROR(format, ...) LOG(spd_err, format, ##__VA_ARGS__)
#define MWP_FATAL(format, ...) LOG(spd_critical, format, ##__VA_ARGS__)

#define MWP_INFO_FUNC() MWP_INFO("%s()", __FUNCTION__)

#define MWP_INFO_FUNC_BEGIN() MWP_INFO("- Begin")

#define MWP_INFO_FUNC_END() MWP_INFO("- End")

#define MWP_INFO_BUILD_TIME() MWP_INFO("Build Time -- %s %s.", __DATE__, __TIME__)

#ifdef __cplusplus
}
#endif

#endif //CVI_LIB_LOG_INCLUDE_MWPLOG_H_
