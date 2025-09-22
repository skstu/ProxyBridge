@echo off
:: ProxyBridge 隐身斗篷启动脚本 (Windows)
title ProxyBridge 隐身代理服务器

echo ======================================
echo    ProxyBridge 隐身斗篷代理服务器
echo ======================================
echo.

:: 检查管理员权限
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo [警告] 建议以管理员权限运行以获得最佳性能
    echo.
)

:: 检查配置文件
if not exist "config\stealth_config.json" (
    echo [错误] 配置文件不存在: config\stealth_config.json
    pause
    exit /b 1
)

:: 创建必要的目录
if not exist "certs" mkdir certs
if not exist "www" mkdir www
if not exist "logs" mkdir logs

:: 生成示例网站内容用于伪装
if not exist "www\index.html" (
    echo ^<!DOCTYPE html^> > www\index.html
    echo ^<html lang="zh-CN"^> >> www\index.html
    echo ^<head^> >> www\index.html
    echo     ^<meta charset="UTF-8"^> >> www\index.html
    echo     ^<meta name="viewport" content="width=device-width, initial-scale=1.0"^> >> www\index.html
    echo     ^<title^>企业级网络解决方案^</title^> >> www\index.html
    echo     ^<style^> >> www\index.html
    echo         body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background: #f5f5f5; } >> www\index.html
    echo         .container { max-width: 1200px; margin: 0 auto; background: white; padding: 40px; border-radius: 8px; box-shadow: 0 2px 10px rgba(0,0,0,0.1^); } >> www\index.html
    echo         h1 { color: #2c3e50; text-align: center; margin-bottom: 30px; } >> www\index.html
    echo         .services { display: grid; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr^)^); gap: 20px; margin: 30px 0; } >> www\index.html
    echo         .service { padding: 20px; border: 1px solid #ddd; border-radius: 5px; } >> www\index.html
    echo         .footer { text-align: center; margin-top: 40px; color: #666; } >> www\index.html
    echo     ^</style^> >> www\index.html
    echo ^</head^> >> www\index.html
    echo ^<body^> >> www\index.html
    echo     ^<div class="container"^> >> www\index.html
    echo         ^<h1^>企业级网络解决方案^</h1^> >> www\index.html
    echo         ^<p^>我们提供专业的网络基础设施服务，为企业客户提供稳定、安全的网络连接解决方案。^</p^> >> www\index.html
    echo         ^<div class="services"^> >> www\index.html
    echo             ^<div class="service"^> >> www\index.html
    echo                 ^<h3^>网络架构设计^</h3^> >> www\index.html
    echo                 ^<p^>专业的网络拓扑设计和优化服务，确保企业网络的高效运行。^</p^> >> www\index.html
    echo             ^</div^> >> www\index.html
    echo             ^<div class="service"^> >> www\index.html
    echo                 ^<h3^>安全防护^</h3^> >> www\index.html
    echo                 ^<p^>多层安全防护体系，保障企业数据和网络通信安全。^</p^> >> www\index.html
    echo             ^</div^> >> www\index.html
    echo             ^<div class="service"^> >> www\index.html
    echo                 ^<h3^>性能监控^</h3^> >> www\index.html
    echo                 ^<p^>实时网络性能监控和分析，及时发现和解决网络问题。^</p^> >> www\index.html
    echo             ^</div^> >> www\index.html
    echo         ^</div^> >> www\index.html
    echo         ^<div class="footer"^> >> www\index.html
    echo             ^<p^>^&copy; 2024 网络科技有限公司. 保留所有权利.^</p^> >> www\index.html
    echo         ^</div^> >> www\index.html
    echo     ^</div^> >> www\index.html
    echo ^</body^> >> www\index.html
    echo ^</html^> >> www\index.html
    
    echo [信息] 已创建伪装网站内容
)

echo 启动参数：
echo - 监听端口: 1080 (SOCKS5), 8080 (HTTP), 443 (HTTPS)
echo - 隐身隧道: 启用
echo - 协议伪装: HTTPS
echo - 流量混淆: 启用 (Scramble + 噪声注入)
echo - 深度包检测规避: 启用
echo - 协议跳跃: 启用 (每30分钟切换)
echo - 时序抖动: 启用 (0-100ms随机延迟)
echo - 网站伪装: 启用 (企业网络服务)
echo.

echo 隐身特性说明：
echo ✓ TLS 1.3 握手伪造 - 模拟正常HTTPS连接
echo ✓ 随机数据包填充 - 隐藏真实数据长度
echo ✓ 虚假协议字段 - 混淆协议特征
echo ✓ 时序抖动 - 模拟真实网络延迟
echo ✓ 协议动态切换 - HTTPS/WebSocket/HTTP2/gRPC
echo ✓ 流量模式模拟 - 模仿真实浏览行为
echo.

:: 显示客户端连接信息
echo 客户端连接配置：
echo.
echo [SOCKS5 代理]
echo 服务器: 127.0.0.1
echo 端口: 1080
echo 用户名: stealth_user
echo 密码: StealthPassword2024!
echo.
echo [HTTP 代理]
echo 服务器: 127.0.0.1
echo 端口: 8080
echo 用户名: stealth_user
echo 密码: StealthPassword2024!
echo.

echo 正在启动 ProxyBridge 隐身代理服务器...
echo.

:: 启动服务
proxy.exe ^
  --config config\stealth_config.json ^
  --stealth_tunnel true ^
  --stealth_protocol https ^
  --enable_traffic_analysis_evasion true ^
  --enable_deep_packet_inspection_evasion true ^
  --enable_protocol_hopping true ^
  --enable_timing_jitter true ^
  --scramble true ^
  --noise_length 4096 ^
  --doc_directory .\www\ ^
  --autoindex true ^
  --ssl_cert_path .\certs\

if %errorLevel% neq 0 (
    echo.
    echo [错误] 代理服务器启动失败！
    echo 可能原因：
    echo 1. proxy.exe 文件不存在
    echo 2. 端口被占用
    echo 3. 配置文件错误
    echo 4. 证书文件缺失
    echo.
)

pause