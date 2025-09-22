// stealth_proxy_config.cpp
// AdScore绕过代理的配置示例

#include "proxy/proxy_server.hpp"
#include <iostream>

// 配置AdScore绕过代理
proxy::proxy_server_option create_adscore_bypass_config() {
    proxy::proxy_server_option opt;
    
    // 基础网络配置
    opt.listens_.push_back({
        net::ip::tcp::endpoint(net::ip::tcp::v4(), 1080), 
        false
    });
    
    // ===== 核心反检测配置 =====
    
    // 1. 启用零检测模式（最高优先级）
    opt.zero_detection_mode_ = true;
    
    // 2. 启用超级透明模式
    opt.ultra_transparent_mode_ = true;
    
    // 3. 启用SOCKS5协议混淆
    opt.socks5_obfuscation_ = true;
    opt.obfuscation_level_ = 5;  // 最高混淆等级
    
    // 4. 启用人类行为模拟
    opt.human_behavior_simulation_ = true;
    
    // 5. 启用浏览器TLS指纹模拟
    opt.browser_tls_mimicry_ = true;
    
    // 6. 启用深度协议隐藏
    opt.stealth_protocol_mode_ = true;
    
    // 7. 启用Web服务器伪装
    opt.web_server_disguise_mode_ = true;
    
    // 8. 启用内容嗅探欺骗
    opt.content_sniffing_deception_ = true;
    
    // ===== 禁用可能暴露特征的功能 =====
    
    // 禁用UDP（减少协议指纹）
    opt.disable_udp_ = true;
    
    // 禁用加密混淆（使用我们自己的混淆）
    opt.scramble_ = false;
    
    // 禁用速率限制（避免流量模式检测）
    opt.tcp_rate_limit_ = -1;
    
    // 禁用日志记录（减少系统调用特征）
    // opt.logs_level_ = "OFF";  // 如果有这个选项
    
    std::cout << "🛡️ AdScore绕过代理配置已加载" << std::endl;
    std::cout << "✅ 零检测模式: 启用" << std::endl;
    std::cout << "✅ 协议混淆等级: 5 (最高)" << std::endl;
    std::cout << "✅ 行为模拟: 启用" << std::endl;
    std::cout << "✅ 多重伪装: 启用" << std::endl;
    
    return opt;
}

// 配置HTTP-only模式（无SOCKS5特征）
proxy::proxy_server_option create_http_only_config() {
    proxy::proxy_server_option opt;
    
    // HTTP代理端口
    opt.listens_.push_back({
        net::ip::tcp::endpoint(net::ip::tcp::v4(), 8080), 
        false
    });
    
    // 完全禁用SOCKS5
    opt.disable_socks5_completely_ = true;
    opt.disable_socks_ = true;
    
    // 启用零检测HTTP转发
    opt.zero_detection_mode_ = true;
    opt.content_sniffing_deception_ = true;
    
    std::cout << "🌐 HTTP-only代理配置已加载" << std::endl;
    std::cout << "✅ SOCKS5协议: 完全禁用" << std::endl;
    std::cout << "✅ 仅HTTP CONNECT: 启用" << std::endl;
    
    return opt;
}

// 配置极限隐蔽模式
proxy::proxy_server_option create_ultimate_stealth_config() {
    proxy::proxy_server_option opt;
    
    // 使用非标准端口
    opt.listens_.push_back({
        net::ip::tcp::endpoint(net::ip::tcp::v4(), 8443), 
        false
    });
    
    // 启用所有隐蔽功能
    opt.zero_detection_mode_ = true;
    opt.ultra_transparent_mode_ = true;
    opt.socks5_obfuscation_ = true;
    opt.obfuscation_level_ = 5;
    opt.human_behavior_simulation_ = true;
    opt.browser_tls_mimicry_ = true;
    opt.stealth_protocol_mode_ = true;
    opt.web_server_disguise_mode_ = true;
    opt.content_sniffing_deception_ = true;
    opt.port_masquerade_mode_ = true;
    opt.protocol_tunnel_mode_ = true;
    opt.nginx_camouflage_ = true;
    
    // 高级配置
    opt.dpi_bypass_mode_ = true;
    opt.disable_all_detection_ = true;
    
    std::cout << "🥷 极限隐蔽模式配置已加载" << std::endl;
    std::cout << "✅ 所有反检测功能: 启用" << std::endl;
    std::cout << "✅ DPI绕过: 启用" << std::endl;
    std::cout << "✅ 协议伪装: 多重模式" << std::endl;
    
    return opt;
}

