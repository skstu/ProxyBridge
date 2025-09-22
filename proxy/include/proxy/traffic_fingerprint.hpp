#ifndef INCLUDE__2023_10_18__TRAFFIC_FINGERPRINT_HPP
#define INCLUDE__2023_10_18__TRAFFIC_FINGERPRINT_HPP

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <functional>
#include <cstdint>
#include <vector>
#include <string>

namespace proxy {

// 流量指纹回调类型定义
struct traffic_fingerprint_callbacks {
    // TLS ClientHello 指纹回调
    std::function<void(const std::vector<uint8_t>& client_hello_data)> on_client_hello;
    
    // TCP 窗口大小回调
    std::function<void(uint32_t window_size, bool is_client)> on_tcp_window;
    
    // TTL 值回调
    std::function<void(uint8_t ttl_value, bool is_client)> on_ttl_value;
    
    // TCP 选项回调
    std::function<void(const std::vector<uint8_t>& tcp_options, bool is_client)> on_tcp_options;
    
    // SSL/TLS 握手完成回调
    std::function<void(const std::string& cipher_suite, const std::string& protocol_version)> on_ssl_handshake_complete;
    
    // 数据包时序回调
    std::function<void(uint64_t timestamp_us, size_t packet_size, bool is_outbound)> on_packet_timing;
};

// 流量指纹管理器
class traffic_fingerprint_manager {
public:
    static traffic_fingerprint_manager& instance() {
        static traffic_fingerprint_manager inst;
        return inst;
    }
    
    void set_callbacks(const traffic_fingerprint_callbacks& callbacks) {
        callbacks_ = callbacks;
    }
    
    const traffic_fingerprint_callbacks& get_callbacks() const {
        return callbacks_;
    }
    
    // 触发 ClientHello 回调
    void trigger_client_hello(const std::vector<uint8_t>& data) {
        if (callbacks_.on_client_hello) {
            callbacks_.on_client_hello(data);
        }
    }
    
    // 触发 TCP 窗口回调
    void trigger_tcp_window(uint32_t window_size, bool is_client) {
        if (callbacks_.on_tcp_window) {
            callbacks_.on_tcp_window(window_size, is_client);
        }
    }
    
    // 触发 TTL 回调
    void trigger_ttl(uint8_t ttl, bool is_client) {
        if (callbacks_.on_ttl_value) {
            callbacks_.on_ttl_value(ttl, is_client);
        }
    }
    
    // 触发 TCP 选项回调
    void trigger_tcp_options(const std::vector<uint8_t>& options, bool is_client) {
        if (callbacks_.on_tcp_options) {
            callbacks_.on_tcp_options(options, is_client);
        }
    }
    
    // 触发 SSL 握手完成回调
    void trigger_ssl_handshake_complete(const std::string& cipher, const std::string& version) {
        if (callbacks_.on_ssl_handshake_complete) {
            callbacks_.on_ssl_handshake_complete(cipher, version);
        }
    }
    
    // 触发数据包时序回调
    void trigger_packet_timing(uint64_t timestamp_us, size_t size, bool is_outbound) {
        if (callbacks_.on_packet_timing) {
            callbacks_.on_packet_timing(timestamp_us, size, is_outbound);
        }
    }
    
private:
    traffic_fingerprint_callbacks callbacks_;
};

// 获取当前时间戳（微秒）
inline uint64_t get_timestamp_us() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

} // namespace proxy

#endif // INCLUDE__2023_10_18__TRAFFIC_FINGERPRINT_HPP