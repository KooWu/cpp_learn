#include <iostream>
#include <iomanip>
#include <ctime>      // C风格时间函数
#include <chrono>     // C++11起的chrono库
#include <thread>     // 线程休眠
#include <string>

using namespace std;
using namespace std::chrono;

// 1. C风格时间函数
void cStyleTimeFunctions() {
    cout << "\n=== C风格时间函数 ===" << endl;
    
    // 获取当前时间（秒级）
    time_t now = time(nullptr);
    cout << "当前时间戳（秒）: " << now << endl;
    
    // 转换为本地时间
    tm* localTime = localtime(&now);
    cout << "本地时间: " 
         << localTime->tm_year + 1900 << "-"
         << setw(2) << setfill('0') << localTime->tm_mon + 1 << "-"
         << setw(2) << setfill('0') << localTime->tm_mday << " "
         << setw(2) << setfill('0') << localTime->tm_hour << ":"
         << setw(2) << setfill('0') << localTime->tm_min << ":"
         << setw(2) << setfill('0') << localTime->tm_sec << endl;
    
    // 转换为UTC时间
    tm* utcTime = gmtime(&now);
    cout << "UTC时间: " 
         << put_time(utcTime, "%Y-%m-%d %H:%M:%S") << endl;
    
    // 格式化输出
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
    cout << "格式化时间: " << buffer << endl;
    
    // 字符串转时间
    const char* timeStr = "2023-10-01 12:30:45";
    tm timeStruct = {};
    strptime(timeStr, "%Y-%m-%d %H:%M:%S", &timeStruct);
    time_t parsedTime = mktime(&timeStruct);
    cout << "解析后的时间戳: " << parsedTime << endl;
}

// 2. C++11 chrono库 - 时间点
void chronoTimePoints() {
    cout << "\n=== C++ chrono时间点 ===" << endl;
    
    // 获取当前时间点
    system_clock::time_point now = system_clock::now();
    
    // 转换为time_t
    time_t now_c = system_clock::to_time_t(now);
    cout << "当前时间: " << ctime(&now_c);
    
    // 时间点运算（加1小时）
    auto oneHourLater = now + hours(1);
    time_t later_c = system_clock::to_time_t(oneHourLater);
    cout << "1小时后: " << ctime(&later_c);
    
    // 计算时间差
    auto midnight = system_clock::time_point{};
    duration<double> secondsSinceMidnight = now - midnight;
    cout << "自午夜以来的秒数: " << secondsSinceMidnight.count() << endl;
}

// 3. C++11 chrono库 - 时间段
void chronoDurations() {
    cout << "\n=== C++ chrono时间段 ===" << endl;
    
    // 定义不同的时间段
    duration<int> oneMinute(60);               // 60秒
    duration<double, ratio<1, 1000>> fiveMs(5); // 5毫秒
    duration<double, ratio<60, 1>> twoHours(2); // 2小时
    
    // 时间段运算
    auto totalSeconds = oneMinute + twoHours;
    cout << "1分钟 + 2小时 = " << totalSeconds.count() << " 秒" << endl;
    
    // 转换为不同单位
    auto ms = duration_cast<milliseconds>(oneMinute);
    cout << "1分钟 = " << ms.count() << " 毫秒" << endl;
    
    // 测量代码执行时间
    auto start = high_resolution_clock::now();
    for (int i = 0; i < 1000000; ++i) {
        // 模拟一些工作
    }
    auto end = high_resolution_clock::now();
    
    duration<double, milli> elapsed = end - start;
    cout << "循环执行时间: " << elapsed.count() << " 毫秒" << endl;
}

// 4. 高精度计时器
class Timer {
private:
    high_resolution_clock::time_point startTime;
    
public:
    Timer() { reset(); }
    
    void reset() {
        startTime = high_resolution_clock::now();
    }
    
    double elapsedSeconds() const {
        auto endTime = high_resolution_clock::now();
        return duration_cast<duration<double>>(endTime - startTime).count();
    }
    
    double elapsedMilliseconds() const {
        return elapsedSeconds() * 1000.0;
    }
};

void timerExample() {
    cout << "\n=== 高精度计时器示例 ===" << endl;
    
    Timer timer;
    
    // 模拟工作
    this_thread::sleep_for(milliseconds(250));
    
    cout << "休眠时间: " << timer.elapsedMilliseconds() << " 毫秒" << endl;
    
    timer.reset();
    // 另一个任务
    this_thread::sleep_for(milliseconds(300));
    
    cout << "第二次休眠时间: " << timer.elapsedMilliseconds() << " 毫秒" << endl;
}

// 6. 睡眠功能
void sleepExample() {
    cout << "\n=== 线程休眠示例 ===" << endl;
    
    cout << "开始休眠..." << endl;
    
    // 休眠1秒
    this_thread::sleep_for(seconds(1));
    
    // 休眠500毫秒
    this_thread::sleep_for(milliseconds(500));
    
    // 休眠0.2秒
    this_thread::sleep_for(duration<double>(0.2));
    
    cout << "休眠结束" << endl;
}

int main() {
    cStyleTimeFunctions();
    chronoTimePoints();
    chronoDurations();
    timerExample();
    sleepExample();
    
    return 0;
}