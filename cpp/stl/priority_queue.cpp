#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

// 1. 自定义类型：任务（用于优先队列）
struct Task {
    std::string name;
    int priority;

    // 构造函数
    Task(const std::string& n = "", int p = 0) : name(n), priority(p) {}

    // 重载 < 运算符，定义优先级顺序（大顶堆）
    bool operator<(const Task& other) const {
        return priority < other.priority;  // 优先级高的任务先出队
    }

    // 重载输出运算符，方便打印
    friend std::ostream& operator<<(std::ostream& os, const Task& t) {
        return os << "[" << t.name << ", 优先级: " << t.priority << "]";
    }
};

// 2. 自定义类型：坐标点（用于最小距离排序）
struct Point {
    int x, y;
    double distance;  // 到原点的距离

    Point(int x = 0, int y = 0) : x(x), y(y) {
        distance = std::sqrt(x * x + y * y);
    }

    // 重载 < 运算符，定义距离比较（小顶堆）
    bool operator<(const Point& other) const {
        return distance > other.distance;  // 距离小的点先出队
    }

    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << "(" << p.x << ", " << p.y << ", 距离: " << p.distance << ")";
    }
};

// 3. 自定义比较器：用于字符串长度排序
struct CompareStringLength {
    bool operator()(const std::string& a, const std::string& b) const {
        return a.length() < b.length();  // 长字符串优先
    }
};

int main() {
    // === 基本用法示例（大顶堆） ===
    std::cout << "=== 基本用法（大顶堆） ===\n";
    std::priority_queue<int> maxHeap;

    // 插入元素
    maxHeap.push(30);
    maxHeap.push(10);
    maxHeap.push(20);

    // 访问顶部元素
    std::cout << "顶部元素: " << maxHeap.top() << "\n";  // 输出: 30

    // 删除顶部元素
    maxHeap.pop();
    std::cout << "删除后顶部: " << maxHeap.top() << "\n";  // 输出: 20

    // 检查状态
    std::cout << "堆大小: " << maxHeap.size() << "\n\n";


    // === 使用自定义比较器（小顶堆） ===
    std::cout << "=== 小顶堆 ===\n";
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

    // 插入元素
    minHeap.push(30);
    minHeap.push(10);
    minHeap.push(20);

    // 访问顶部元素
    std::cout << "顶部元素: " << minHeap.top() << "\n";  // 输出: 10

    // 删除顶部元素
    minHeap.pop();
    std::cout << "删除后顶部: " << minHeap.top() << "\n\n";  // 输出: 20


    // === 使用自定义类型（任务调度） ===
    std::cout << "=== 任务调度 ===\n";
    std::priority_queue<Task> taskQueue;

    // 插入任务
    taskQueue.push({"修复漏洞", 3});
    taskQueue.push({"添加功能", 2});
    taskQueue.push({"编写文档", 1});
    taskQueue.push({"优化性能", 4});

    // 处理所有任务（按优先级降序）
    std::cout << "任务处理顺序:\n";
    while (!taskQueue.empty()) {
        std::cout << taskQueue.top() << "\n";
        taskQueue.pop();
    }
    std::cout << "\n";


    // === 自定义比较器（字符串长度） ===
    std::cout << "=== 字符串长度排序 ===\n";
    std::priority_queue<std::string, std::vector<std::string>, CompareStringLength> stringHeap;

    // 插入字符串
    stringHeap.push("apple");
    stringHeap.push("banana");
    stringHeap.push("cherry");
    stringHeap.push("date");

    // 输出（按长度降序）
    std::cout << "按长度降序排列:\n";
    while (!stringHeap.empty()) {
        std::cout << stringHeap.top() << "\n";
        stringHeap.pop();
    }
    std::cout << "\n";


    // === Top-K 问题（最小的 K 个元素） ===
    std::cout << "=== Top-K 问题（最小的 K 个元素） ===\n";
    std::vector<int> data = {3, 1, 4, 1, 5, 9, 2, 6, 5};
    int k = 4;

    // 使用大顶堆维护最小的 K 个元素
    std::priority_queue<int> topKHeap;

    for (int num : data) {
        topKHeap.push(num);
        if (topKHeap.size() > k) {
            topKHeap.pop();  // 移除最大元素
        }
    }

    // 输出最小的 K 个元素（降序）
    std::cout << "最小的 " << k << " 个元素:\n";
    std::vector<int> result;
    while (!topKHeap.empty()) {
        result.push_back(topKHeap.top());
        topKHeap.pop();
    }
    // 反转以获得升序
    std::reverse(result.begin(), result.end());
    for (int num : result) {
        std::cout << num << " ";
    }
    std::cout << "\n\n";


    // === 最近点问题（最小距离） ===
    std::cout << "=== 最近点问题 ===\n";
    std::vector<Point> points = {{3, 4}, {1, 1}, {5, 12}, {0, 1}, {2, 2}};
    int m = 3;  // 找出最近的 3 个点

    // 使用小顶堆（自动按距离排序）
    std::priority_queue<Point> pointHeap;
    for (const auto& p : points) {
        pointHeap.push(p);
    }

    // 输出最近的 m 个点
    std::cout << "最近的 " << m << " 个点:\n";
    for (int i = 0; i < m && !pointHeap.empty(); ++i) {
        std::cout << pointHeap.top() << "\n";
        pointHeap.pop();
    }
    std::cout << "\n";


    // === 数据流中位数 ===
    std::cout << "=== 数据流中位数 ===\n";
    std::vector<int> stream = {2, 1, 5, 7, 2, 0, 5};
    std::priority_queue<int> maxHeapForMedian;  // 存储较小的一半元素
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeapForMedian;  // 存储较大的一半元素

    std::cout << "数据流中位数:\n";
    for (int num : stream) {
        // 插入元素
        if (maxHeapForMedian.empty() || num <= maxHeapForMedian.top()) {
            maxHeapForMedian.push(num);
        } else {
            minHeapForMedian.push(num);
        }

        // 平衡两个堆的大小
        if (maxHeapForMedian.size() > minHeapForMedian.size() + 1) {
            minHeapForMedian.push(maxHeapForMedian.top());
            maxHeapForMedian.pop();
        } else if (minHeapForMedian.size() > maxHeapForMedian.size()) {
            maxHeapForMedian.push(minHeapForMedian.top());
            minHeapForMedian.pop();
        }

        // 计算中位数
        double median;
        if (maxHeapForMedian.size() == minHeapForMedian.size()) {
            median = (maxHeapForMedian.top() + minHeapForMedian.top()) / 2.0;
        } else {
            median = maxHeapForMedian.top();
        }

        std::cout << "插入 " << num << " 后的中位数: " << median << "\n";
    }

    return 0;
}