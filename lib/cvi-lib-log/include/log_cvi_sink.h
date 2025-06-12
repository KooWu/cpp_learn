#pragma once

#include <spdlog/common.h>
#include <spdlog/details/os.h>
#include "spdlog/spdlog.h"
#include <spdlog/details/null_mutex.h>
#include <spdlog/details/synchronous_factory.h>
#include "spdlog/sinks/base_sink.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <filesystem>
#include <mutex>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>


namespace cvi {
namespace log {

// 自定义日志沉淀池 (sink)，用于日志存储和轮转
class CviFileSink : public spdlog::sinks::base_sink<spdlog::details::null_mutex> {
public:
    // 构造函数接收日志文件的基本路径、最大文件大小（字节）、最多保存的日志文件个数
    CviFileSink(const std::string& fileName, uint32_t maxSize, uint16_t maxFiles)
        : m_fileName(fileName), m_maxSize(maxSize), m_maxFiles(maxFiles) {
    }

private:
    void sink_it_(const spdlog::details::log_msg& msg) override {
        int32_t fd = open(m_fileName.c_str(), O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
        if (fd < 0) {
            // printf("open file[%s] fail, errno[%d]\n", m_fileName.c_str(), errno);
            return;
        }

        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<spdlog::details::null_mutex>::formatter_->format(msg, formatted);

        uint32_t len = formatted.size();
        if (write(fd, formatted.data(), len) < 0) {
            // printf("write fail, errno[%d]\n", errno);
            return;
        }
        close(fd);

        if (len > m_maxSize) {
            // printf("one msg's len[%ld] > archive_max_size[%ld], no rotate\n", (long)len, (long)m_maxSize);
            return;
        }

        struct stat info;
        if (stat(m_fileName.c_str(), &info) != 0) {
            // printf("stat [%s] fail, errno[%d], maybe in rotating\n", m_fileName.c_str(), errno);
            return;
        }

        if (info.st_size + len < m_maxSize) {
            return;
        }
        RotateLogs();
    }

    void flush_() override {
    }

    void RotateLogs() {
        std::lock_guard<std::mutex> lk(m_writeMutex);
        int32_t lockFd = open("/tmp/log.lock", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
        if (lockFd < 0) {
            // printf("open lock file fail, errno[%d]\n", errno);
            return;
        }

        struct flock fl;
        fl.l_type = F_WRLCK;
        fl.l_start = 0;
        fl.l_whence = SEEK_SET;
        fl.l_len = 0;
        if (fcntl(lockFd, F_SETLK, &fl)) {
            // printf("fcntl fail, errno[%d]\n", errno);
            close(lockFd);
            return;
        }

        do {
            // 重新检查，防止其他进程已经完成了日志轮转
            struct stat info;
            if (stat(m_fileName.c_str(), &info) != 0) {
                // printf("stat [%s] fail, errno[%d], maybe in rotating\n", m_fileName.c_str(), errno);
                break;
            }
            if (info.st_size < m_maxSize) {
                break;
            }

            // log文件名格式：fileName.0, fileName.1, ...
            // fileName.1 -> fileName.2
            // fileName.0 -> fileName.1
            // fileName -> fileName.0
            for (size_t i = m_maxFiles - 1; i > 0; --i) {
                std::string oldName = fmt::format("{}.{}", m_fileName, i - 1);
                std::string newName = fmt::format("{}.{}", m_fileName, i);
                if (std::filesystem::exists(oldName)) {
                    std::filesystem::rename(oldName, newName);
                }
            }
            if (std::filesystem::exists(m_fileName)) {
                std::filesystem::rename(m_fileName, fmt::format("{}.0", m_fileName));
            }
        } while (0);

        fl.l_type = F_UNLCK;
        fcntl(lockFd, F_SETLK, &fl);
        close(lockFd);
    }
    std::string m_fileName; // 日志文件名
    uint32_t m_maxSize;           // 单个日志文件的最大大小（字节）
    uint16_t m_maxFiles;          // 最大保留的日志文件个数
    std::mutex m_writeMutex;      // 多线程轮转日志时，使用互斥锁
};

}
}