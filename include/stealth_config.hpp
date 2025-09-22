#ifndef STEALTH_CONFIG_HPP
#define STEALTH_CONFIG_HPP

#include <string>
#include <vector>
#include <unordered_map>

namespace stealth {

// 隐蔽策略类型
enum class stealth_strategy {
    NATIVE_HTTP_TUNNEL,     // 原生HTTP隧道（推荐）
    TLS_SNI_MASKING,        // TLS SNI伪装
    PROTOCOL_MULTIPLEXING,  // 协议复用
    TRAFFIC_OBFUSCATION,    // 流量混淆
    ZERO_DETECTION_MODE     // 零检测模式
};

// 隐蔽配置结构
struct stealth_config {
    stealth_strategy strategy = stealth_strategy::NATIVE_HTTP_TUNNEL;
    
    // HTTP隧道配置
    std::string tunnel_host = "example.com";
    uint16_t tunnel_port = 443;
    std::string tunnel_path = "/";
    
    // TLS伪装配置
    std::string fake_sni = "cloudflare.com";
    std::vector<std::string> fake_alpn = {"h2", "http/1.1"};
    
    // 流量混淆参数
    bool enable_padding = true;
    size_t min_padding = 16;
    size_t max_padding = 256;
    
    // 时序混淆
    bool enable_timing_randomization = true;
    size_t min_delay_ms = 1;
    size_t max_delay_ms = 50;
    
    // 包大小分片
    bool enable_fragmentation = true;
    size_t fragment_size = 1024;
    
    // 伪装HTTP headers
    std::unordered_map<std::string, std::string> fake_headers = {
        {"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36"},
        {"Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"},
        {"Accept-Language", "en-US,en;q=0.5"},
        {"Accept-Encoding", "gzip, deflate, br"},
        {"Cache-Control", "no-cache"},
        {"Pragma", "no-cache"}
    };
};

// 预设配置
class stealth_presets {
public:
    // Chrome浏览器模拟配置
    static stealth_config chrome_simulation() {
        stealth_config config;
        config.strategy = stealth_strategy::NATIVE_HTTP_TUNNEL;
        config.fake_headers["User-Agent"] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36";
        config.fake_sni = "www.google.com";
        config.tunnel_host = "www.cloudflare.com";
        config.tunnel_port = 443;
        return config;
    }
    
    // Firefox浏览器模拟配置
    static stealth_config firefox_simulation() {
        stealth_config config;
        config.strategy = stealth_strategy::NATIVE_HTTP_TUNNEL;
        config.fake_headers["User-Agent"] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:121.0) Gecko/20100101 Firefox/121.0";
        config.fake_sni = "www.mozilla.org";
        config.tunnel_host = "www.amazon.com";
        config.tunnel_port = 443;
        return config;
    }
    
    // 移动端模拟配置
    static stealth_config mobile_simulation() {
        stealth_config config;
        config.strategy = stealth_strategy::NATIVE_HTTP_TUNNEL;
        config.fake_headers["User-Agent"] = "Mozilla/5.0 (iPhone; CPU iPhone OS 17_0 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.0 Mobile/15E148 Safari/604.1";
        config.fake_sni = "www.apple.com";
        config.tunnel_host = "www.microsoft.com";
        config.tunnel_port = 443;
        return config;
    }
    
    // 零检测模式（最隐蔽）
    static stealth_config zero_detection() {
        stealth_config config;
        config.strategy = stealth_strategy::ZERO_DETECTION_MODE;
        config.enable_padding = true;
        config.enable_timing_randomization = true;
        config.enable_fragmentation = true;
        config.fake_sni = "github.com";
        config.tunnel_host = "api.github.com";
        config.tunnel_port = 443;
        return config;
    }
};

} // namespace stealth

#endif // STEALTH_CONFIG_HPP