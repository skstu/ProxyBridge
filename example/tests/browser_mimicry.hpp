#ifndef BROWSER_MIMICRY_HPP
#define BROWSER_MIMICRY_HPP

#include <string>
#include <vector>
#include <random>
#include <unordered_map>

namespace proxy {
namespace browser_mimicry {

// 真实浏览器的User-Agent字符串
const std::vector<std::string> real_user_agents = {
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:121.0) Gecko/20100101 Firefox/121.0",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:121.0) Gecko/20100101 Firefox/121.0",
    "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36"
};

// 真实浏览器的Accept头部
const std::vector<std::string> real_accept_headers = {
    "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7",
    "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8",
    "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
};

// 真实浏览器的Accept-Language头部
const std::vector<std::string> real_accept_language = {
    "en-US,en;q=0.9",
    "zh-CN,zh;q=0.9,en;q=0.8",
    "en-GB,en;q=0.9",
    "ja-JP,ja;q=0.9,en;q=0.8"
};

// 真实浏览器的Accept-Encoding头部
const std::vector<std::string> real_accept_encoding = {
    "gzip, deflate, br",
    "gzip, deflate",
    "gzip, deflate, br, zstd"
};

// TLS特征配置
struct TLSFingerprint {
    std::vector<uint16_t> cipher_suites;
    std::vector<uint16_t> extensions;
    std::vector<uint16_t> elliptic_curves;
    std::vector<uint8_t> elliptic_curve_point_formats;
    uint16_t signature_algorithms;
};

// Chrome的TLS指纹
const TLSFingerprint chrome_tls = {
    {0x1301, 0x1302, 0x1303, 0xc02b, 0xc02f, 0xc02c, 0xc030, 0xcca9, 0xcca8, 0xc013, 0xc014, 0x009c, 0x009d, 0x002f, 0x0035},
    {0x0000, 0x0023, 0x0010, 0x0005, 0x000a, 0x000b, 0x000d, 0x0012, 0x0033, 0x002b, 0x002d, 0x0029, 0x0017, 0x001b},
    {0x001d, 0x0017, 0x0018, 0x0019},
    {0x00},
    0x0403
};

// Firefox的TLS指纹  
const TLSFingerprint firefox_tls = {
    {0x1301, 0x1302, 0x1303, 0xc02b, 0xc02f, 0xc02c, 0xc030, 0xcca9, 0xcca8, 0xc009, 0xc013, 0xc00a, 0xc014, 0x009c, 0x009d, 0x002f, 0x0035, 0x000a},
    {0x0000, 0x0017, 0x001b, 0x0023, 0x0010, 0x000a, 0x000b, 0x000d, 0x002b, 0x002d, 0x0005, 0x0033, 0x0012, 0x0029},
    {0x0017, 0x0018, 0x0019, 0x001d},
    {0x00},
    0x0403
};

class BrowserMimicryManager {
private:
    std::mt19937 rng;
    std::uniform_int_distribution<size_t> dist_ua;
    std::uniform_int_distribution<size_t> dist_accept;
    std::uniform_int_distribution<size_t> dist_lang;
    std::uniform_int_distribution<size_t> dist_encoding;

public:
    BrowserMimicryManager() 
        : rng(std::random_device{}())
        , dist_ua(0, real_user_agents.size() - 1)
        , dist_accept(0, real_accept_headers.size() - 1)
        , dist_lang(0, real_accept_language.size() - 1)
        , dist_encoding(0, real_accept_encoding.size() - 1)
    {}

    std::string get_random_user_agent() {
        return real_user_agents[dist_ua(rng)];
    }

    std::string get_random_accept() {
        return real_accept_headers[dist_accept(rng)];
    }

    std::string get_random_accept_language() {
        return real_accept_language[dist_lang(rng)];
    }

    std::string get_random_accept_encoding() {
        return real_accept_encoding[dist_encoding(rng)];
    }

    // 获取应该保留的头部字段（模拟真实浏览器行为）
    std::unordered_map<std::string, std::string> get_browser_headers() {
        std::unordered_map<std::string, std::string> headers;
        
        // 随机决定是否添加某些可选头部
        std::uniform_int_distribution<int> coin_flip(0, 1);
        
        if (coin_flip(rng)) {
            headers["Accept-Language"] = get_random_accept_language();
        }
        
        if (coin_flip(rng)) {
            headers["Accept-Encoding"] = get_random_accept_encoding();
        }
        
        // DNT头部（50%概率）
        if (coin_flip(rng)) {
            headers["DNT"] = "1";
        }
        
        // Upgrade-Insecure-Requests（80%概率）
        std::uniform_int_distribution<int> upgrade_dist(0, 4);
        if (upgrade_dist(rng) < 4) {
            headers["Upgrade-Insecure-Requests"] = "1";
        }
        
        return headers;
    }

    // 判断是否应该保留特定头部（避免移除浏览器会发送的头部）
    bool should_preserve_header(const std::string& header_name) {
        static const std::vector<std::string> browser_headers = {
            "Accept", "Accept-Language", "Accept-Encoding", "Cache-Control",
            "DNT", "Upgrade-Insecure-Requests", "Sec-Fetch-Dest", "Sec-Fetch-Mode",
            "Sec-Fetch-Site", "Sec-Fetch-User", "Cookie", "Referer"
        };
        
        for (const auto& bh : browser_headers) {
            if (header_name == bh) {
                return true;
            }
        }
        return false;
    }
};

} // namespace browser_mimicry
} // namespace proxy

#endif // BROWSER_MIMICRY_HPP