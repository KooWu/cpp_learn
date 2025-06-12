# cvi-lib-log

## 简介

本仓库为日志库，提供日志记录、日志格式化、日志等级过滤等功能。
同时支持日志输出到控制台/文件/。

## 依赖
日志输出依赖spdlog，配置依赖tomlplusplus

## 实现原理
此日志库基于高性能C++日志库 spdlog 构建，利用了 spdlog 的灵活性和强大特性。在底层架构上，它通过管理一组称为“sink”的输出目的地来实现多样化的日志输出。具体而言，一个 sink 负责将日志信息输出到控制台，而另一个自定义实现的 sink 则专注于多进程环境下的文件日志记录，并且实现了安全的日志轮转机制以确保日志文件不会无限制增长。

对于日志格式化和等级过滤等功能，完全依赖于 spdlog 提供的强大功能。

## 基本使用
### 新接口
需要先调用 `CviLogInit()` 函数初始化日志库，然后就可以使用 `CVI_XXX()` 函数输出日志信息。

```c++
#include "log_service.h" // 包含日志库头文件

int main() {
    CviLogInit("app_name");
    CVI_INFO("Hello, world!"); // 新增接口
    return 0;
}
```

### 兼容接口
需要先调用 `log_system_init()` 函数初始化日志库，然后就可以使用 `MWP_XXX()` 函数输出日志信息。

```c++
#include "MWPLog.h" // 包含日志库头文件

int main() {
    log_system_init("app_name");
    MWP_INFO("Hello, world!"); // 新增接口
    return 0;
}
```

### 编译
在 CMakeLists.txt 中添加以下内容：
```cmake
find_package(cvi-lib-log REQUIRED)
target_link_libraries(your_target PRIVATE cvi-lib-log)
```

### 新接口与兼容接口的区别
- MWP_TRACE输出的日志等级为notice，CVI_TRACE输出的日志等级为trace，notice等级，高于warn低于error。

