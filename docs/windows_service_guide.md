# ProxyBridge Windows 服务配置指南

## 为什么选择 ProxyBridge 而不是 3proxy？

### ProxyBridge 的优势：
1. **现代 C++20 架构**：使用协程和异步 I/O，性能更好
2. **更强的安全性**：支持 scramble 加密和噪声注入
3. **更好的伪装**：支持 HTTP/HTTPS 伪装和 SSL/TLS 加密
4. **智能路由**：支持地区过滤和用户认证
5. **易于配置**：JSON 配置文件，比 3proxy.cfg 更易懂

### 与 3proxy 功能对比：

| 功能 | 3proxy | ProxyBridge |
|------|--------|-------------|
| SOCKS4/5 | ✓ | ✓ |
| HTTP Proxy | ✓ | ✓ |
| SSL/TLS | ✓ | ✓ (更强) |
| 用户认证 | ✓ | ✓ |
| 流量加密 | ✗ | ✓ (scramble) |
| 流量伪装 | ✗ | ✓ (nginx 伪装) |
| 噪声注入 | ✗ | ✓ |
| 地区过滤 | ✗ | ✓ |
| 多证书 SNI | ✗ | ✓ |
| 现代协程 | ✗ | ✓ |

## 安装和配置 ProxyBridge

### 1. 编译 ProxyBridge
```bash
# 在 ProxyBridge 目录下
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

### 2. 创建配置文件
创建 `proxy_config.json`：
```json
{
  "server": {
    "listen": [
      "0.0.0.0:1080",
      "0.0.0.0:8080"
    ],
    "auth_users": [
      {"username": "admin", "password": "secure_password_123"},
      {"username": "user1", "password": "user_password_456"}
    ],
    "ssl_cert_path": "./certs/",
    "scramble": true,
    "noise_length": 4096,
    "tcp_timeout": 300,
    "disable_insecure": false
  },
  "security": {
    "allow_regions": ["中国", "香港", "台湾"],
    "deny_regions": ["美国", "俄罗斯"],
    "ipip_db": "./ipip.datx"
  },
  "disguise": {
    "doc_directory": "./www/",
    "autoindex": true,
    "version_string": "nginx/1.20.2"
  }
}
```

### 3. Windows 服务安装脚本
创建 `install_service.bat`：
```batch
@echo off
echo 安装 ProxyBridge Windows 服务...

sc create ProxyBridge binpath= "C:\ProxyBridge\proxy.exe --config C:\ProxyBridge\proxy_config.json" start= auto
sc description ProxyBridge "ProxyBridge 高性能代理服务器"
sc start ProxyBridge

echo 服务安装完成！
pause
```

### 4. 服务管理脚本
创建 `manage_service.bat`：
```batch
@echo off
echo ProxyBridge 服务管理
echo 1. 启动服务
echo 2. 停止服务
echo 3. 重启服务
echo 4. 查看状态
echo 5. 卸载服务
set /p choice="请选择操作 (1-5): "

if "%choice%"=="1" (
    sc start ProxyBridge
    echo 服务已启动
)
if "%choice%"=="2" (
    sc stop ProxyBridge
    echo 服务已停止
)
if "%choice%"=="3" (
    sc stop ProxyBridge
    timeout /t 3
    sc start ProxyBridge
    echo 服务已重启
)
if "%choice%"=="4" (
    sc query ProxyBridge
)
if "%choice%"=="5" (
    sc stop ProxyBridge
    sc delete ProxyBridge
    echo 服务已卸载
)

pause
```