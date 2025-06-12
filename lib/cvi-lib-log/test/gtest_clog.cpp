// log_test.cpp
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include <fstream>
#include <filesystem>
#include "log_service.h"
#include "MWPLog.h"

// 全局测试环境
class LogTestEnvironment : public ::testing::Environment {
public:
    void SetUp() override {
        // 全局只初始化一次
        log_system_init("test_log_app");
    }
};

// 测试夹具
class LogTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 每个测试前重置日志级别
        log_system_reload_config(level_enum::spd_info);
        CviLogSetFileLevel(spdlog::level::info);
    }

    void TearDown() override {
        cleanLogFiles();
    }

    // 读取日志文件内容
    std::string readLogFile(const std::string& appName) {
        std::string logFile = "/tmp/" + appName + ".log";
        std::ifstream file(logFile);
        return std::string(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>()
        );
    }

    // 检查日志内容是否包含指定文本
    bool logContains(const std::string& content, const std::string& text) {
        return content.find(text) != std::string::npos;
    }

    void cleanLogFiles() {
        for (const auto& entry : std::filesystem::directory_iterator("/tmp/")) {
            if (entry.path().extension() == ".log") {
                std::filesystem::remove(entry.path());
            }
        }
    }
};

// 测试控制台日志级别设置
TEST_F(LogTest, ConsoleLogLevel) {
    EXPECT_NO_THROW(log_system_reload_config(level_enum::spd_trace));
    EXPECT_NO_THROW(log_system_reload_config(level_enum::spd_debug));
    EXPECT_NO_THROW(log_system_reload_config(level_enum::spd_info));
    EXPECT_NO_THROW(log_system_reload_config(level_enum::spd_warn));
    EXPECT_NO_THROW(log_system_reload_config(level_enum::spd_err));
    EXPECT_NO_THROW(log_system_reload_config(level_enum::spd_critical));
    EXPECT_NO_THROW(log_system_reload_config(level_enum::spd_off));
}

// 测试文件日志级别设置
TEST_F(LogTest, SetGetLogLevel) {
    // 测试各个级别设置
    // 测试设置日志级别
    EXPECT_EQ(0, log_system_set_console_level("test_log_app", level_enum::spd_info));
    
    // 测试获取日志级别
    EXPECT_EQ(level_enum::spd_info, log_system_get_console_level("test_log_app"));
    
    // 测试无效参数
    EXPECT_EQ(-1, log_system_set_console_level("no_test_app", level_enum::spd_info));
    EXPECT_EQ(level_enum::spd_info, log_system_get_console_level("no_test_app"));
}

// 测试日志输出
TEST_F(LogTest, LogOutput) {
    CviLogSetFileLevel(spdlog::level::trace);
    
    // 测试各级别日志输出
    MWP_TRACE("Trace message");
    MWP_DEBUG("Debug message");
    MWP_INFO("Info message");
    MWP_WARN("Warning message");
    MWP_ERROR("Error message");
    MWP_FATAL("Fatal message");
    
    std::string logContent = readLogFile("test_log_app");
    
    EXPECT_TRUE(logContains(logContent, "Trace message"));
    EXPECT_TRUE(logContains(logContent, "Debug message"));
    EXPECT_TRUE(logContains(logContent, "Info message"));
    EXPECT_TRUE(logContains(logContent, "Warning message"));
    EXPECT_TRUE(logContains(logContent, "Error message"));
    EXPECT_TRUE(logContains(logContent, "Fatal message"));
    EXPECT_TRUE(logContains(logContent, "gtest_clog.cpp"));
}

// 测试日志级别过滤
TEST_F(LogTest, LogLevelFiltering) {
    CviLogSetFileLevel(spdlog::level::warn);
    
    MWP_TRACE("Trace message");
    MWP_DEBUG("Debug message");
    MWP_INFO("Info message");
    MWP_WARN("Warning message");
    MWP_ERROR("Error message");
    MWP_FATAL("Fatal message");
    
    std::string logContent = readLogFile("test_log_app");
    
    EXPECT_FALSE(logContains(logContent, "Trace message"));
    EXPECT_FALSE(logContains(logContent, "Debug message"));
    EXPECT_FALSE(logContains(logContent, "Info message"));
    EXPECT_TRUE(logContains(logContent, "Warning message"));
    EXPECT_TRUE(logContains(logContent, "Error message"));
    EXPECT_TRUE(logContains(logContent, "Fatal message"));
}

// 测试格式化日志
TEST_F(LogTest, FormattedLog) {
    CviLogSetFileLevel(spdlog::level::info);
    
    MWP_INFO("Integer: %d", 42);
    MWP_INFO("Float: %.2f", 3.14159);
    MWP_INFO("String: %s", "test");
    MWP_INFO("Multiple: %s %s", "hello", "world");
    
    std::string logContent = readLogFile("test_log_app");
    
    EXPECT_TRUE(logContains(logContent, "Integer: 42"));
    EXPECT_TRUE(logContains(logContent, "Float: 3.14"));
    EXPECT_TRUE(logContains(logContent, "String: test"));
    EXPECT_TRUE(logContains(logContent, "Multiple: hello world"));
}

// 测试并发日志
TEST_F(LogTest, ConcurrentLogging) {
    CviLogSetFileLevel(spdlog::level::info);
    
    const int threadCount = 10;
    const int logsPerThread = 100;
    std::vector<std::thread> threads;
    
    for (int i = 0; i < threadCount; ++i) {
        threads.emplace_back([i, logsPerThread]() {
            for (int j = 0; j < logsPerThread; ++j) {
                MWP_INFO("Thread %d Log %d", i, j);
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::string logContent = readLogFile("test_log_app");
    int logCount = 0;
    size_t pos = 0;
    std::string searchStr = "Thread";
    while ((pos = logContent.find(searchStr, pos)) != std::string::npos) {
        ++logCount;
        pos += searchStr.length();
    }
    
    EXPECT_EQ(logCount, threadCount * logsPerThread);
}

// 测试大量日志
TEST_F(LogTest, LargeVolumeLogging) {
    CviLogSetFileLevel(spdlog::level::info);
    
    const int logCount = 10000;
    
    for (int i = 0; i < logCount; ++i) {
        MWP_INFO("Log message %d", i);
    }
    
    std::filesystem::path logPath("/tmp/test_log_app.log");
    EXPECT_TRUE(std::filesystem::exists(logPath));
    EXPECT_GT(std::filesystem::file_size(logPath), 0);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    
    // 添加全局测试环境
    testing::AddGlobalTestEnvironment(new LogTestEnvironment);
    
    return RUN_ALL_TESTS();
}