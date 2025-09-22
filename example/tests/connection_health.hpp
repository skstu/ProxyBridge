#ifndef CONNECTION_HEALTH_HPP
#define CONNECTION_HEALTH_HPP

#include <string>
#include <unordered_map>
#include <chrono>
#include <memory>
#include <mutex>

namespace proxy {
namespace connection_health {

// 连接健康状态
struct HostHealth {
    std::string host;
    uint16_t port;
    int consecutive_failures = 0;
    int consecutive_successes = 0;
    std::chrono::steady_clock::time_point last_failure;
    std::chrono::steady_clock::time_point last_success;
    bool is_healthy = true;
    
    // 计算当前健康分数 (0-100)
    int get_health_score() const {
        auto now = std::chrono::steady_clock::now();
        
        // 如果最近30秒内有连续3次失败，降低健康分数
        if (consecutive_failures >= 3) {
            auto time_since_failure = now - last_failure;
            if (time_since_failure < std::chrono::seconds(30)) {
                return std::max(0, 50 - consecutive_failures * 10);
            }
        }
        
        // 如果最近有成功连接，提高健康分数
        if (consecutive_successes > 0) {
            auto time_since_success = now - last_success;
            if (time_since_success < std::chrono::seconds(60)) {
                return std::min(100, 70 + consecutive_successes * 5);
            }
        }
        
        return 60; // 默认健康分数
    }
    
    // 是否应该使用备用策略
    bool should_use_fallback() const {
        return consecutive_failures >= 2 && get_health_score() < 40;
    }
};

// 连接健康管理器
class HealthManager {
private:
    std::unordered_map<std::string, HostHealth> host_health_;
    mutable std::mutex mutex_;
    
    std::string make_key(const std::string& host, uint16_t port) const {
        return host + ":" + std::to_string(port);
    }
    
public:
    // 记录连接成功
    void record_success(const std::string& host, uint16_t port) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto key = make_key(host, port);
        auto& health = host_health_[key];
        
        health.host = host;
        health.port = port;
        health.consecutive_successes++;
        health.consecutive_failures = 0;
        health.last_success = std::chrono::steady_clock::now();
        health.is_healthy = true;
    }
    
    // 记录连接失败
    void record_failure(const std::string& host, uint16_t port) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto key = make_key(host, port);
        auto& health = host_health_[key];
        
        health.host = host;
        health.port = port;
        health.consecutive_failures++;
        health.consecutive_successes = 0;
        health.last_failure = std::chrono::steady_clock::now();
        
        // 连续失败3次标记为不健康
        if (health.consecutive_failures >= 3) {
            health.is_healthy = false;
        }
    }
    
    // 获取主机健康状态
    HostHealth get_health(const std::string& host, uint16_t port) const {
        std::lock_guard<std::mutex> lock(mutex_);
        auto key = make_key(host, port);
        auto it = host_health_.find(key);
        
        if (it != host_health_.end()) {
            return it->second;
        }
        
        // 返回默认健康状态
        HostHealth default_health;
        default_health.host = host;
        default_health.port = port;
        return default_health;
    }
    
    // 获取推荐的连接策略
    struct ConnectionStrategy {
        std::chrono::milliseconds timeout;
        int max_retries;
        std::chrono::milliseconds retry_delay;
        bool use_keepalive;
        bool enable_fast_reconnect;
    };
    
    ConnectionStrategy get_strategy(const std::string& host, uint16_t port) const {
        auto health = get_health(host, port);
        ConnectionStrategy strategy;
        
        if (health.should_use_fallback()) {
            // 不健康的主机使用保守策略
            strategy.timeout = std::chrono::seconds(60);
            strategy.max_retries = 5;
            strategy.retry_delay = std::chrono::milliseconds(2000);
            strategy.use_keepalive = false;
            strategy.enable_fast_reconnect = false;
        } else if (health.get_health_score() > 80) {
            // 健康的主机使用激进策略
            strategy.timeout = std::chrono::seconds(30);
            strategy.max_retries = 2;
            strategy.retry_delay = std::chrono::milliseconds(500);
            strategy.use_keepalive = true;
            strategy.enable_fast_reconnect = true;
        } else {
            // 默认策略
            strategy.timeout = std::chrono::seconds(45);
            strategy.max_retries = 3;
            strategy.retry_delay = std::chrono::milliseconds(1000);
            strategy.use_keepalive = true;
            strategy.enable_fast_reconnect = false;
        }
        
        return strategy;
    }
    
    // 清理过期的健康记录
    void cleanup_expired() {
        std::lock_guard<std::mutex> lock(mutex_);
        auto now = std::chrono::steady_clock::now();
        
        for (auto it = host_health_.begin(); it != host_health_.end();) {
            auto& health = it->second;
            
            // 清理5分钟前的记录
            bool expired = false;
            if (health.last_failure != std::chrono::steady_clock::time_point{}) {
                expired = (now - health.last_failure) > std::chrono::minutes(5);
            }
            if (health.last_success != std::chrono::steady_clock::time_point{}) {
                expired = expired && (now - health.last_success) > std::chrono::minutes(5);
            }
            
            if (expired) {
                it = host_health_.erase(it);
            } else {
                ++it;
            }
        }
    }
};

// 全局健康管理器实例
inline HealthManager& get_global_health_manager() {
    static HealthManager instance;
    return instance;
}

} // namespace connection_health
} // namespace proxy

#endif // CONNECTION_HEALTH_HPP