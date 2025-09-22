#pragma once
#include <vector>
#include <string>
#include <cstring>
#include <cstdint>

// 为 SoftEther VPN Client 优化的 ProxyBridge 配置

// 在 proxy_server_option 中添加 SoftEther 特定的优化设置
struct softether_optimized_config {
    // 针对 SoftEther VPN 流量的特殊配置
    bool enable_softether_mode = true;
    
    // 保持长连接，VPN 需要持续连接
    int keep_alive_interval = 30; // 秒
    
    // 适当的缓冲区大小，优化 VPN 数据传输
    size_t buffer_size = 1024 * 1024; // 1MB
    
    // 针对 VPN 流量的特殊超时设置
    int vpn_tcp_timeout = 300; // 5分钟，比普通代理更长
    
    // 是否启用 VPN 流量识别和优化
    bool vpn_traffic_optimization = true;
};

// 在 proxy_session 中添加 SoftEther 检测和优化
inline bool detect_softether_traffic(const std::vector<std::uint8_t>& data) {
    // SoftEther VPN 协议特征检测
    // SoftEther 使用特殊的协议头标识
    if (data.size() >= 8) {
        // 检查是否为 SoftEther VPN 数据包
        // SoftEther 通常在 HTTPS 伪装模式下工作
        const char* softether_signature = "SE-VPN";
        if (std::memcmp(data.data(), softether_signature, 6) == 0) {
            return true;
        }
        
        // 检查 SoftEther 的 SSL-VPN 模式特征
        if (data[0] == 0x16 && data[1] == 0x03) { // TLS handshake
            // 进一步检查 TLS Client Hello 中的扩展
            // SoftEther 会在 SNI 扩展中包含特殊标识
            return check_softether_sni(data);
        }
    }
    
    return false;
}

inline bool check_softether_sni(const std::vector<std::uint8_t>& tls_data) {
    // 简化的 SNI 检查，实际实现需要完整的 TLS 解析
    // SoftEther 常用的 SNI 标识
    std::vector<std::string> softether_sni_patterns = {
        "vpngate-client-hostname",
        "softether-vpn",
        "packetix-vpn"
    };
    
    std::string data_str(tls_data.begin(), tls_data.end());
    for (const auto& pattern : softether_sni_patterns) {
        if (data_str.find(pattern) != std::string::npos) {
            return true;
        }
    }
    
    return false;
}