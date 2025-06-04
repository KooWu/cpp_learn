#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// ==================== 错误码实现 ====================
enum class ErrorCode {
    SUCCESS = 0,
    DIVIDE_BY_ZERO,
    FILE_NOT_FOUND,
    INVALID_DATA,
    MEMORY_ALLOC_FAILED
};

// 使用错误码的除法函数
ErrorCode divideErrorCode(int a, int b, int& result) {
    if (b == 0) {
        return ErrorCode::DIVIDE_BY_ZERO;
    }
    result = a / b;
    return ErrorCode::SUCCESS;
}

// 使用错误码的文件读取函数
ErrorCode readFileErrorCode(const string& filename, vector<int>& data) {
    ifstream file(filename);
    if (!file.is_open()) {
        return ErrorCode::FILE_NOT_FOUND;
    }

    int value;
    while (file >> value) {
        data.push_back(value);
    }

    if (data.empty()) {
        return ErrorCode::INVALID_DATA;
    }

    return ErrorCode::SUCCESS;
}

// ==================== 异常实现 ====================
// 自定义异常类
class DivideByZeroException : public runtime_error {
public:
    DivideByZeroException() : runtime_error("Division by zero") {}
};

class FileNotFoundException : public runtime_error {
public:
    FileNotFoundException(const string& filename) 
        : runtime_error("File not found: " + filename) {}
};

class InvalidDataException : public runtime_error {
public:
    InvalidDataException() : runtime_error("Invalid or empty data") {}
};

// 使用异常的除法函数
int divideException(int a, int b) {
    if (b == 0) {
        throw DivideByZeroException();
    }
    return a / b;
}

// 使用异常的文件读取函数
void readFileException(const string& filename, vector<int>& data) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw FileNotFoundException(filename);
    }

    int value;
    while (file >> value) {
        data.push_back(value);
    }

    if (data.empty()) {
        throw InvalidDataException();
    }
}

// ==================== 主函数测试 ====================
int main() {
    cout << "=== 错误码示例 ===" << endl;
    int result;
    ErrorCode ec = divideErrorCode(10, 0, result);
    if (ec != ErrorCode::SUCCESS) {
        switch (ec) {
            case ErrorCode::DIVIDE_BY_ZERO:
                cerr << "错误：除以零" << endl;
                break;
            default:
                cerr << "未知错误" << endl;
        }
    }

    vector<int> data;
    ec = readFileErrorCode("nonexistent.txt", data);
    if (ec != ErrorCode::SUCCESS) {
        switch (ec) {
            case ErrorCode::FILE_NOT_FOUND:
                cerr << "错误：文件未找到" << endl;
                break;
            case ErrorCode::INVALID_DATA:
                cerr << "错误：数据无效" << endl;
                break;
            default:
                cerr << "未知错误" << endl;
        }
    }

    cout << "\n=== 异常示例 ===" << endl;
    try {
        int result = divideException(10, 0);
        cout << "结果：" << result << endl;
    } catch (const DivideByZeroException& e) {
        cerr << "异常：" << e.what() << endl;
    } catch (const exception& e) {
        cerr << "其他异常：" << e.what() << endl;
    }

    try {
        vector<int> data;
        readFileException("nonexistent.txt", data);
        cout << "读取数据成功，大小：" << data.size() << endl;
    } catch (const FileNotFoundException& e) {
        cerr << "异常：" << e.what() << endl;
    } catch (const InvalidDataException& e) {
        cerr << "异常：" << e.what() << endl;
    } catch (const exception& e) {
        cerr << "其他异常：" << e.what() << endl;
    }

    // 演示栈展开
    cout << "\n=== 栈展开示例 ===" << endl;
    try {
        throw runtime_error("测试栈展开");
    } catch (const exception& e) {
        cerr << "捕获异常：" << e.what() << endl;
    }

    return 0;
}    