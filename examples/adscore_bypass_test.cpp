// adscore_bypass_test.cpp
// AdScore绕过测试工具

#include "proxy/proxy_server.hpp"
#include <iostream>
#include <thread>

void print_test_header() {
    std::cout << "🛡️ ==== AdScore检测绕过测试工具 ====" << std::endl;
    std::cout << "🎯 目标: 通过AdScore指纹检测" << std::endl;
    std::cout << "🔧 方法: 多种透明化技术" << std::endl;
    std::cout << std::endl;
}

void print_test_options() {
    std::cout << "📋 测试选项:" << std::endl;
    std::cout << "1. 标准SOCKS5代理 (基准测试)" << std::endl;
    std::cout << "2. 超级透明SOCKS5代理" << std::endl;
    std::cout << "3. 仅HTTP CONNECT代理" << std::endl;
    std::cout << "4. 零检测特征模式" << std::endl;
    std::cout << "5. 上游代理链模式" << std::endl;
    std::cout << std::endl;
    std::cout << "请选择测试模式 (1-5): ";
}

proxy::proxy_server_option create_standard_config() {
    proxy::proxy_server_option opt;
    
    // 基本配置
    opt.listens_.push_back({net::ip::tcp::endpoint(net::ip::tcp::v4(), 1080), false});
    
    std::cout << "📡 标准SOCKS5代理配置" << std::endl;
    std::cout << "🔌 端口: 1080" << std::endl;
    std::cout << "⚠️  预期结果: 可能被AdScore检测" << std::endl;
    
    return opt;
}

proxy::proxy_server_option create_ultra_transparent_config() {
    proxy::proxy_server_option opt;
    
    // 基本配置
    opt.listens_.push_back({net::ip::tcp::endpoint(net::ip::tcp::v4(), 1081), false});
    
    // 启用超级透明模式
    opt.ultra_transparent_mode_ = true;
    
    // 禁用可能的检测特征
    opt.scramble_ = false;
    opt.tcp_rate_limit_ = -1;  // 无速率限制
    
    std::cout << "🔧 超级透明SOCKS5代理配置" << std::endl;
    std::cout << "🔌 端口: 1081" << std::endl;
    std::cout << "✨ 特征: 65KB缓冲区，零检测回调" << std::endl;
    std::cout << "🎯 预期结果: 显著提升通过率" << std::endl;
    
    return opt;
}

proxy::proxy_server_option create_http_only_config() {
    proxy::proxy_server_option opt;
    
    // 基本配置
    opt.listens_.push_back({net::ip::tcp::endpoint(net::ip::tcp::v4(), 8080), false});
    
    // 禁用SOCKS，仅HTTP
    opt.disable_socks_ = true;
    opt.force_http_connect_ = true;
    
    std::cout << "🌐 仅HTTP CONNECT代理配置" << std::endl;
    std::cout << "🔌 端口: 8080" << std::endl;
    std::cout << "✨ 特征: 纯HTTP协议，避免SOCKS检测" << std::endl;
    std::cout << "🎯 预期结果: 高通过率" << std::endl;
    
    return opt;
}

proxy::proxy_server_option create_zero_detection_config() {
    proxy::proxy_server_option opt;
    
    // 基本配置
    opt.listens_.push_back({net::ip::tcp::endpoint(net::ip::tcp::v4(), 1082), false});
    
    // 零检测特征模式
    opt.disable_all_detection_ = true;
    opt.ultra_transparent_mode_ = true;
    
    // 禁用所有可能暴露特征的功能
    opt.scramble_ = false;
    opt.tcp_rate_limit_ = -1;
    opt.disable_udp_ = true;
    
    std::cout << "🚫 零检测特征模式配置" << std::endl;
    std::cout << "🔌 端口: 1082" << std::endl;
    std::cout << "✨ 特征: 完全禁用检测，直接透传" << std::endl;
    std::cout << "🎯 预期结果: 最高通过率" << std::endl;
    
    return opt;
}

