#include <sqlite3.h>
#include <iostream>
#include <mutex>
#include <random>
#include <chrono>
#include <thread>

class DBSession {
public:
    static DBSession& Instance() {
        static DBSession instance;
        return instance;
    }

    bool Execute(const std::string& sql) {
        std::lock_guard<std::mutex> lock(db_mutex_);
        char* errmsg = nullptr;
        int rc = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errmsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL Error: " << errmsg << std::endl;
            sqlite3_free(errmsg);
            return false;
        }
        return true;
    }

private:
    sqlite3* db_;
    std::mutex db_mutex_;

    DBSession() {
        int rc = sqlite3_open("mydatabase.db", &db_);
        if (rc != SQLITE_OK) {
            std::cerr << "Cannot open database: " << sqlite3_errmsg(db_) << std::endl;
            exit(1);
        }
        // // 初始化设置
        // sqlite3_exec(db_, "PRAGMA journal_mode=WAL;", nullptr, nullptr, nullptr);
        // sqlite3_exec(db_, "PRAGMA synchronous=NORMAL;", nullptr, nullptr, nullptr);
    }

    ~DBSession() {
        if (db_) {
            sqlite3_close(db_);
            std::cout << "Database connection closed." << std::endl;
        }
    }

    DBSession(const DBSession&) = delete;
    DBSession& operator=(const DBSession&) = delete;
};

// 创建一个随机名字生成器
std::string generateRandomName(size_t length) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::string name;
    name.reserve(length);
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> dist(0, sizeof(alphanum) - 2);

    for (size_t i = 0; i < length; ++i) {
        name += alphanum[dist(rng)];
    }

    return name;
}

// 使用示例
int main() {
    // sqlite3_soft_heap_limit(8 * 1024 * 1024);
    auto& db = DBSession::Instance();
    // 执行多次操作
    db.Execute("CREATE TABLE IF NOT EXISTS users(id INTEGER PRIMARY KEY, name TEXT);");
    
    // 周期性地插入随机生成的名字
    for (size_t i = 0; i < 100000; ++i) { // 假设我们想插入10个随机名字
        std::string name = generateRandomName(100); // 每个名字长度为5
        std::string sql = "INSERT INTO users(name) VALUES ('" + name + "');";
        db.Execute(sql);
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 每隔一秒插入一个名字
    }
    return 0;
}