#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <memory>

// 前向声明
class Command;
class Receiver;
class Invoker;

// ========== 1. 命令接口 ==========
class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() = default;
};

// ========== 2. 接收者类（处理实际逻辑） ==========
class Receiver {
private:
    std::string name;
public:
    Receiver(const std::string& n) : name(n) {}
    
    // 普通成员函数
    void action(int value) {
        std::cout << "Receiver[" << name << "]::action called with " << value << std::endl;
    }
    
    // 返回值和参数不同的成员函数
    std::string query(const std::string& param) {
        return "Receiver["+ name + "]::query: " + param;
    }
};

// ========== 3. 具体命令实现 ==========
// 3.1 使用 std::function 和 Lambda 的命令
class LambdaCommand : public Command {
private:
    std::function<void()> action;
public:
    LambdaCommand(const std::function<void()>& func) : action(func) {}
    
    void execute() override {
        action();
    }
};

// 3.2 使用 std::bind 的命令（绑定成员函数）
class BoundCommand : public Command {
private:
    std::function<void()> boundAction;
public:
    // 构造函数接收 std::function
    BoundCommand(const std::function<void()>& func) : boundAction(func) {}
    
    void execute() override {
        boundAction();
    }
};

// ========== 4. 调用者类（管理命令） ==========
class Invoker {
private:
    std::vector<std::shared_ptr<Command>> commands;
public:
    void addCommand(std::shared_ptr<Command> cmd) {
        commands.push_back(cmd);
    }
    
    void executeAll() {
        for (auto& cmd : commands) {
            cmd->execute();
        }
    }
};

// ========== 5. 全局函数（用于演示） ==========
void globalFunction(int x, const std::string& msg) {
    std::cout << "Global function called with " << x << " and " << msg << std::endl;
}

// ========== 6. 主函数 ==========
int main() {
    // 创建接收者
    Receiver receiver("FileReceiver");
    
    // 创建调用者
    Invoker invoker;
    
    // === 示例1：使用 Lambda 表达式 ===
    int value = 42;
    invoker.addCommand(std::make_shared<LambdaCommand>(
        [&receiver, value]() {
            receiver.action(value);
        }
    ));
    
    // === 示例2：使用 std::bind 绑定成员函数 ===
    // 绑定 Receiver::action，固定参数为 100
    auto boundMember = std::bind(&Receiver::action, &receiver, 100);
    invoker.addCommand(std::make_shared<BoundCommand>(boundMember));
    
    // === 示例3：使用 std::bind 绑定全局函数 ===
    // 绑定 globalFunction，固定第一个参数为 200
    auto boundGlobal = std::bind(globalFunction, 200, std::placeholders::_1);
    invoker.addCommand(std::make_shared<LambdaCommand>(
        [boundGlobal]() {
            boundGlobal("Hello from bound global function");
        }
    ));
    
    // === 示例4：更复杂的 Lambda 捕获和调用 ===
    std::string queryParam = "config.txt";
    invoker.addCommand(std::make_shared<LambdaCommand>(
        [&receiver, queryParam]() {
            std::string result = receiver.query(queryParam);
            std::cout << "Query result: " << result << std::endl;
        }
    ));
    
    // 执行所有命令
    std::cout << "Executing all commands..." << std::endl;
    invoker.executeAll();
    
    return 0;
}