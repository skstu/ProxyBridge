@echo off
:: SOCKS5 隐身代理启动脚本
title SOCKS5 隐身代理服务器

echo ==========================================
echo    SOCKS5 隐身代理服务器 - 风控绕过版
echo ==========================================
echo.

:: 检查管理员权限
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo [警告] 建议以管理员权限运行以获得最佳性能
    echo.
)

:: 检查配置文件
if not exist "config\socks5_stealth_config.json" (
    echo [错误] 配置文件不存在: config\socks5_stealth_config.json
    pause
    exit /b 1
)

echo [信息] 正在初始化隐身系统...

:: 创建必要的目录
if not exist "certs" mkdir certs
if not exist "www" mkdir www
if not exist "logs" mkdir logs

echo [信息] 启动参数配置:

echo ================== 基础配置 ==================
echo - 协议类型: SOCKS5 (高度优化)
echo - 监听端口: 1080 (SOCKS5), 8080 (HTTP), 443 (HTTPS)
echo - 认证用户: socks5_user
echo - SSL/TLS: 支持

echo ================ 隐身隧道配置 ================
echo - 隐身隧道: 启用 (SOCKS5 特化)
echo - 协议伪装: SOCKS5 + Chrome 浏览器
echo - 流量混淆: 启用 (Scramble + 8KB噪声)
echo - 深度包检测规避: 启用
echo - 协议跳跃: 启用 (每15分钟切换)
echo - 时序抖动: 启用 (0-50ms)
echo - 多层混淆: 启用

echo ============== 高级SOCKS5特性 ===============
echo - 虚假握手: 启用
echo - 连接池: 启用 (8连接)
echo - 请求分片: 启用 (16-128字节)
echo - 响应填充: 启用 (20%% 填充率)

echo ================ 风控绕过技术 ================
echo ✓ SOCKS5 协议特征隐藏
echo ✓ 流量时序随机化
echo ✓ 数据包大小伪装
echo ✓ 协议指纹伪造
echo ✓ 行为模式模拟
echo ✓ 连接模式混淆
echo ✓ 握手序列随机化
echo ✓ 噪声数据注入

echo =============== 客户端连接信息 ===============
echo.
echo [SOCKS5 代理配置]
echo 服务器: 127.0.0.1
echo 端口: 1080
echo 类型: SOCKS5
echo 用户名: socks5_user
echo 密码: Socks5Stealth2024!
echo.
echo [HTTP/HTTPS 代理配置]
echo 服务器: 127.0.0.1
echo 端口: 8080 (HTTP) / 443 (HTTPS)
echo 用户名: socks5_user
echo 密码: Socks5Stealth2024!
echo.

echo ============== 高级使用建议 =================
echo 1. 建议使用SOCKS5端口(1080)获得最佳隐身效果
echo 2. 启用客户端的"远程DNS解析"选项
echo 3. 配置客户端使用代理链以增强安全性
echo 4. 定期切换客户端软件以避免指纹识别
echo 5. 避免在同一时间段内大量连接
echo.

echo [启动] 正在启动 SOCKS5 隐身代理服务器...
echo.

:: 启动服务
proxy.exe ^
  --config config\socks5_stealth_config.json ^
  --stealth_tunnel true ^
  --stealth_protocol socks5 ^
  --enable_traffic_analysis_evasion true ^
  --enable_deep_packet_inspection_evasion true ^
  --enable_protocol_hopping true ^
  --enable_timing_jitter true ^
  --enable_multilayer_obfuscation true ^
  --enable_traffic_fingerprint_spoofing true ^
  --target_fingerprint chrome ^
  --scramble true ^
  --noise_length 8192 ^
  --tcp_timeout 300 ^
  --udp_timeout 60 ^
  --doc_directory .\www\ ^
  --autoindex true ^
  --ssl_cert_path .\certs\

if %errorLevel% neq 0 (
    echo.
    echo [错误] SOCKS5 代理服务器启动失败！
    echo 可能原因：
    echo 1. proxy.exe 文件不存在
    echo 2. 端口被占用 (检查1080/8080/443端口)
    echo 3. 配置文件错误
    echo 4. 系统权限不足
    echo 5. 防火墙阻止
    echo.
    echo 解决方法：
    echo 1. 检查文件完整性
    echo 2. 使用 netstat -an ^| findstr ":1080" 检查端口
    echo 3. 以管理员身份运行
    echo 4. 临时关闭防火墙测试
    echo.
)

pause