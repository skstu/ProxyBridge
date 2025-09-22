#ifndef SOCKS5_OPTIMIZATION_HPP
#define SOCKS5_OPTIMIZATION_HPP

#include <random>
#include <chrono>
#include <vector>
#include <array>

namespace proxy {
namespace socks5_optimization {

// SOCKS5连接时序优化
class TimingOptimizer {
private:
    std::mt19937 rng;
    std::uniform_int_distribution<int> delay_dist;
    std::uniform_int_distribution<int> micro_delay_dist;
    
public:
    TimingOptimizer() 
        : rng(std::random_device{}())
        , delay_dist(5, 50) // 降低延迟范围：5-50毫秒，避免过长延迟导致超时
        , micro_delay_dist(1, 10) // 微延迟：1-10毫秒
    {}
    
    // 获取握手前的随机延迟，模拟真实客户端行为
    std::chrono::milliseconds get_handshake_delay() {
        return std::chrono::milliseconds(delay_dist(rng));
    }
    
    // 获取认证后的随机延迟（更短，避免影响性能）
    std::chrono::milliseconds get_auth_delay() {
        return std::chrono::milliseconds(micro_delay_dist(rng));
    }
    
    // 获取连接建立后的微延迟
    std::chrono::milliseconds get_connect_delay() {
        return std::chrono::milliseconds(micro_delay_dist(rng));
    }
};

// SOCKS5数据包大小优化
class PacketSizeOptimizer {
private:
    std::mt19937 rng;
    std::uniform_int_distribution<size_t> chunk_dist;
    
public:
    PacketSizeOptimizer() 
        : rng(std::random_device{}())
        , chunk_dist(1024, 8192) // 1KB-8KB随机块大小
    {}
    
    // 获取传输时的随机块大小，避免固定模式
    size_t get_chunk_size() {
        return chunk_dist(rng);
    }
    
    // 检查是否需要分片传输（模拟真实应用）
    bool should_fragment(size_t data_size) {
        if (data_size > 4096) {
            std::uniform_int_distribution<int> fragment_chance(0, 9);
            return fragment_chance(rng) < 3; // 30%概率分片
        }
        return false;
    }
};

// TCP选项优化器
class TcpOptionsOptimizer {
private:
    std::mt19937 rng;
    
public:
    TcpOptionsOptimizer() : rng(std::random_device{}()) {}
    
    // 获取TCP窗口大小（模拟不同操作系统）
    struct TcpWindowConfig {
        int receive_buffer_size;
        int send_buffer_size;
        bool tcp_nodelay;
        bool keep_alive;
    };
    
    TcpWindowConfig get_tcp_config(size_t connection_id) {
        // 根据连接ID选择不同的TCP配置，模拟不同客户端
        std::array<TcpWindowConfig, 4> configs = {{
            {65536, 65536, true, true},    // Chrome风格
            {32768, 32768, false, true},   // Firefox风格  
            {87380, 16384, true, false},   // Linux默认
            {65536, 8192, true, true}      // Windows默认
        }};
        
        return configs[connection_id % configs.size()];
    }
    
    // 是否启用TCP_NODELAY（避免固定模式）
    bool should_enable_nodelay(size_t connection_id) {
        std::uniform_int_distribution<int> nodelay_chance(0, 9);
        // 70%概率启用nodelay，模拟真实应用分布
        return nodelay_chance(rng) < 7;
    }
};

// 连接行为模拟器
class ConnectionBehaviorSimulator {
private:
    std::mt19937 rng;
    
public:
    ConnectionBehaviorSimulator() : rng(std::random_device{}()) {}
    
    // 模拟真实客户端的连接重试行为
    struct RetryConfig {
        int max_retries;
        std::chrono::milliseconds base_delay;
        double backoff_multiplier;
    };
    
    RetryConfig get_retry_config() {
        std::uniform_int_distribution<int> retry_dist(2, 4); // 增加重试次数
        std::uniform_int_distribution<int> delay_dist(500, 2000); // 增加重试间隔
        
        return {
            retry_dist(rng),
            std::chrono::milliseconds(delay_dist(rng)),
            1.5 + (rng() % 100) / 100.0  // 1.5-2.5倍退避
        };
    }
    
    // 模拟真实客户端的Keep-Alive行为
    bool should_use_keepalive() {
        std::uniform_int_distribution<int> keepalive_chance(0, 9);
        return keepalive_chance(rng) < 8; // 80%概率启用keep-alive
    }
    
    // 模拟真实的连接超时设置 - 针对Google服务优化
    std::chrono::seconds get_connection_timeout() {
        std::uniform_int_distribution<int> timeout_dist(30, 90); // 增加超时时间
        return std::chrono::seconds(timeout_dist(rng));
    }
    
    // 获取DNS解析超时时间
    std::chrono::seconds get_dns_timeout() {
        std::uniform_int_distribution<int> dns_timeout_dist(10, 30);
        return std::chrono::seconds(dns_timeout_dist(rng));
    }
    
    // 检查是否应该使用快速重连（针对Google等大型服务）
    bool should_fast_reconnect(const std::string& domain) {
        // 对Google服务启用快速重连
        if (domain.find("google.com") != std::string::npos ||
            domain.find("googleapis.com") != std::string::npos ||
            domain.find("googleusercontent.com") != std::string::npos) {
            return true;
        }
        return false;
    }
};

// SOCKS5握手优化器
class HandshakeOptimizer {
private:
    std::mt19937 rng;
    
public:
    HandshakeOptimizer() : rng(std::random_device{}()) {}
    
    // 优化认证方法选择顺序（模拟不同客户端）
    std::vector<uint8_t> get_auth_methods() {
        std::vector<std::vector<uint8_t>> method_patterns = {
            {0x00, 0x02},           // 无认证+用户名密码
            {0x02, 0x00},           // 用户名密码+无认证  
            {0x00},                 // 仅无认证
            {0x02}                  // 仅用户名密码
        };
        
        std::uniform_int_distribution<size_t> pattern_dist(0, method_patterns.size() - 1);
        return method_patterns[pattern_dist(rng)];
    }
    
    // 模拟真实客户端的域名解析偏好
    enum class AddressType {
        PREFER_IPV4,
        PREFER_IPV6, 
        PREFER_DOMAIN,
        BALANCED
    };
    
    AddressType get_address_preference() {
        std::uniform_int_distribution<int> pref_dist(0, 3);
        return static_cast<AddressType>(pref_dist(rng));
    }
};

} // namespace socks5_optimization
} // namespace proxy

#endif // SOCKS5_OPTIMIZATION_HPP