## 日志等级过滤
日志库支持日志等级过滤，可以通过设置日志等级来控制日志输出的详细程度，日志等级可在配置文件里面设置，设置方式参考[日志配置](#jump日志配置)，如果想在运行时动态设置日志等级，可以使用 `CviLogSetLevel()` 函数。

```c++
#include "log_service.h"

int main() {
    CviLogInit("app_name");
    CviLogSetLevel(spdlog::level::warn);
    return 0;
}
```

## <span id="jump日志配置">日志配置</span>

日志文件配置默认位于 /etc/log_cfg.toml，可以通过修改该文件来修改日志配置。
还可以通过环境变量 `LOG_CFG_FILE_NAME_ENV` 来指定日志配置文件的路径。
例如：export LOG_CFG_FILE_NAME_ENV=/path/to/log_cfg.toml

### 日志配置说明
日志配置文件采用 TOML 格式，主要包含以下配置项：
> logFile：日志文件配置，包括日志文件名fileName、大小fileSize、个数fileNum等。所有应用共享该配置。

> apps：应用配置，每个应用可以配置日志等级level、是否输出到控制台enableConsole、是否输出到文件enableFile，用name来区分不同应用。

```toml
[logFile]
# 日志文件配置
fileName = '/tmp/test_log_app.log' # 日志文件名
fileSize = 102400 # 日志文件大小，单位：byte
fileNum = 3 # 日志文件个数

# 第一个应用配置
[[apps]]
name = 'test_log_app' # 应用名
consoleLevel = 'debug' # 控制台日志等级
fileLevel = 'trace' # 文件日志等级
enableConsole = false # 是否输出到控制台
enableFile = true # 是否输出到文件

# 第二个应用配置
[[apps]]
name = 'persist_test'
consoleLevel = 'info' # 控制台日志等级
fileLevel = 'warn' # 文件日志等级
enableConsole = false
enableFile = true
```
### 配置文件日志等级说明
日志等级分为以下几类：
- trace：最详细的日志，用于调试。
- debug：调试信息，用于调试。
- info：一般信息，用于跟踪。
- warn：警告信息，用于提醒。
- notice：提醒信息，用于跟踪。
- err：错误信息，用于记录错误。
- critical：严重错误信息，用于记录严重错误。

### 如何为新应用配置日志

如果需要为新应用配置日志，需要在配置文件中增加相应的配置项，并在程序启动时调用 `CviLogInit("new_app")` 函数初始化日志库。

```toml
[[apps]]
name = 'new_app'
consoleLevel = 'info' # 控制台日志等级
fileLevel = 'warn' # 文件日志等级
enableConsole = false
enableFile = true
```

### 默认等级
如果没有配置日志等级，默认等级为info，默认输出到文件，不输出到控制台，默认输出文件名为 /tmp/usb.log，默认日志文件大小为 10M，默认日志文件个数为 3。

## 新接口输出
为了让日志输出更方便，新接口CVI_xxx()，用{}占位符取代原来printf风格的日志输出。

### 基本类型输出
```c++
    // 基本格式化
    bool flag = true;
    CVI_INFO("flag: {}", flag); // 输出: flag: true
    // 整数格式化
    int32_t ret = 32;
    CVI_INFO("ret: {}", ret); // 输出: ret: 32
    // 长整数格式化
    int64_t num = 1234567890123456789;
    CVI_INFO("num: {}", num); // 输出: num: 1234567890123456789
    // 指针格式化
    void* ptr = (void*)0x12345678;
    CVI_INFO("ptr: {}", ptr); // 输出: ptr: 0x12345678
    // 浮点数格式化
    float f = 3.141592653589793;
    CVI_INFO("f: {}", f); // 输出: f: 3.1415927
    // 字符串格式化
    CVI_INFO("Hello, {}!", "World"); // 输出: Hello, World!
    // 数字格式化
    CVI_INFO("The answer is {}.", 42); // 输出: The answer is 42.
    // 浮点数格式化
    CVI_INFO("Pi is approximately {:.6f}.", 3.141592653589793); // 输出: Pi is approximately 3.141593.
    // 对齐和填充
    CVI_INFO("{:>10}", "hello"); // 右对齐，总宽度为10，输出: '     hello'
    CVI_INFO("{:<10}", "hello"); // 左对齐，总宽度为10，输出: 'hello        
    CVI_INFO("{:^10}", "hello"); // 居中对齐，总宽度为10， 输出: '  hello   '
    // 自定义填充字符
    CVI_INFO("{:*^10}", "hello"); // 使用星号填充，居中对齐，输出: '**hello***'
    // 位置控制格式化
    CVI_INFO("I'd rather be {1} than {0}.", "right", "happy");   // 输出: I'd rather be happy than right.
    CVI_INFO("{0}, {1}, {0}!", "Hello", "World"); // 输出: Hello, World, Hello! 
```
### 时间格式化输出
```c++
    auto now = std::chrono::system_clock::now();
    CVI_INFO("Date and time: {}", now); // Date and time: 2025-01-20 06:18:27.222230671
    CVI_INFO("Time: {:%H:%M}", now); // Time: 06:18
```
### 容器输出
```c++
    std::array<int, 3> arr = {1, 2, 3};
    CVI_INFO("arr: {}", arr); // arr: [1, 2, 3]
    std::vector<int> vec = {1, 2, 3, 4, 5};
    CVI_INFO("Vector: {}", vec); // Vector: [1, 2, 3, 4, 5]
    std::map<std::string, int> map = {{"a", 1}, {"b", 2}, {"c", 3}};
    CVI_INFO("Map: {}", map); // Map: {a:1, b:2, c:3}
    std::set<std::string> set = {"apple", "banana", "orange"};
    CVI_INFO("Set: {}", set); // Set: {apple, orange, banana}
```
### 自定义类型输出
```c++
    struct CustomType {
        std::string x;
        int y;
    };

    // 为 CustomType 类型特化 fmt::formatter
    template <>
    struct fmt::formatter<CustomType> {
        constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
            return ctx.begin();
        }

        // 格式化 CustomType 对象
        template <typename FormatContext>
        auto format(const CustomType& p, FormatContext& ctx) -> decltype(ctx.out()) {
            // 使用解析的格式说明符来决定如何格式化
            return fmt::format_to(ctx.out(), "(x:{},y:{})", p.x, p.y);
        }
    };
    CustomType test{"jkl", 2};
    CVI_INFO("CustomType: {}", test); // Point: {x:jkl, y:2}
```
### 枚举输出
借助开源库nameof，可以将枚举类型输出为字符串。
```c++
#include <nameof.hpp>

    enum class Color { RED, GREEN, BLUE };
    Color myColor = Color::RED;
    CVI_INFO("Color: {}", NAMEOF_ENUM(myColor)); // Color: RED
```
CMakeLists.txt中添加依赖：
```cmake
find_package(nameof REQUIRED)
target_link_libraries(your_target PRIVATE nameof::nameof)
```