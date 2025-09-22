// http_connect_proxy.cpp
// HTTP CONNECT透明代理 - 避免SOCKS5检测特征

#include "proxy/proxy_server.hpp"
#include <iostream>
#include <regex>

// HTTP CONNECT专用会话类
class http_connect_session : public std::enable_shared_from_this<http_connect_session> {
public:
    http_connect_session(boost::asio::io_context& io_context)
        : client_socket_(io_context)
        , server_socket_(io_context) {}

    boost::asio::ip::tcp::socket& client_socket() {
        return client_socket_;
    }

    void start() {
        read_http_request();
    }

private:
    void read_http_request() {
        auto self = shared_from_this();
        
        boost::asio::async_read_until(client_socket_, request_buffer_, "\r\n\r\n",
            [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
                if (!ec) {
                    process_connect_request(bytes_transferred);
                }
            });
    }

    void process_connect_request(std::size_t bytes_transferred) {
        auto self = shared_from_this();
        
        std::istream request_stream(&request_buffer_);
        std::string method, target, version;
        request_stream >> method >> target >> version;

        if (method != "CONNECT") {
            send_error_response("405 Method Not Allowed");
            return;
        }

        // 解析目标地址
        size_t colon_pos = target.find(':');
        if (colon_pos == std::string::npos) {
            send_error_response("400 Bad Request");
            return;
        }

        std::string host = target.substr(0, colon_pos);
        std::string port = target.substr(colon_pos + 1);

        std::cout << "🎯 CONNECT请求: " << host << ":" << port << std::endl;

        // 连接到目标服务器
        boost::asio::ip::tcp::resolver resolver(client_socket_.get_executor());
        resolver.async_resolve(host, port,
            [this, self](boost::system::error_code ec, 
                         boost::asio::ip::tcp::resolver::results_type results) {
                if (!ec) {
                    boost::asio::async_connect(server_socket_, results,
                        [this, self](boost::system::error_code ec, 
                                     boost::asio::ip::tcp::endpoint) {
                            if (!ec) {
                                send_connect_response();
                            } else {
                                send_error_response("502 Bad Gateway");
                            }
                        });
                } else {
                    send_error_response("502 Bad Gateway");
                }
            });
    }

    void send_connect_response() {
        auto self = shared_from_this();
        
        std::string response = "HTTP/1.1 200 Connection established\r\n\r\n";
        
        boost::asio::async_write(client_socket_, boost::asio::buffer(response),
            [this, self](boost::system::error_code ec, std::size_t) {
                if (!ec) {
                    std::cout << "✅ 连接建立，开始透明传输" << std::endl;
                    start_tunnel();
                }
            });
    }

    void send_error_response(const std::string& status) {
        std::string response = "HTTP/1.1 " + status + "\r\n\r\n";
        boost::asio::async_write(client_socket_, boost::asio::buffer(response),
            [this](boost::system::error_code, std::size_t) {
                // 连接将被关闭
            });
    }

    void start_tunnel() {
        // 开始双向数据传输 - 完全透明
        auto self = shared_from_this();
        
        // 客户端 -> 服务器
        tunnel_data(client_socket_, server_socket_, client_buffer_);
        
        // 服务器 -> 客户端
        tunnel_data(server_socket_, client_socket_, server_buffer_);
    }

    void tunnel_data(boost::asio::ip::tcp::socket& from, 
                     boost::asio::ip::tcp::socket& to, 
                     std::array<char, 16384>& buffer) {
        auto self = shared_from_this();
        
        from.async_read_some(boost::asio::buffer(buffer),
            [this, self, &from, &to, &buffer](boost::system::error_code ec, std::size_t bytes) {
                if (!ec && bytes > 0) {
                    boost::asio::async_write(to, boost::asio::buffer(buffer, bytes),
                        [this, self, &from, &to, &buffer](boost::system::error_code ec, std::size_t) {
                            if (!ec) {
                                // 继续传输
                                tunnel_data(from, to, buffer);
                            }
                        });
                }
                // 如果出错，连接会自然关闭
            });
    }

private:
    boost::asio::ip::tcp::socket client_socket_;
    boost::asio::ip::tcp::socket server_socket_;
    boost::asio::streambuf request_buffer_;
    std::array<char, 16384> client_buffer_;  // 16KB - Chrome默认
    std::array<char, 16384> server_buffer_;
};

class http_connect_server {
public:
    http_connect_server(boost::asio::io_context& io_context, unsigned short port)
        : io_context_(io_context)
        , acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {}

    void start() {
        accept_connections();
    }

private:
    void accept_connections() {
        auto new_session = std::make_shared<http_connect_session>(io_context_);
        
        acceptor_.async_accept(new_session->client_socket(),
            [this, new_session](boost::system::error_code ec) {
                if (!ec) {
                    std::cout << "📡 新连接建立" << std::endl;
                    new_session->start();
                }
                
                // 继续接受新连接
                accept_connections();
            });
    }

private:
    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        
        std::cout << "🚀 HTTP CONNECT透明代理服务器" << std::endl;
        std::cout << "📡 监听端口: 8080" << std::endl;
        std::cout << "🎯 协议: HTTP CONNECT (避免SOCKS5检测)" << std::endl;
        std::cout << "🔧 特征: 完全透明传输" << std::endl;
        std::cout << std::endl;
        
        http_connect_server server(io_context, 8080);
        server.start();
        
        std::cout << "✅ 服务器已启动，配置方法:" << std::endl;
        std::cout << "Chrome: --proxy-server=http://127.0.0.1:8080" << std::endl;
        std::cout << "Firefox: HTTP代理 127.0.0.1:8080" << std::endl;
        std::cout << std::endl;
        
        std::cout << "🧪 测试步骤:" << std::endl;
        std::cout << "1. 配置浏览器使用HTTP代理" << std::endl;
        std::cout << "2. 访问 https://adscore.com/demo/" << std::endl;
        std::cout << "3. 对比与直连的检测结果" << std::endl;
        std::cout << std::endl;
        
        std::cout << "按 Ctrl+C 停止服务..." << std::endl;
        
        io_context.run();
        
    } catch (std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
    }
    
    return 0;
}

/*
HTTP CONNECT相比SOCKS5的优势:

1. 协议透明度更高
   - 标准HTTP协议，浏览器原生支持
   - 不需要特殊的SOCKS库

2. 检测难度更大
   - HTTP CONNECT是合法的HTTP方法
   - 很多企业代理都使用此协议

3. 握手更简单
   - 只需要一次HTTP请求/响应
   - 立即进入透明隧道模式

4. 更接近直连
   - 建立连接后完全是原始TCP流
   - 没有额外的协议包装

测试结果对比:
- 如果HTTP CONNECT版本通过检测 → 问题在SOCKS5协议
- 如果仍被检测 → 问题在代理本身的存在
*/