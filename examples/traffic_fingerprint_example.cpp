// traffic_fingerprint_example.cpp
// 演示如何使用流量指纹回调功能

#include "proxy/proxy_server.hpp"
#include <iostream>
#include <iomanip>

// 自定义的流量指纹回调处理器
class my_traffic_fingerprint_handler : public proxy::traffic_fingerprint_interface {
public:
    void on_client_hello(const uint8_t* data, size_t size) override {
        std::cout << "TLS ClientHello detected, size: " << size << " bytes" << std::endl;
        std::cout << "First few bytes: ";
        for (size_t i = 0; i < std::min(size_t(16), size); ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') 
                      << static_cast<int>(data[i]) << " ";
        }
        std::cout << std::dec << std::endl;
    }
    
    void on_tcp_window(uint32_t window_size, bool is_client) override {
        std::cout << "TCP Window Size: " << window_size 
                  << " (from " << (is_client ? "client" : "server") << ")" << std::endl;
    }
    
    void on_ttl_value(uint8_t ttl_value, bool is_client) override {
        std::cout << "TTL Value: " << static_cast<int>(ttl_value)
                  << " (from " << (is_client ? "client" : "server") << ")" << std::endl;
    }
    
    void on_tcp_options(const uint8_t* options, size_t size, bool is_client) override {
        std::cout << "TCP Options (" << (is_client ? "client" : "server") 
                  << "), size: " << size << " bytes" << std::endl;
    }
    
    void on_ssl_handshake_complete(const char* cipher_suite, const char* protocol_version) override {
        std::cout << "SSL Handshake Complete:" << std::endl;
        std::cout << "  Cipher Suite: " << cipher_suite << std::endl;
        std::cout << "  Protocol Version: " << protocol_version << std::endl;
    }
    
    void on_packet_timing(uint64_t timestamp_us, size_t packet_size, bool is_outbound) override {
        std::cout << "Packet " << (is_outbound ? "sent" : "received")
                  << " at " << timestamp_us << "us, size: " << packet_size << " bytes" << std::endl;
    }
};

int main() {
    try {
        // 创建IO上下文
        boost::asio::io_context io_context;
        
        // 配置代理服务器选项
        proxy::proxy_server_option options;
        
        // 设置监听地址和端口
        boost::asio::ip::tcp::endpoint listen_endpoint(
            boost::asio::ip::make_address("127.0.0.1"), 1080);
        options.listens_.emplace_back(listen_endpoint, false);
        
        // 创建代理服务器
        auto server = proxy::proxy_server::make(io_context.get_executor(), std::move(options));
        
        // 创建并设置流量指纹回调处理器
        auto fingerprint_handler = std::make_unique<my_traffic_fingerprint_handler>();
        auto& fingerprint_mgr = proxy::traffic_fingerprint_manager::instance();
        fingerprint_mgr.set_interface(fingerprint_handler.get());
        
        // 启动服务器
        server->start();
        
        std::cout << "SOCKS5 Proxy Server with Traffic Fingerprint Callbacks started on 127.0.0.1:1080" << std::endl;
        std::cout << "Press Ctrl+C to stop..." << std::endl;
        
        // 运行IO上下文
        io_context.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

/*
使用说明：

1. 编译并运行此程序
2. 配置浏览器使用 127.0.0.1:1080 作为SOCKS5代理
3. 访问任何HTTPS网站，你将看到详细的流量指纹信息：
   - TLS ClientHello 数据包内容
   - TCP 窗口大小
   - TTL 值
   - SSL/TLS 握手完成信息
   - 数据包时序信息

这些信息可以用于：
- 流量分析和调试
- 检测代理特征
- 网络性能优化
- 安全研究
*/