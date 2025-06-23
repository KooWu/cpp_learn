#pragma once
#include "spdlog/pattern_formatter.h"
#include "spdlog/details/os.h"
#include "iostream"

namespace spdlog {
namespace custom_formatters {

class cached_pid_formatter : public custom_flag_formatter {
public:
    cached_pid_formatter() : m_cachedPid(std::to_string(details::os::pid())) {
    }

    void format(const details::log_msg &, const std::tm &, memory_buf_t &dest) override {
        dest.append(m_cachedPid);
    }

    std::unique_ptr<custom_flag_formatter> clone() const override {
        return details::make_unique<cached_pid_formatter>();
    }

private:
    std::string m_cachedPid;
};

} // namespace custom_formatters
} // namespace spdlog