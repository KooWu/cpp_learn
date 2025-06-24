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