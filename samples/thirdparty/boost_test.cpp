#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

using boost::asio::ip::tcp;
using boost::system::error_code;

// 会话类：处理客户端连接
class Session : public std::enable_shared_from_this<Session> {
public:
    explicit Session(boost::asio::io_context& io_context)
        : socket_(io_context) {}

    tcp::socket& socket() { return socket_; }

    void start() {
        async_read_until(socket_, buffer_, '\n',
            boost::bind(&Session::handle_read, shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

private:
    void handle_read(const error_code& error, size_t bytes_transferred) {
        if (!error) {
            std::istream is(&buffer_);
            std::string request;
            std::getline(is, request);
            
            // 处理请求并回显
            std::string response = "Server received: " + request + "\n";
            boost::asio::async_write(socket_, boost::asio::buffer(response),
                boost::bind(&Session::handle_write, shared_from_this(),
                    boost::asio::placeholders::error));
        }
    }

    void handle_write(const error_code& error) {
        if (!error) {
            // 继续等待下一个请求
            start();
        }
    }

    tcp::socket socket_;
    boost::asio::streambuf buffer_;
};

// 服务器类：监听连接
class Server {
public:
    Server(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
          io_context_(io_context) {
        start_accept();
    }

private:
    void start_accept() {
        auto new_session = std::make_shared<Session>(io_context_);
        acceptor_.async_accept(new_session->socket(),
            boost::bind(&Server::handle_accept, this, new_session,
                boost::asio::placeholders::error));
    }

    void handle_accept(std::shared_ptr<Session> new_session, const error_code& error) {
        if (!error) {
            new_session->start();
        }
        // 继续接受下一个连接
        start_accept();
    }

    tcp::acceptor acceptor_;
    boost::asio::io_context& io_context_;
};

// 客户端示例
void client_example() {
    try {
        boost::asio::io_context io_context;
        tcp::socket socket(io_context);
        tcp::resolver resolver(io_context);
        
        // 连接到服务器
        boost::asio::connect(socket, resolver.resolve("127.0.0.1", "12345"));
        
        // 发送消息
        std::string message = "Hello, server!\n";
        boost::asio::write(socket, boost::asio::buffer(message));
        
        // 接收响应
        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, '\n');
        std::istream is(&response);
        std::string line;
        std::getline(is, line);
        std::cout << "Client received: " << line << std::endl;
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

int main() {
    try {
        // 启动服务器（在单独线程中运行）
        boost::asio::io_context io_context;
        Server server(io_context, 12345);
        
        std::thread server_thread([&io_context]() {
            io_context.run();
        });
        
        // 运行客户端示例
        client_example();
        
        // 关闭服务器
        io_context.stop();
        server_thread.join();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}