void print_usage_instructions() {
    std::cout << std::endl;
    std::cout << "📋 使用说明:" << std::endl;
    std::cout << std::endl;
    
    std::cout << "🔧 配置浏览器:" << std::endl;
    std::cout << "Chrome: --proxy-server=socks5://127.0.0.1:1080" << std::endl;
    std::cout << "Firefox: SOCKS v5 代理 127.0.0.1:1080" << std::endl;
    std::cout << std::endl;
    
    std::cout << "🧪 测试步骤:" << std::endl;
    std::cout << "1. 直连访问 https://adscore.com/demo/ (记录基准结果)" << std::endl;
    std::cout << "2. 启用代理后再次访问" << std::endl;
    std::cout << "3. 对比检测结果" << std::endl;
    std::cout << std::endl;
    
    std::cout << "✅ 成功标准:" << std::endl;
    std::cout << "- 浏览器指纹与直连一致" << std::endl;
    std::cout << "- 无\"Proxy Detection\"警告" << std::endl;
    std::cout << "- AdScore评分接近直连结果" << std::endl;
    std::cout << std::endl;
    
    std::cout << "⚠️  注意事项:" << std::endl;
    std::cout << "- IP地址会显示为代理IP（这是正常的）" << std::endl;
    std::cout << "- 首次连接可能稍慢（协议协商）" << std::endl;
    std::cout << "- 部分网站可能有额外的反代理检测" << std::endl;
}

int main() {
    try {
        std::cout << "🛡️ ==== AdScore检测绕过代理服务器 ====" << std::endl;
        std::cout << "🎯 专门设计用于绕过AdScore指纹检测" << std::endl;
        std::cout << std::endl;
        
        std::cout << "请选择配置模式:" << std::endl;
        std::cout << "1. AdScore绕过模式 (推荐)" << std::endl;
        std::cout << "2. HTTP-only模式" << std::endl;
        std::cout << "3. 极限隐蔽模式" << std::endl;
        std::cout << "请输入选择 (1-3): ";
        
        int choice;
        std::cin >> choice;
        
        net::io_context io_context;
        proxy::proxy_server_option opt;
        
        switch (choice) {
            case 1:
                opt = create_adscore_bypass_config();
                break;
            case 2:
                opt = create_http_only_config();
                break;
            case 3:
                opt = create_ultimate_stealth_config();
                break;
            default:
                std::cout << "无效选择，使用默认AdScore绕过模式" << std::endl;
                opt = create_adscore_bypass_config();
                break;
        }
        
        // 创建并启动代理服务器
        auto server = proxy::proxy_server::make(io_context.get_executor(), std::move(opt));
        server->start();
        
        print_usage_instructions();
        
        std::cout << "🚀 代理服务器已启动，按 Ctrl+C 停止..." << std::endl;
        
        // 运行事件循环
        io_context.run();
        
    } catch (std::exception& e) {
        std::cerr << "❌ 错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

/*
编译说明:
g++ -std=c++20 -I../proxy/include stealth_proxy_config.cpp -lboost_system -lboost_coroutine -pthread -o stealth_proxy

性能建议:
1. 在生产环境中，建议先用模式1测试
2. 如果仍被检测，尝试模式3
3. 监控系统资源使用情况
4. 根据目标网站调整混淆等级

高级配置:
- 可以通过修改obfuscation_level_来调整混淆强度
- human_behavior_simulation_增加了随机延迟，可能影响速度
- 多种伪装模式可以组合使用，但会增加复杂性
*/