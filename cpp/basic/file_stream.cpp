#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// 1. 标准流示例
void standardStreamsExample() {
    cout << "=== 标准流示例 ===" << endl;
    string name;
    int age;

    cout << "请输入您的姓名: ";
    cin >> name;
    cout << "请输入您的年龄: ";
    cin >> age;

    cout << "您好，" << name << "！您今年" << age << "岁。" << endl;
    cerr << "这是一条错误信息示例。" << endl;
    clog << "这是一条日志信息示例。" << endl;
}

// 2. 文件流示例
void fileStreamsExample() {
    cout << "\n=== 文件流示例 ===" << endl;
    
    // 写入文件
    ofstream outFile("data.txt");
    if (outFile.is_open()) {
        outFile << "Hello, File!" << endl;
        outFile << 12345 << endl;
        outFile << 3.14159 << endl;
        outFile.close();
        cout << "文件写入成功。" << endl;
    } else {
        cerr << "无法打开文件进行写入。" << endl;
    }

    // 读取文件
    ifstream inFile("data.txt");
    if (inFile.is_open()) {
        string line;
        int number;
        double pi;

        getline(inFile, line);
        inFile >> number;
        inFile >> pi;

        cout << "读取内容:" << endl;
        cout << line << endl;
        cout << number << endl;
        cout << pi << endl;
        inFile.close();
    } else {
        cerr << "无法打开文件进行读取。" << endl;
    }
}

// 3. 二进制文件操作示例
struct Person {
    char name[50];
    int age;
};

void binaryFileExample() {
    cout << "\n=== 二进制文件示例 ===" << endl;
    
    // 写入二进制文件
    Person person = {"Alice", 30};
    ofstream binOutFile("person.bin", ios::binary);
    if (binOutFile.is_open()) {
        binOutFile.write(reinterpret_cast<const char*>(&person), sizeof(Person));
        binOutFile.close();
        cout << "二进制文件写入成功。" << endl;
    }

    // 读取二进制文件
    Person readPerson;
    ifstream binInFile("person.bin", ios::binary);
    if (binInFile.is_open()) {
        binInFile.read(reinterpret_cast<char*>(&readPerson), sizeof(Person));
        binInFile.close();
        cout << "从二进制文件读取:" << endl;
        cout << "姓名: " << readPerson.name << endl;
        cout << "年龄: " << readPerson.age << endl;
    }
}

// 4. 文件状态检查示例
void fileStatusExample() {
    cout << "\n=== 文件状态检查示例 ===" << endl;
    ifstream file("nonexistent.txt"); // 尝试打开不存在的文件

    if (!file.is_open()) {
        cout << "文件打开失败！" << endl;
        if (file.fail()) cout << "失败标志已设置" << endl;
        if (file.bad()) cout << "严重错误标志已设置" << endl;
    }

    // 正确打开文件并检查状态
    ofstream testFile("test.txt");
    testFile << "test";
    testFile.close();

    ifstream goodFile("test.txt");
    if (goodFile.good()) {
        cout << "文件正常打开。" << endl;
        goodFile.close();
    }
}

// 5. 文件定位示例
void filePositioningExample() {
    cout << "\n=== 文件定位示例 ===" << endl;
    
    // 创建测试文件
    ofstream testFile("position.txt");
    testFile << "0123456789";
    testFile.close();

    // 读取文件特定位置
    ifstream inFile("position.txt");
    if (inFile.is_open()) {
        // 移动到位置5
        inFile.seekg(5);
        char c;
        inFile.get(c);
        cout << "位置5的字符是: " << c << endl;

        // 从文件末尾向前移动3个位置
        inFile.seekg(-3, ios::end);
        inFile.get(c);
        cout << "倒数第3个字符是: " << c << endl;

        // 获取当前位置
        streampos pos = inFile.tellg();
        cout << "当前位置: " << pos << endl;
        inFile.close();
    }
}

// 6. 字符串流示例
void stringStreamExample() {
    cout << "\n=== 字符串流示例 ===" << endl;
    
    // 字符串格式化
    ostringstream oss;
    string name = "Bob";
    int score = 95;
    oss << "学生: " << name << ", 分数: " << score;
    string result = oss.str();
    cout << "格式化后的字符串: " << result << endl;

    // 字符串解析
    string input = "42 3.14 Hello";
    istringstream iss(input);
    int num;
    double d;
    string word;

    iss >> num >> d >> word;
    cout << "解析结果: " << num << ", " << d << ", " << word << endl;
}

// 7. 文件流异常处理示例
void fileExceptionExample() {
    cout << "\n=== 文件异常处理示例 ===" << endl;
    
    ifstream file;
    // 设置异常触发条件
    file.exceptions(ios::failbit | ios::badbit);

    try {
        file.open("nonexistent.txt");
        cout << "文件打开成功？这不可能！" << endl;
    } catch (const ifstream::failure& e) {
        cerr << "文件操作异常: " << e.what() << endl;
        cerr << "错误码: " << e.code() << endl;
    }
}

int main() {
    standardStreamsExample();
    fileStreamsExample();
    binaryFileExample();
    fileStatusExample();
    filePositioningExample();
    stringStreamExample();
    fileExceptionExample();

    return 0;
}