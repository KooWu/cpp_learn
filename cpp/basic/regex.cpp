#include <iostream>
#include <regex>
#include <sstream>
#include <cstdint> 
#include <string>
#include <optional>  // C++17及以上，用于返回可选值

// 匹配形如0x[十六进制]_shot或0x[十六进制]_EXT的字符串，并提取十六进制数字
std::optional<uint32_t> extractHexNumber(const std::string& input) {
    // 正则表达式模式：
    // - 0x：固定前缀
    // - ([0-9a-fA-F]+)：捕获组1，匹配1个以上十六进制字符（要提取的数字部分）
    // - _(shot|EXT)：匹配_shot或_EXT后缀
    const std::regex pattern(R"(0x([0-9a-fA-F]+)_(shot|EXT))");
    std::smatch matchResult;
    std::stringstream ss;
    uint32_t value;
    // 执行匹配
    if (std::regex_match(input, matchResult, pattern)) {
        // 匹配成功，返回捕获组1的内容（十六进制数字部分）
        ss << std::hex << matchResult[1].str();
        ss >> value;
        // 检查转换是否成功且无多余字符
        if (ss.fail() || !ss.eof()) {
            return std::nullopt; // 转换失败
        }
        return value;
    }
    // 匹配失败，返回空
    return std::nullopt;
}

int main() {
    // 测试案例
    std::vector<std::string> testCases = {
        "0x123456_shot",   // 应提取"123456"
        "0xABCDEF_EXT",    // 应提取"ABCDEF"
        "0xa1b2c3_shot",   // 应提取"a1b2c3"
        "0x123_EXT",       // 应提取"123"
        "0xGHIJKL_shot",   // 无效（含非十六进制字符）
        "123456_shot",     // 无效（缺少0x前缀）
        "0x123456_other"   // 无效（后缀不是shot或EXT）
    };

    for (const auto& str : testCases) {
        auto hexNum = extractHexNumber(str);
        if (hexNum.has_value()) {
            std::cout << "输入: " << str << " → 提取的十六进制数字: " << hexNum.value() << std::endl;
        } else {
            std::cout << "输入: " << str << " → 不匹配模式" << std::endl;
        }
    }

    return 0;
}
