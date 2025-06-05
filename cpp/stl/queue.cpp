#include <iostream>
#include <queue>
#include <list>
#include <vector>
#include <string>

// 1. 自定义类型：用于队列存储
struct Task {
    std::string name;
    int priority;
    
    // 构造函数
    Task(const std::string& n = "", int p = 0) : name(n), priority(p) {}
    
    // 重载输出运算符，方便打印
    friend std::ostream& operator<<(std::ostream& os, const Task& t) {
        return os << "[" << t.name << ", 优先级: " << t.priority << "]";
    }
};

// 2. 树节点结构：用于 BFS 示例
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

int main() {
    // === 基本用法示例 ===
    std::cout << "=== 基本用法 ===\n";
    std::queue<int> intQueue;

    // 入队操作
    intQueue.push(10);
    intQueue.push(20);
    intQueue.emplace(30);  // 直接构造元素

    // 访问队首和队尾
    std::cout << "队首元素: " << intQueue.front() << "\n";  // 输出: 10
    std::cout << "队尾元素: " << intQueue.back() << "\n";   // 输出: 30

    // 出队操作
    intQueue.pop();
    std::cout << "出队后队首: " << intQueue.front() << "\n";  // 输出: 20

    // 检查队列状态
    std::cout << "队列大小: " << intQueue.size() << "\n";  // 输出: 2
    std::cout << "是否为空: " << (intQueue.empty() ? "是" : "否") << "\n\n";


    // === 使用自定义类型 ===
    std::cout << "=== 使用自定义类型 ===\n";
    std::queue<Task> taskQueue;

    // 入队自定义类型
    taskQueue.push({"数据处理", 2});
    taskQueue.emplace("网络请求", 1);  // 直接构造对象

    // 访问队首
    std::cout << "队首任务: " << taskQueue.front() << "\n";  // 输出: [数据处理, 优先级: 2]

    // 修改队首元素（通过引用）
    taskQueue.front().priority = 3;
    std::cout << "修改后队首: " << taskQueue.front() << "\n";  // 输出: [数据处理, 优先级: 3]

    // 遍历队列（需弹出元素）
    std::cout << "遍历队列: ";
    while (!taskQueue.empty()) {
        std::cout << taskQueue.front() << " ";
        taskQueue.pop();
    }
    std::cout << "\n\n";


    // === 使用 list 作为底层容器 ===
    std::cout << "=== 使用 list 作为底层容器 ===\n";
    std::queue<std::string, std::list<std::string>> listQueue;

    // 入队操作
    listQueue.push("apple");
    listQueue.push("banana");
    listQueue.emplace("cherry");

    // 访问队首和队尾
    std::cout << "队首: " << listQueue.front() << "\n";  // 输出: apple
    std::cout << "队尾: " << listQueue.back() << "\n";   // 输出: cherry

    // 清空队列
    while (!listQueue.empty()) {
        listQueue.pop();
    }
    std::cout << "清空后队列大小: " << listQueue.size() << "\n\n";


    // === 树的层序遍历（BFS）示例 ===
    std::cout << "=== 树的层序遍历 ===\n";
    // 构建二叉树:
    //       1
    //      / \
    //     2   3
    //    / \
    //   4   5
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);

    // 层序遍历
    std::queue<TreeNode*> treeQueue;
    treeQueue.push(root);

    std::cout << "层序遍历结果: ";
    while (!treeQueue.empty()) {
        TreeNode* current = treeQueue.front();
        treeQueue.pop();
        std::cout << current->val << " ";

        if (current->left) treeQueue.push(current->left);
        if (current->right) treeQueue.push(current->right);
    }
    std::cout << "\n\n";

    // 释放内存
    delete root->left->left;
    delete root->left->right;
    delete root->left;
    delete root->right;
    delete root;


    // === 任务调度示例 ===
    std::cout << "=== 任务调度示例 ===\n";
    std::queue<std::string> jobQueue;

    // 添加任务
    jobQueue.push("解析配置文件");
    jobQueue.push("初始化数据库");
    jobQueue.push("启动服务器");
    jobQueue.push("加载插件");

    // 处理所有任务
    std::cout << "任务处理顺序:\n";
    while (!jobQueue.empty()) {
        std::cout << "- 处理: " << jobQueue.front() << "\n";
        jobQueue.pop();
        std::cout << "  剩余任务: " << jobQueue.size() << "\n";
    }
    std::cout << "\n";


    // === 生产者-消费者模型简化示例 ===
    std::cout << "=== 生产者-消费者模型 ===\n";
    std::queue<int> messageQueue;
    const int MAX_MESSAGES = 5;

    // 生产者: 添加消息
    std::cout << "生产者添加消息:\n";
    for (int i = 1; i <= MAX_MESSAGES; ++i) {
        messageQueue.push(i);
        std::cout << "  消息 " << i << " 已入队\n";
    }

    // 消费者: 处理消息
    std::cout << "消费者处理消息:\n";
    while (!messageQueue.empty()) {
        int msg = messageQueue.front();
        messageQueue.pop();
        std::cout << "  处理消息 " << msg << "\n";
    }

    return 0;
}