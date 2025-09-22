# SOCKS5 隐身代理 - 客户端配置指南

## 概述

本指南详细说明如何配置各种客户端软件以使用 SOCKS5 隐身代理，实现最佳的风控绕过效果。

## 基础连接信息

- **服务器地址**: 你的服务器IP或域名
- **端口**: 1080 (SOCKS5)
- **协议**: SOCKS5
- **用户名**: socks5_user
- **密码**: Socks5Stealth2024!
- **DNS**: 远程解析 (重要!)

## 客户端配置

### 1. Chrome + SwitchyOmega

```json
{
  "proxy": {
    "scheme": "socks5",
    "host": "your-server-ip",
    "port": 1080,
    "username": "socks5_user", 
    "password": "Socks5Stealth2024!"
  },
  "options": {
    "proxyDNS": true,
    "disableCache": true
  }
}
```

**配置步骤**:
1. 安装 SwitchyOmega 扩展
2. 新建情景模式 → 代理服务器
3. 代理协议: SOCKS5
4. 代理服务器: your-server-ip
5. 代理端口: 1080
6. 启用 "代理DNS查询"

### 2. Firefox

**配置路径**: 设置 → 常规 → 网络设置

```
手动代理配置:
SOCKS 主机: your-server-ip
端口: 1080
SOCKS v5: ✓
通过SOCKS代理DNS: ✓ (重要!)
```

### 3. Telegram

**桌面版配置**:
```
设置 → 高级 → 连接类型 → 使用自定义代理
类型: SOCKS5
服务器: your-server-ip
端口: 1080
用户名: socks5_user
密码: Socks5Stealth2024!
```

### 4. V2rayN 配置

```json
{
  "protocol": "socks",
  "settings": {
    "servers": [
      {
        "address": "your-server-ip",
        "port": 1080,
        "users": [
          {
            "user": "socks5_user",
            "pass": "Socks5Stealth2024!"
          }
        ]
      }
    ]
  },
  "streamSettings": {
    "sockopt": {
      "tproxy": "off",
      "mark": 0
    }
  }
}
```

### 5. Clash 配置

```yaml
proxies:
  - name: "SOCKS5-隐身"
    type: socks5
    server: your-server-ip
    port: 1080
    username: socks5_user
    password: Socks5Stealth2024!
    udp: true
    
proxy-groups:
  - name: "隐身代理"
    type: select
    proxies:
      - "SOCKS5-隐身"
      - DIRECT

rules:
  - MATCH,隐身代理
```

### 6. Proxifier 配置

```
代理服务器设置:
地址: your-server-ip
端口: 1080
协议: SOCKS版本5
认证: 是
用户名: socks5_user
密码: Socks5Stealth2024!

代理规则:
应用程序: 所有应用
目标: 任何地址
操作: 通过SOCKS5-隐身代理
```

### 7. Linux 系统级代理

**方法1: 使用 proxychains**
```bash
# 编辑配置文件
sudo nano /etc/proxychains.conf

# 添加代理配置
[ProxyList]
socks5 your-server-ip 1080 socks5_user Socks5Stealth2024!

# 使用方式
proxychains curl https://ipinfo.io
```

**方法2: 系统环境变量**
```bash
export http_proxy=socks5://socks5_user:Socks5Stealth2024!@your-server-ip:1080
export https_proxy=socks5://socks5_user:Socks5Stealth2024!@your-server-ip:1080
export all_proxy=socks5://socks5_user:Socks5Stealth2024!@your-server-ip:1080
```

### 8. Android 配置

**使用 Shadowsocks 客户端**:
```
服务器: your-server-ip
远程端口: 1080
密码: (留空)
加密: plain
插件: 无
插件选项: 无

高级设置:
协议: socks5
混淆: plain
混淆参数: (留空)
```

**使用 ProxyDroid**:
```
代理类型: SOCKS5
主机: your-server-ip
端口: 1080
用户名: socks5_user
密码: Socks5Stealth2024!
```

### 9. iOS 配置

**使用 Shadowrocket**:
```
类型: SOCKS5
服务器: your-server-ip
端口: 1080
用户名: socks5_user
密码: Socks5Stealth2024!
```

## 高级配置技巧

### 1. 最佳隐身实践

```bash
# 1. 启用远程DNS解析
proxy_dns=true

# 2. 禁用IPv6 (避免泄露)
disable_ipv6=true

# 3. 使用随机User-Agent
random_user_agent=true

# 4. 启用请求头混淆
header_obfuscation=true

# 5. 分散连接时间
connection_interval=5-30s
```

### 2. 代理链配置

```
Client → Local SOCKS5 → Your Server → Target
              ↓
         隐身处理层
```

### 3. 流量分流规则

```yaml
# 国内直连，国外代理
rules:
  - DOMAIN-SUFFIX,cn,DIRECT
  - GEOIP,CN,DIRECT
  - DOMAIN-KEYWORD,google,SOCKS5-隐身
  - DOMAIN-KEYWORD,youtube,SOCKS5-隐身
  - DOMAIN-KEYWORD,facebook,SOCKS5-隐身
  - MATCH,SOCKS5-隐身
```

## 安全建议

### 1. 连接安全
- ✅ 始终启用用户名密码认证
- ✅ 使用强密码
- ✅ 定期更换密码
- ✅ 启用远程DNS解析
- ✅ 禁用WebRTC (防止IP泄露)

### 2. 使用习惯
- 🔄 定期切换客户端软件
- 🔄 避免长时间连续使用
- 🔄 使用不同时间段连接
- 🔄 配合其他隐私工具

### 3. 检测方法

**IP检测**:
```bash
curl -x socks5://socks5_user:Socks5Stealth2024!@your-server-ip:1080 https://ipinfo.io
```

**DNS检测**:
```bash
dig @8.8.8.8 myip.opendns.com
```

**WebRTC检测**:
访问 https://ipleak.net 检查是否有IP泄露

## 故障排除

### 1. 连接失败
```
检查项目:
□ 服务器IP和端口是否正确
□ 用户名密码是否正确
□ 服务器防火墙设置
□ 客户端防火墙设置
□ 网络连通性
```

### 2. 速度慢
```
优化方法:
□ 选择更近的服务器
□ 调整隐身参数
□ 使用有线网络
□ 关闭不必要的加密
```

### 3. 被检测
```
应对措施:
□ 切换伪装协议
□ 调整时序抖动
□ 更换客户端软件
□ 分散使用时间
□ 检查配置泄露
```

## 性能调优

### 1. 客户端优化
```
连接池大小: 8-16
超时设置: 30s
重试次数: 3
缓冲区大小: 64KB
```

### 2. 服务端参数
```
tcp_timeout: 300
udp_timeout: 60
noise_length: 8192
jitter_range_ms: 50
```

记住：隐身效果 = 正确配置 + 合理使用 + 定期调整