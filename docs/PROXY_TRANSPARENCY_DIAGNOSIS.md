# 代理桥透明度诊断指南

## 🎯 问题分析

根据您的描述：
- ✅ **Chromium直连** → 通过AdScore检测
- ✅ **Chromium + OpenVPN系统代理** → 通过检测  
- ❌ **Chromium + 您的代理桥** → 被检测

说明**代理桥本身改变了流量特征**，导致被识别为代理流量。

## 🔍 可能的检测点

### 1. **SOCKS5握手特征**
AdScore可能检测SOCKS5协议的握手过程：

```
客户端 → 代理桥: SOCKS5_AUTH_REQUEST
代理桥 → 客户端: SOCKS5_AUTH_RESPONSE  
客户端 → 代理桥: SOCKS5_CONNECT_REQUEST
代理桥 → 客户端: SOCKS5_CONNECT_RESPONSE
```

**解决方案**: 使用HTTP CONNECT代理替代SOCKS5

### 2. **TCP连接时序差异**
代理桥引入额外的网络跳数：

```
直连:    Chromium -----(1跳)-----> 目标服务器
代理桥:  Chromium → 代理桥 → 目标服务器 (2跳)
```

额外的延迟和时序变化可能被检测。

### 3. **socket选项差异**
代理桥可能设置了不同的TCP选项：

```bash
# 检查socket选项差异
netstat -ano | findstr :1080    # Windows
ss -tuln | grep :1080          # Linux
```

### 4. **缓冲区大小和IO模式**
- 直连: 系统默认缓冲区
- 代理桥: 可能使用不同的缓冲区大小

## 🛠️ 诊断步骤

### 步骤1: 抓包分析

```bash
# 使用Wireshark抓包对比
# 1. 直连访问adscore.com的流量
# 2. 通过代理桥访问的流量
# 对比TCP握手、TLS握手、HTTP请求的差异
```

### 步骤2: 检查TCP指纹

```python
# 使用p0f或类似工具检查TCP指纹
# 对比直连和代理的TCP窗口大小、选项等
```

### 步骤3: 测试不同代理协议

```bash
# 测试HTTP CONNECT代理
curl -x http://127.0.0.1:8080 https://adscore.com/

# 测试SOCKS5代理  
curl -x socks5://127.0.0.1:1080 https://adscore.com/

# 对比检测结果
```

## 🔧 解决方案

### 方案1: 切换到HTTP CONNECT代理

修改代理桥，默认使用HTTP CONNECT而非SOCKS5：

```cpp
// 在proxy_server_option中添加
options.force_http_connect_ = true;
options.disable_socks_ = true;  // 禁用SOCKS5
```

### 方案2: 原生socket透明代理

绕过所有proxy_stream包装，直接使用raw socket：

```cpp
// 使用原生socket进行透明转发
tcp::socket raw_client_socket;
tcp::socket raw_server_socket;

// 直接socket-to-socket转发
co_await transfer_raw_sockets(raw_client_socket, raw_server_socket);
```

### 方案3: 内核级透明代理 (Linux)

使用iptables + REDIRECT实现完全透明：

```bash
# 设置iptables规则
iptables -t nat -A OUTPUT -p tcp --dport 80,443 -j REDIRECT --to-port 1080

# 程序使用SO_ORIGINAL_DST获取原始目标
```

### 方案4: TUN/TAP设备代理

创建虚拟网络设备，完全模拟网络栈：

```cpp
// 创建TUN设备
int tun_fd = open("/dev/net/tun", O_RDWR);
// 处理IP数据包，完全透明转发
```

## 🚀 快速测试方案

### 测试1: 最小化代理桥

创建一个最简单的TCP中继：

```cpp
// minimal_proxy.cpp
#include <boost/asio.hpp>

net::awaitable<void> relay(tcp::socket& from, tcp::socket& to) {
    char buffer[16384];  // 16KB - Chrome默认大小
    boost::system::error_code ec;
    
    while (true) {
        auto bytes = co_await from.async_read_some(
            net::buffer(buffer), net_awaitable[ec]);
        if (ec) break;
        
        co_await net::async_write(to, net::buffer(buffer, bytes), 
                                  net_awaitable[ec]);
        if (ec) break;
    }
}
```

### 测试2: HTTP隧道模式

```cpp
// 实现简单的HTTP CONNECT隧道
// 避免SOCKS5协议可能的检测
```

### 测试3: 本地端口转发

```bash
# 使用系统工具测试
ssh -L 8080:target.com:443 localhost
# 对比直连和转发的检测差异
```

## 📊 检测结果分析

### 如果HTTP CONNECT也被检测
说明问题在于**额外的网络跳数**或**时序特征**

### 如果原生socket转发也被检测  
说明问题可能在于**程序本身的网络行为**

### 如果TUN/TAP模式通过检测
说明AdScore检测的是**应用层代理协议**

## 🎯 最可能的解决方案

基于您的情况，我推荐按以下顺序尝试：

1. **禁用SOCKS5，只使用HTTP CONNECT**
2. **使用原生socket bypass所有包装层**  
3. **如果还不行，配置上游代理**

```cpp
// 快速修复配置
options.disable_socks_ = true;           // 禁用SOCKS5
options.force_http_connect_ = true;      // 强制HTTP CONNECT
options.proxy_pass_ = "socks5://127.0.0.1:1081";  // 使用OpenVPN作为上游
```

这样既保持了您的OpenVPN方案，又绕过了代理桥的检测问题。

## 🔍 后续优化

如果上述方案解决了问题，可以进一步优化：

1. **智能路由**: 只对检测域名使用上游代理
2. **协议伪装**: 将代理流量伪装成正常HTTP流量
3. **时序优化**: 添加随机延迟模拟真实网络

---

**关键诊断问题**:
- 您使用的是SOCKS5还是HTTP代理协议？
- 是否可以尝试切换协议类型？
- 是否愿意配置OpenVPN作为上游代理？