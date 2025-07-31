#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509.h>

#define ROOT_CERT "/home/yaoxingwei0382/my/cpp_learn/samples/openssl/cert/root_cert.pem"
#define SERVER_CERT "/home/yaoxingwei0382/my/cpp_learn/samples/openssl/cert/server_cert.pem"
#define SERVER_PRIVATE "/home/yaoxingwei0382/my/cpp_learn/samples/openssl/cert/server_private.pem"
// 错误处理函数
void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

// 创建TCP套接字
int createSocket(int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("创建套接字失败");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    // 绑定端口
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("绑定端口失败");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 监听连接
    if (listen(sockfd, 5) < 0) {
        perror("监听失败");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

// 初始化OpenSSL库
void initOpenSSL() {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
}

// 创建SSL上下文
SSL_CTX* createSSLContext(bool isServer, const std::string& certFile, const std::string& keyFile) {
    const SSL_METHOD* method;
    SSL_CTX* ctx;

    if (isServer) {
        // 服务器使用TLS方法
        method = TLS_server_method();
    } else {
        // 客户端使用TLS方法
        method = TLS_client_method();
    }

    ctx = SSL_CTX_new(method);
    if (!ctx) {
        handleErrors();
    }

    // 服务器需要加载证书和私钥
    if (isServer) {
        // 加载服务器证书
        if (SSL_CTX_use_certificate_file(ctx, certFile.c_str(), SSL_FILETYPE_PEM) <= 0) {
            handleErrors();
        }

        // 加载服务器私钥
        if (SSL_CTX_use_PrivateKey_file(ctx, keyFile.c_str(), SSL_FILETYPE_PEM) <= 0) {
            handleErrors();
        }

        // 验证私钥与证书是否匹配
        if (!SSL_CTX_check_private_key(ctx)) {
            fprintf(stderr, "私钥与证书不匹配\n");
            handleErrors();
        }
    } else {
        // 客户端验证服务器证书
        SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, nullptr);
        
        // 加载信任的根证书
        if (SSL_CTX_load_verify_locations(ctx, ROOT_CERT, nullptr) <= 0) {
            handleErrors();
        }
    }

    return ctx;
}

// 服务器处理函数
void handleClient(int clientSock, SSL_CTX* ctx) {
    SSL* ssl = SSL_new(ctx);
    if (!ssl) {
        handleErrors();
    }

    // 将SSL与套接字关联
    SSL_set_fd(ssl, clientSock);

    // 接受SSL连接（完成握手、身份验证和密钥交换）
    if (SSL_accept(ssl) <= 0) {
        handleErrors();
    } else {
        std::cout << "\n服务器: SSL握手成功" << std::endl;

        // 获取并打印客户端验证的服务器证书信息
        X509* cert = SSL_get_peer_certificate(ssl);
        if (cert) {
            char* subject = X509_NAME_oneline(X509_get_subject_name(cert), nullptr, 0);
            std::cout << "服务器证书主题: " << subject << std::endl;
            OPENSSL_free(subject);
            X509_free(cert);
        }

        // 接收客户端加密数据
        char buffer[1024] = {0};
        int bytesRead = SSL_read(ssl, buffer, sizeof(buffer) - 1);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            std::cout << "服务器: 接收到加密数据: " << buffer << std::endl;
        }

        // 发送加密响应
        std::string response = "服务器已收到您的消息: \"" + std::string(buffer) + "\"";
        SSL_write(ssl, response.c_str(), response.length());
        std::cout << "服务器: 已发送加密响应" << std::endl;
    }

    // 清理
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(clientSock);
}

// 服务器主函数
void runServer(int port, const std::string& certFile, const std::string& keyFile) {
    initOpenSSL();
    SSL_CTX* ctx = createSSLContext(true, certFile, keyFile);
    int serverSock = createSocket(port);

    std::cout << "服务器: 正在端口 " << port << " 上监听HTTPS连接..." << std::endl;

    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &clientLen);
        
        if (clientSock < 0) {
            perror("接受连接失败");
            continue;
        }

        std::cout << "服务器: 接受来自 " << inet_ntoa(clientAddr.sin_addr) << " 的连接" << std::endl;
        
        // 处理客户端连接（实际应用中应使用线程池）
        std::thread clientThread(handleClient, clientSock, ctx);
        clientThread.detach();
    }

    // 清理（实际中不会执行到这里）
    close(serverSock);
    SSL_CTX_free(ctx);
    EVP_cleanup();
}

// 客户端主函数
void runClient(const std::string& host, int port) {
    initOpenSSL();
    
    // 创建客户端SSL上下文
    SSL_CTX* ctx = createSSLContext(false, "", "");
    
    // 创建套接字并连接到服务器
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("创建套接字失败");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    // 解析主机名
    if (inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr) <= 0) {
        perror("解析主机失败");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 连接服务器
    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("连接服务器失败");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 创建SSL对象
    SSL* ssl = SSL_new(ctx);
    if (!ssl) {
        handleErrors();
    }
    SSL_set_fd(ssl, sockfd);

    // 建立SSL连接（完成握手、身份验证和密钥交换）
    if (SSL_connect(ssl) <= 0) {
        handleErrors();
    } else {
        std::cout << "\n客户端: SSL握手成功" << std::endl;

        // 验证服务器证书
        X509* cert = SSL_get_peer_certificate(ssl);
        if (cert) {
            char* subject = X509_NAME_oneline(X509_get_subject_name(cert), nullptr, 0);
            char* issuer = X509_NAME_oneline(X509_get_issuer_name(cert), nullptr, 0);
            
            std::cout << "服务器证书主题: " << subject << std::endl;
            std::cout << "服务器证书颁发者: " << issuer << std::endl;
            
            OPENSSL_free(subject);
            OPENSSL_free(issuer);
            X509_free(cert);
        }

        // 发送加密数据
        std::string message = "Hello, HTTPS World!";
        SSL_write(ssl, message.c_str(), message.length());
        std::cout << "客户端: 已发送加密数据: " << message << std::endl;

        // 接收加密响应
        char buffer[1024] = {0};
        int bytesRead = SSL_read(ssl, buffer, sizeof(buffer) - 1);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            std::cout << "客户端: 接收到加密响应: " << buffer << std::endl;
        }
    }

    // 清理
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sockfd);
    SSL_CTX_free(ctx);
    EVP_cleanup();
}

int main(int argc, char* argv[]) {
    // 证书和密钥文件路径
    const std::string serverCert = SERVER_CERT;
    const std::string serverKey = SERVER_PRIVATE;
    const int port = 4433;  // HTTPS默认端口是443，这里用4433避免需要root权限
    
    if (argc != 2) {
        std::cerr << "用法: " << argv[0] << " [server|client]" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    if (mode == "server") {
        runServer(port, serverCert, serverKey);
    } else if (mode == "client") {
        runClient("127.0.0.1", port);
    } else {
        std::cerr << "无效模式，请使用 'server' 或 'client'" << std::endl;
        return 1;
    }

    return 0;
}