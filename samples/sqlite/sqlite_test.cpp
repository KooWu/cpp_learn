
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <vector>

sqlite3* initDatabase(const char* dbPath) {
    sqlite3* db;
    int rc = sqlite3_open(dbPath, &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return nullptr;
    }
    return db;
}

void createTable(sqlite3* db) {
    const char* sql = "CREATE TABLE IF NOT EXISTS KeyValue (key INTEGER PRIMARY KEY, value TEXT);";
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

void insertData(sqlite3* db, int key, const std::string& value) {
    std::string sql = "INSERT INTO KeyValue (key, value) VALUES (" + std::to_string(key) + ", '" + value + "');";
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Data inserted successfully." << std::endl;
    }
}

void deleteData(sqlite3* db, int key) {
    std::string sql = "DELETE FROM KeyValue WHERE key = " + std::to_string(key) + ";";
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Data deleted successfully." << std::endl;
    }
}

int callback(void* data, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    std::cout << std::endl;
    return 0;
}

// 全局向量用于在回调中收集所有key（也可使用lambda捕获，此处为兼容C回调简化处理）
static std::vector<int> g_allKeys;

// 收集所有key的回调函数
static int collectKeysCallback(void* data, int argc, char**argv, char** azColName) {
    // 假设key列是整数类型，取第一列（key列）的值
    if (argc > 0 && argv[0] != nullptr) {
        g_allKeys.push_back(std::stoi(argv[0]));
    }
    return 0;
}

void queryData(sqlite3* db, int key) {
    std::string sql = "SELECT * FROM KeyValue WHERE key = " + std::to_string(key) + ";";
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), callback, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

void updateData(sqlite3* db, int key, const std::string& value) {
    std::string sql = "UPDATE KeyValue SET value = '" + value + "' WHERE key = " + std::to_string(key) + ";";
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Data updated successfully." << std::endl;
    }
}

// 新增：查询所有key的接口，返回包含所有key的向量
std::vector<int> getAllKeys(sqlite3* db) {
    g_allKeys.clear(); // 清空全局向量，避免残留数据
    const char* sql = "SELECT key FROM KeyValue;"; // 仅查询key列
    char* errMsg = nullptr;
    
    if (sqlite3_exec(db, sql, collectKeysCallback, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error (getAllKeys): " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    
    return g_allKeys; // 返回收集到的所有key
}

int main() {
    sqlite3* db = initDatabase("test.db");
    if (db == nullptr) {
        return 1;
    }

    createTable(db);

    // 插入数据
    insertData(db, 1, "Value1");
    insertData(db, 2, "Value2");
    std::cout << "===============" << std::endl;

    // 演示查询所有key
    std::vector<int> allKeys = getAllKeys(db);
    std::cout << "\nAll keys in table:" << std::endl;
    for (int key : allKeys) {
        std::cout << "key = " << key << std::endl;
    }
    std::cout << "===============" << std::endl;

    // 查询数据
    queryData(db, 1);
    std::cout << "===============" << std::endl;

    // 更新数据
    updateData(db, 1, "NewValue1");
    std::cout << "===============" << std::endl;

    // 再次查询数据
    queryData(db, 1);
    std::cout << "===============" << std::endl;

    // 删除数据
    deleteData(db, 1);
    std::cout << "===============" << std::endl;

    // 再次查询数据
    queryData(db, 1);
    std::cout << "===============" << std::endl;

    // 关闭数据库连接
    sqlite3_close(db);

    return 0;
}