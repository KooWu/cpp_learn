#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>  // C++17及以上
#include <cstdio>      // 传统C文件操作
#include <sys/stat.h>  // 目录操作（POSIX）
#include <unistd.h>

using namespace std;
namespace fs = std::filesystem;

// ==================== 文件操作 ====================

// 1. 文件读写（C++流）
void fileStreamOperations() {
    cout << "\n=== 文件流操作 ===" << endl;
    
    // 写入文件
    ofstream outFile("data.txt");
    if (outFile.is_open()) {
        outFile << "Hello, File!" << endl;
        outFile << 12345 << endl;
        outFile.close();
        cout << "文件写入成功" << endl;
    }

    // 读取文件
    ifstream inFile("data.txt");
    string line;
    while (getline(inFile, line)) {
        cout << "读取: " << line << endl;
    }
    inFile.close();
}

// 2. 文件操作（传统C风格）
void cStyleFileOperations() {
    cout << "\n=== 传统C文件操作 ===" << endl;
    
    // 写入文件
    FILE* file = fopen("data_c.txt", "w");
    if (file) {
        fprintf(file, "Hello from C!\n");
        fclose(file);
        cout << "C风格文件写入成功" << endl;
    }

    // 读取文件
    file = fopen("data_c.txt", "r");
    if (file) {
        char buffer[100];
        while (fgets(buffer, sizeof(buffer), file)) {
            cout << "C读取: " << buffer;
        }
        fclose(file);
    }
}

// ==================== 目录操作（C++17 filesystem） ====================

// 3. 创建目录
void createDirectory() {
    cout << "\n=== 创建目录 ===" << endl;
    
    fs::path dirPath("test_dir");
    if (fs::create_directory(dirPath)) {
        cout << "目录创建成功: " << dirPath << endl;
    } else {
        cout << "目录已存在或创建失败" << endl;
    }
}

// 4. 递归创建目录
void createDirectoriesRecursive() {
    cout << "\n=== 递归创建目录 ===" << endl;
    
    fs::path nestedPath("test_dir/nested/subdir");
    if (fs::create_directories(nestedPath)) {
        cout << "递归目录创建成功: " << nestedPath << endl;
    }
}

// 5. 目录遍历
void directoryTraversal() {
    cout << "\n=== 目录遍历 ===" << endl;
    
    fs::path dirPath("test_dir");
    if (fs::exists(dirPath) && fs::is_directory(dirPath)) {
        cout << "目录内容:" << endl;
        for (const auto& entry : fs::directory_iterator(dirPath)) {
            cout << (entry.is_directory() ? "[目录] " : "[文件] ") 
                 << entry.path() << endl;
        }
    }
}

// 6. 文件和目录信息
void fileAndDirectoryInfo() {
    cout << "\n=== 文件和目录信息 ===" << endl;
    
    fs::path filePath("data.txt");
    fs::path dirPath("test_dir");
    
    if (fs::exists(filePath)) {
        cout << "文件大小: " << fs::file_size(filePath) << " 字节" << endl;
    }
    
    if (fs::exists(dirPath)) {
        cout << "目录存在: " << dirPath << endl;
    }
}

// 7. 文件和目录删除
void deleteFilesAndDirectories() {
    cout << "\n=== 删除文件和目录 ===" << endl;
    
    fs::path filePath("data_c.txt");
    fs::path dirPath("test_dir");
    
    if (fs::exists(filePath)) {
        fs::remove(filePath);
        cout << "文件已删除: " << filePath << endl;
    }
    
    if (fs::exists(dirPath)) {
        fs::remove_all(dirPath);  // 递归删除
        cout << "目录已删除: " << dirPath << endl;
    }
}

// ==================== 跨平台目录操作（传统方式） ====================

// 8. 跨平台目录创建（传统方式）
void createDirectoryLegacy() {
    cout << "\n=== 传统方式创建目录 ===" << endl;
    
    #ifdef _WIN32
        if (_mkdir("legacy_dir") == 0) {
            cout << "Windows目录创建成功" << endl;
        }
    #else
        if (mkdir("legacy_dir", 0777) == 0) {
            cout << "Unix目录创建成功" << endl;
        }
    #endif
}

// 9. 获取当前工作目录
void getCurrentDirectory() {
    cout << "\n=== 当前工作目录 ===" << endl;
    
    #ifdef _WIN32
        char buffer[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, buffer);
        cout << "Windows当前目录: " << buffer << endl;
    #else
        char buffer[1024];
        if (getcwd(buffer, sizeof(buffer)) != nullptr) {
            cout << "Unix当前目录: " << buffer << endl;
        }
    #endif
}

// ==================== 主函数 ====================

int main() {
    fileStreamOperations();
    cStyleFileOperations();
    createDirectory();
    createDirectoriesRecursive();
    directoryTraversal();
    fileAndDirectoryInfo();
    deleteFilesAndDirectories();
    createDirectoryLegacy();
    getCurrentDirectory();
    
    // 清理
    fs::remove("data.txt");
    fs::remove_all("legacy_dir");
    
    return 0;
}