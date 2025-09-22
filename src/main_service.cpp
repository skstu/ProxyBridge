// ProxyBridge 主程序 - 支持 Windows 服务和控制台模式
#include <iostream>
#include <string>
#include <memory>
#include <csignal>
#include <thread>

#ifdef _WIN32
#include "windows_service.hpp"
#endif

#include "proxy/proxy_server.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>

using namespace proxy;

class ProxyApplication {
private:
    std::unique_ptr<boost::asio::io_context> m_io_context;
    std::shared_ptr<proxy_server> m_server;
    bool m_running = false;

public:
    ProxyApplication() : m_io_context(std::make_unique<boost::asio::io_context>()) {}

    void Initialize(const std::string& config_file = "") {
        // 创建代理服务器配置
        proxy_server_option options;
        
        // 基本配置
        options.listens_.emplace_back(tcp::endpoint(tcp::v4(), 1080), false); // SOCKS5
        options.listens_.emplace_back(tcp::endpoint(tcp::v4(), 8080), false); // HTTP
        
        // 添加用户认证
        options.auth_users_.emplace_back("admin", "ProxyBridge2024!");
        options.auth_users_.emplace_back("user", "UserPass123");
        
        // 安全配置
        options.scramble_ = true;
        options.noise_length_ = 4096;
        options.disable_insecure_ = false;
        
        // 超时配置
        options.tcp_timeout_ = 300;
        options.udp_timeout_ = 60;
        
        // 创建代理服务器
        m_server = proxy_server::make(m_io_context->get_executor(), std::move(options));
        
        std::cout << "ProxyBridge initialized successfully!" << std::endl;
        std::cout << "Listening on:" << std::endl;
        std::cout << "  SOCKS5: 0.0.0.0:1080" << std::endl;
        std::cout << "  HTTP:   0.0.0.0:8080" << std::endl;
        std::cout << "Authentication: admin/ProxyBridge2024!, user/UserPass123" << std::endl;
        std::cout << "Scramble encryption: Enabled" << std::endl;
    }

    void Start() {
        if (!m_server) {
            std::cerr << "Server not initialized!" << std::endl;
            return;
        }

        m_running = true;
        m_server->start();
        
        std::cout << "ProxyBridge started successfully!" << std::endl;
        
        // 运行 IO 上下文
        m_io_context->run();
    }

    void Stop() {
        if (m_server) {
            m_server->close();
            std::cout << "ProxyBridge stopped." << std::endl;
        }
        
        if (m_io_context) {
            m_io_context->stop();
        }
        
        m_running = false;
    }

    bool IsRunning() const { return m_running; }
};

// 全局应用实例
static std::unique_ptr<ProxyApplication> g_app;

// 信号处理
void SignalHandler(int signal) {
    std::cout << "\nReceived signal " << signal << ", shutting down..." << std::endl;
    if (g_app) {
        g_app->Stop();
    }
}

#ifdef _WIN32
// Windows 服务处理函数
void ServiceStartHandler() {
    try {
        g_app = std::make_unique<ProxyApplication>();
        g_app->Initialize();
        g_app->Start();
    }
    catch (const std::exception& e) {
        std::cerr << "Service start error: " << e.what() << std::endl;
    }
}

void ServiceStopHandler() {
    if (g_app) {
        g_app->Stop();
        g_app.reset();
    }
}
#endif

void PrintBanner() {
    std::cout << R"(
╔═══════════════════════════════════════════════════════════════╗
║                         ProxyBridge                           ║
║                  高性能代理服务器 v2.0                          ║
║                                                               ║
║  功能特性:                                                     ║
║  ✓ SOCKS4/5 + HTTP/HTTPS 代理                                 ║
║  ✓ SSL/TLS 加密 + Scramble 混淆                               ║
║  ✓ 用户认证 + 地区过滤                                         ║
║  ✓ 流量伪装 + 噪声注入                                         ║
║  ✓ Windows 服务支持                                           ║
╚═══════════════════════════════════════════════════════════════╝
)" << std::endl;
}

int main(int argc, char* argv[]) {
    PrintBanner();

#ifdef _WIN32
    // Windows 服务模式
    if (argc > 1) {
        std::string arg = argv[1];
        
        WindowsService service("ProxyBridge", "ProxyBridge 高性能代理服务器");
        
        if (arg == "--install") {
            if (!ServiceUtils::IsElevated()) {
                std::cerr << "Error: Administrator privileges required for service installation!" << std::endl;
                std::cerr << "Please run as administrator." << std::endl;
                return 1;
            }
            
            char path[MAX_PATH];
            GetModuleFileNameA(nullptr, path, MAX_PATH);
            std::string servicePath = std::string(path) + " --service";
            
            if (service.InstallService(servicePath)) {
                std::cout << "Service installed successfully!" << std::endl;
                std::cout << "Use 'sc start ProxyBridge' to start the service." << std::endl;
                return 0;
            } else {
                std::cerr << "Failed to install service!" << std::endl;
                return 1;
            }
        }
        else if (arg == "--uninstall") {
            if (!ServiceUtils::IsElevated()) {
                std::cerr << "Error: Administrator privileges required!" << std::endl;
                return 1;
            }
            
            if (service.UninstallService()) {
                std::cout << "Service uninstalled successfully!" << std::endl;
                return 0;
            } else {
                std::cerr << "Failed to uninstall service!" << std::endl;
                return 1;
            }
        }
        else if (arg == "--service") {
            // 作为服务运行
            service.SetServiceHandler(ServiceStartHandler, ServiceStopHandler);
            return service.RunAsService() ? 0 : 1;
        }
        else if (arg == "--console") {
            // 强制控制台模式
            // 继续执行下面的控制台逻辑
        }
        else if (arg == "--help" || arg == "-h") {
            ServiceUtils::PrintUsage();
            return 0;
        }
        else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            ServiceUtils::PrintUsage();
            return 1;
        }
    }
    else if (ServiceUtils::IsRunningAsService()) {
        // 自动检测到在服务环境中运行
        WindowsService service("ProxyBridge", "ProxyBridge 高性能代理服务器");
        service.SetServiceHandler(ServiceStartHandler, ServiceStopHandler);
        return service.RunAsService() ? 0 : 1;
    }
#endif

    // 控制台模式
    try {
        // 设置信号处理
        std::signal(SIGINT, SignalHandler);
        std::signal(SIGTERM, SignalHandler);
#ifdef _WIN32
        std::signal(SIGBREAK, SignalHandler);
#endif

        // 创建并启动应用
        g_app = std::make_unique<ProxyApplication>();
        g_app->Initialize();
        
        std::cout << "\nPress Ctrl+C to stop the server..." << std::endl;
        
        g_app->Start();
        
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}