# 3方库介绍

## tomlplusplus
toml解析库c++版本，用来替代json，可以携带注释，结构更简单
版本：3.4.0
依赖：无

## spdlog
现代化日志库，非常流行，支持自定义format/pattern，以及支持占位符式的输出，自研日志库依赖
版本：1.15.3
依赖：无

## googletest
单元测试库，用作接口测试，在做单元测试时，不仅测试外部接口，也要测试内部接口，要求c++14，比此版本高的需要c++17
版本：1.16.0
依赖：无

## benchmark
性能测试库，google出品
版本：1.9.4
依赖：无

## nameof
利用常用的元编程工具，用户获取变量/类型/函数邓的名称字符串
版本：0.10.4
依赖：无

## libunwind
用于程序栈展开（Stack Unwinding）的库，它可以在运行时获取函数调用栈信息，常用于调试工具、性能分析器和异常处理系统
版本：1.8.1
依赖：无

## jemalloc
专注解决内存碎片的内存管理器，在多线程情况下的竞争也少，如果是多线程应用，关注内存碎片和性能，选择 jemalloc，通常jemalloc是个不错的选项，注意tccache的配置
版本：5.3.0
依赖：无

## gperftools
google开发的一套高性能工具集，高性能内存分配器tcmalloc/cpu性能分析工具/heap内存分析工具/内存泄漏检测工具/缓存使用分析工具，根据不同选项 --heap-checker/cpu-profiler/heap-profiler/
版本：2.16.90
依赖：libunwind

## abseil-cpp
Abseil 是由 Google 开发的开源 C++ 库集合，旨在为开发者提供高效、可靠且可移植的基础工具。它的名称来源于 “Abstract” 的缩写，寓意其提供抽象化的核心功能，帮助开发者避免重复造轮子，有基础工具/时间与数学/并发控制/调试与测试
版本：20250127.0，protofbuf 30.2依赖此版本
依赖：NA

## protobuf
Protocol Buffers 是 Google 开发的一种语言无关、平台无关的序列化数据结构的方法，常用于通信协议、数据存储等场景。
版本：20250127.0，protofbuf 30.2依赖此版本
依赖：abseil-cpp 20250127.0

## fdbus
轻量级的soa通信中间件，与vsomeip类似，可以与tool中fdbus_tool进行配合生成接口代码，其中的序列化方案依赖protobuf，但是编译无依赖
版本：无版本
依赖：NA

## openssl
OpenSSL 是一个开源的加密库，提供了安全套接字层（SSL）和传输层安全（TLS）协议的实现，同时包含了大量密码学相关的功能，广泛应用于网络安全通信、数据加密、数字签名等场景。
需要高性能的，可以选择WolfSSL，支持多种平台和处理器架构。
版本：1.1.1k
依赖：无版本

## mosquitto
Mosquitto 是一个开源的消息代理软件，主要用于实现基于 MQTT（Message Queuing Telemetry Transport） 协议的消息通信。它由 Eclipse Foundation 维护，具有轻量级、低功耗、占用资源少等特点，非常适合物联网（IoT）设备、嵌入式系统以及需要在低带宽或不稳定网络环境下工作的场景。
版本：2.0.21
依赖：openssl（自动检测有安全库，则打开安全功能）

## boost
Boost 库是一个开源的 C++ 程序库集合，由 C++ 标准委员会前成员发起，旨在为 C++ 开发者提供高效、可靠且可移植的组件。它不仅补充了 C++ 标准库的功能，还包含了许多前沿技术实现，被广泛应用于系统开发、网络编程、数值计算等领域。下载链接https://www.boost.org/releases/latest/
版本：1.80
依赖：无

## dlt-daemon
DLT（Diagnostic Log and Trace）是一种专为汽车电子系统设计的标准化日志协议，由 AUTOSAR（汽车开放系统架构）组织开发。DLT-Daemon 是 DLT 协议的核心实现，负责收集、处理和分发车辆各 ECU（电子控制单元）产生的日志数据
版本：2.18.10
依赖：无

## vsomeip
一种轻量级的通信中间件，主要用于汽车电子领域的服务通信，基于 IP 协议实现跨进程、跨主机的对象通信。
版本：3.5.6
依赖：boost（1.80，1.88不可用）dlt-daemon(可选)

## CAPICXX
CAPICXX（Common API C++）是用于汽车电子的 C++ 中间件框架，提供跨进程通信（IPC）能力，支持多种传输协议。它由两部分核心组件构成：

CAPICXX-CORE-RUNTIME：提供基础通信抽象层和接口定义语言（IDL）编译工具
CAPICXX-SOMEIP-RUNTIME：基于 SOME/IP（Scalable service-Oriented MiddlewarE over IP）协议的具体实现

两者关系：Core 是基础框架，SOMEIP 是基于 Core 的传输层实现。
版本：3.2.4
依赖：无

## sqlite
SQLite 是一款轻量级的嵌入式关系型数据库，以其零配置、无需独立服务器、跨平台和高效性而广泛应用于各类场景（如移动应用、嵌入式设备、桌面软件等）
版本：3.50.4
依赖：无