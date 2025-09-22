#pragma once
#include <string>
#include <cstdint>

// 最佳实践：VPN + ProxyBridge + 流量伪装
struct anti_detection_config {
    // VPN配置
    bool use_residential_vpn = true;
    bool enable_ip_rotation = true;
    int rotation_interval_hours = 6;
    
    // ProxyBridge增强配置
    bool enable_scramble = true;
    std::uint16_t noise_length = 2048;
    std::string ssl_ciphers = "TLS_AES_128_GCM_SHA256:TLS_AES_256_GCM_SHA384:ECDHE-RSA-AES128-GCM-SHA256";
    
    // 流量伪装
    std::string user_agent_pool[3] = {
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36", 
        "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36"
    };
    
    // 时间随机化
    bool randomize_timing = true;
    int min_delay_ms = 100;
    int max_delay_ms = 2000;
};