// transparent_proxy_test.cpp
// 完全透明代理测试程序

#include "proxy/proxy_server.hpp"
#include <iostream>

int main() {
    try {
        // 创建IO上下文
        boost::asio::io_context io_context;
        
        // 最简化的代理配置 - 完全透明模式
        proxy::proxy_server_option options;
        
        // 设置监听地址和端口
        boost::asio::ip::tcp::endpoint listen_endpoint(
            boost::asio::ip::make_address("127.0.0.1"), 1080);
        options.listens_.emplace_back(listen_endpoint, false);
        
        // 完全禁用所有检测和特殊处理
        options.disable_http_ = false;       // 允许HTTP
        options.disable_socks_ = false;      // 允许SOCKS
        options.disable_insecure_ = false;   // 允许明文连接
        options.disable_udp_ = false;        // 允许UDP
        
        // 禁用所有可能改变流量特征的功能
        options.scramble_ = false;           // 禁用流量混淆
        options.reuse_port_ = false;         // 禁用端口重用
        options.happyeyeballs_ = true;       // 使用标准连接算法
        
        // 不设置任何上游代理，完全直连
        options.proxy_pass_ = "";
        
        // 不设置任何本地绑定接口
        options.local_ip_ = "";
        
        // 不设置任何认证
        options.auth_users_.clear();
        
        // 不设置任何地区限制
        options.allow_regions_.clear();
        options.deny_regions_.clear();
        
        // 创建代理服务器
        auto server = proxy::proxy_server::make(io_context.get_executor(), std::move(options));
        
        // 启动服务器
        server->start();
        
        std::cout << "🔍 完全透明代理测试服务器已启动" << std::endl;
        std::cout << "📡 监听地址: 127.0.0.1:1080" << std::endl;
        std::cout << "🎯 模式: 完全透明（无任何检测或修改）" << std::endl;
        std::cout << std::endl;
        
        std::cout << "测试步骤:" << std::endl;
        std::cout << "1. 配置Chromium使用此代理: --proxy-server=socks5://127.0.0.1:1080" << std::endl;
        std::cout << "2. 访问AdScore检测页面: https://adscore.com/demo/" << std::endl;
        std::cout << "3. 对比直连和通过代理的检测结果" << std::endl;
        std::cout << std::endl;
        
        std::cout << "预期结果:" << std::endl;
        std::cout << "- 如果代理桥透明度良好，检测结果应该相同" << std::endl;
        std::cout << "- 如果仍被检测，说明需要进一步优化透明度" << std::endl;
        std::cout << std::endl;
        
        std::cout << "按 Ctrl+C 停止测试..." << std::endl;
        
        // 运行IO上下文
        io_context.run();
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

/*
透明度检查清单:

✅ 已移除的可能暴露特征:
- 流量指纹回调和检测
- TCP选项获取和修改
- SSL握手信息获取
- 时序分析和记录
- 智能路由检测逻辑
- 缓冲区大小修改 (改为16KB，与Chromium一致)
- 任何额外的协议头添加

✅ 保持的透明特性:
- 原生boost::asio IO操作
- 系统默认TCP选项
- 标准缓冲区大小
- 直连相同的连接时序
- 完全透明的数据传输

🔍 如果此版本仍被检测，可能的原因:
1. proxy_stream包装层引入了特征
2. 协程调度引入了时序差异  
3. SOCKS5握手过程被检测
4. 需要完全bypass proxy_stream，直接使用raw socket

下一步优化方向:
- 使用raw socket bypass所有包装层
- 实现zero-copy透明传输
- 完全模拟系统网络栈行为
*/