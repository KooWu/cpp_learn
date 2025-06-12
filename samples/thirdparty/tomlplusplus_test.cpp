#include <iostream>
#include <fstream>
#include <string>
#include <toml++/toml.h>

int main() {
    // 解析 TOML 字符串
    std::string toml_str = R"(
        # 这是一个 TOML 配置文件示例
        
        title = "TOML 示例"
        
        [database]
        hostname = "localhost"
        port = 5432
        username = "admin"
        password = "secret"
        
        [servers]
        [servers.alpha]
        ip = "10.0.0.1"
        dc = "eqdc10"
        
        [servers.beta]
        ip = "10.0.0.2"
        dc = "eqdc10"
        enabled = true
        
        [[clients]]
        name = "客户端 A"
        data = [ 1, 2, 3 ]
        
        [[clients]]
        name = "客户端 B"
        data = [ 4, 5, 6 ]
    )";

    // 解析 TOML 字符串
    try {
        auto data = toml::parse(toml_str);
        
        // 访问和修改数据
        std::cout << "Title: " << data["title"].value<std::string>().value() << std::endl;
        
        // 访问表中的数据
        std::cout << "Database hostname: " << data["database"]["hostname"].value<std::string>().value() << std::endl;
        std::cout << "Database port: " << data["database"]["port"].value<int64_t>().value() << std::endl;
        
        // 修改数据 - 使用 ref() 方法获取可变引用
        data["database"]["port"].ref<int64_t>() = 5433;
        std::cout << "New database port: " << data["database"]["port"].value<int64_t>().value() << std::endl;
        
        // 访问数组表
        std::cout << "\nClients:" << std::endl;
        const auto clients = data["clients"].as_array();
        for (const auto& entry : *clients) {
            const auto& client = *entry.as_table();
            std::cout << "  Name: " << client["name"].value<std::string>().value() << std::endl;
            std::cout << "  Data: ";

            const auto datas = client["data"].as_array();
            for (auto& value : *datas) {
                std::cout << value.value<int>().value() << " ";
            }
            std::cout << std::endl;
        }
        
        // 创建新的 TOML 数据
toml::table new_table;
        new_table.insert("name", "新项目");
        new_table.insert("version", 1.0);
        
        toml::table config;
        config.insert("enabled", true);
        config.insert("timeout", 300);
        
        new_table.insert("config", config);
        
        // 写入文件
        std::ofstream file("new_config.toml");
        file << new_table;
        file.close();
        
        std::cout << "\n新配置已写入 new_config.toml" << std::endl;
        
    } catch (const toml::parse_error& err) {
        std::cerr << "解析错误: " << err.description() << std::endl;
        std::cerr << "位置: " << err.source().begin << std::endl;
        return 1;
    }
    
    return 0;
}