proxy::proxy_server_option create_upstream_proxy_config() {
    proxy::proxy_server_option opt;
    
    // 基本配置
    opt.listens_.push_back({net::ip::tcp::endpoint(net::ip::tcp::v4(), 1083), false});
    
    // 配置上游代理 (示例 - 需要用户提供真实代理)
    std::cout << "🔗 配置上游代理链" << std::endl;
    std::cout << "📝 请输入上游代理地址 (格式: socks5://user:pass@host:port): ";
    
    std::string upstream_proxy;
    std::getline(std::cin, upstream_proxy);
    
    if (!upstream_proxy.empty()) {
        opt.proxy_pass_ = upstream_proxy;
        opt.ultra_transparent_mode_ = true;
        
        std::cout << "🔌 端口: 1083" << std::endl;
        std::cout << "✨ 特征: 代理链 + 超级透明" << std::endl;
        std::cout << "🎯 预期结果: 最佳反检测效果" << std::endl;
    } else {
        std::cout << "❌ 未配置上游代理，使用标准模式" << std::endl;
        return create_standard_config();
    }
    
    return opt;
}

void print_test_instructions(int mode, int port) {
    std::cout << std::endl;
    std::cout << "🧪 ===== 测试说明 =====" << std::endl;
    
    // 浏览器配置
    std::cout << "📋 浏览器配置:" << std::endl;
    if (mode == 3) {
        std::cout << "Chrome: --proxy-server=http://127.0.0.1:" << port << std::endl;
        std::cout << "Firefox: HTTP代理 127.0.0.1:" << port << std::endl;
    } else {
        std::cout << "Chrome: --proxy-server=socks5://127.0.0.1:" << port << std::endl;
        std::cout << "Firefox: SOCKS v5 127.0.0.1:" << port << std::endl;
    }
    std::cout << std::endl;
    
    // 测试步骤
    std::cout << "🔬 测试步骤:" << std::endl;
    std::cout << "1. 首先直连测试: https://adscore.com/demo/" << std::endl;
    std::cout << "   记录直连的检测结果作为基准" << std::endl;
    std::cout << "2. 配置浏览器使用代理" << std::endl;
    std::cout << "3. 再次访问: https://adscore.com/demo/" << std::endl;
    std::cout << "4. 对比检测结果" << std::endl;
    std::cout << std::endl;
    
    // 判断标准
    std::cout << "✅ 成功标准:" << std::endl;
    std::cout << "- 浏览器指纹检测与直连结果一致" << std::endl;
    std::cout << "- IP检测可能不同(这是正常的)" << std::endl;
    std::cout << "- 没有\"Proxy Detection\"警告" << std::endl;
    std::cout << std::endl;
    
    std::cout << "🚀 服务器已启动，按 Ctrl+C 停止..." << std::endl;
}

int main() {
    try {
        print_test_header();
        print_test_options();
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(); // 清除输入缓冲区
        
        net::io_context io_context;
        proxy::proxy_server_option opt;
        int port = 1080;
        
        switch (choice) {
            case 1:
                opt = create_standard_config();
                port = 1080;
                break;
            case 2:
                opt = create_ultra_transparent_config();
                port = 1081;
                break;
            case 3:
                opt = create_http_only_config();
                port = 8080;
                break;
            case 4:
                opt = create_zero_detection_config();
                port = 1082;
                break;
            case 5:
                opt = create_upstream_proxy_config();
                port = 1083;
                break;
            default:
                std::cout << "❌ 无效选择，使用标准配置" << std::endl;
                opt = create_standard_config();
                port = 1080;
                break;
        }
        
        auto server = proxy::proxy_server::make(io_context.get_executor(), std::move(opt));
        server->start();
        
        print_test_instructions(choice, port);
        
        io_context.run();
        
    } catch (std::exception& e) {
        std::cerr << "❌ 错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

/*
测试结果分析指南:

1. 如果标准模式被检测，超级透明模式通过
   → 说明我们的优化有效

2. 如果所有SOCKS5模式都被检测，HTTP模式通过
   → 说明问题在SOCKS5协议本身

3. 如果所有本地代理都被检测，上游代理链通过
   → 说明需要IP层面的绕过

4. 如果所有模式都被检测
   → 可能需要更深层的网络层绕过技术
*/