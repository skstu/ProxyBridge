@echo off
:: 3proxy 服务安装和 ProxyBridge 替代方案脚本
:: 运行此脚本需要管理员权限

echo ====================================
echo   3proxy 问题解决 和 ProxyBridge 安装
echo ====================================
echo.

:: 检查管理员权限
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo [错误] 此脚本需要管理员权限！
    echo 请右键点击此脚本，选择"以管理员身份运行"
    pause
    exit /b 1
)

echo [信息] 检测到管理员权限，继续执行...
echo.

:MENU
echo 请选择操作：
echo 1. 修复 3proxy 安装问题
echo 2. 安装 ProxyBridge（推荐）
echo 3. 比较 3proxy vs ProxyBridge
echo 4. 生成 ProxyBridge 配置文件
echo 5. 退出
echo.
set /p choice="请输入选择 (1-5): "

if "%choice%"=="1" goto FIX_3PROXY
if "%choice%"=="2" goto INSTALL_PROXYBRIDGE
if "%choice%"=="3" goto COMPARE
if "%choice%"=="4" goto GENERATE_CONFIG
if "%choice%"=="5" goto EXIT
goto MENU

:FIX_3PROXY
echo.
echo ====================================
echo           修复 3proxy 问题
echo ====================================
echo.

echo [步骤1] 停止可能存在的 3proxy 服务...
sc stop 3proxy 2>nul
sc delete 3proxy 2>nul

echo [步骤2] 检查 3proxy.cfg 配置文件...
if not exist "3proxy.cfg" (
    echo [警告] 未找到 3proxy.cfg 配置文件！
    echo 正在创建示例配置文件...
    echo # 3proxy 配置文件示例 > 3proxy.cfg
    echo # 用户认证 >> 3proxy.cfg
    echo users admin:CL:password >> 3proxy.cfg
    echo # SOCKS5 代理，端口 1080 >> 3proxy.cfg
    echo auth strong >> 3proxy.cfg
    echo socks -p1080 >> 3proxy.cfg
    echo # HTTP 代理，端口 8080 >> 3proxy.cfg
    echo proxy -p8080 >> 3proxy.cfg
    echo [完成] 已创建 3proxy.cfg 配置文件
) else (
    echo [完成] 找到 3proxy.cfg 配置文件
)

echo [步骤3] 重新安装 3proxy 服务...
3proxy.exe --install
if %errorLevel% equ 0 (
    echo [成功] 3proxy 服务安装成功！
    
    echo [步骤4] 启动 3proxy 服务...
    sc start 3proxy
    if %errorLevel% equ 0 (
        echo [成功] 3proxy 服务启动成功！
        echo.
        echo 服务信息：
        echo - SOCKS5 代理: 127.0.0.1:1080
        echo - HTTP 代理:   127.0.0.1:8080
        echo - 用户名/密码: admin/password
    ) else (
        echo [错误] 3proxy 服务启动失败
        echo 请检查配置文件和端口占用情况
    )
) else (
    echo [错误] 3proxy 服务安装失败
    echo 可能原因：
    echo 1. 3proxy.exe 文件损坏
    echo 2. 配置文件格式错误
    echo 3. 端口被占用
    echo.
    echo 建议使用 ProxyBridge 替代方案（选项2）
)

echo.
pause
goto MENU

:INSTALL_PROXYBRIDGE
echo.
echo ====================================
echo        安装 ProxyBridge 代理服务器
echo ====================================
echo.

echo ProxyBridge 优势：
echo ✓ 现代 C++ 架构，性能更好
echo ✓ 支持 SSL/TLS 加密和流量混淆
echo ✓ 智能伪装，难以被检测
echo ✓ 支持多用户认证和地区过滤
echo ✓ 自动证书管理
echo.

echo [信息] ProxyBridge 需要编译，正在生成便携版配置...

:: 创建 ProxyBridge 目录
if not exist "ProxyBridge" mkdir ProxyBridge
cd ProxyBridge

:: 生成配置文件
echo [步骤1] 生成 ProxyBridge 配置文件...
echo { > config.json
echo   "listen": [ >> config.json
echo     "0.0.0.0:1080", >> config.json
echo     "0.0.0.0:8080" >> config.json
echo   ], >> config.json
echo   "auth_users": [ >> config.json
echo     {"username": "admin", "password": "ProxyBridge2024!"}, >> config.json
echo     {"username": "user", "password": "UserPass123"} >> config.json
echo   ], >> config.json
echo   "scramble": true, >> config.json
echo   "noise_length": 4096, >> config.json
echo   "tcp_timeout": 300, >> config.json
echo   "ssl_cert_path": "./certs/", >> config.json
echo   "doc_directory": "./www/", >> config.json
echo   "disable_insecure": false >> config.json
echo } >> config.json

:: 生成启动脚本
echo [步骤2] 生成启动脚本...
echo @echo off > start_proxy.bat
echo echo 启动 ProxyBridge 代理服务器... >> start_proxy.bat
echo echo. >> start_proxy.bat
echo echo 配置信息： >> start_proxy.bat
echo echo - SOCKS5 代理: 127.0.0.1:1080 >> start_proxy.bat
echo echo - HTTP 代理:   127.0.0.1:8080 >> start_proxy.bat
echo echo - 管理员账户:  admin / ProxyBridge2024! >> start_proxy.bat
echo echo - 普通账户:    user / UserPass123 >> start_proxy.bat
echo echo - 加密混淆:    已启用 >> start_proxy.bat
echo echo. >> start_proxy.bat
echo echo 按 Ctrl+C 停止服务器 >> start_proxy.bat
echo echo. >> start_proxy.bat
echo REM proxy.exe --config config.json >> start_proxy.bat
echo echo [注意] ProxyBridge 需要先编译才能运行 >> start_proxy.bat
echo echo 请参考 README.md 中的编译说明 >> start_proxy.bat
echo pause >> start_proxy.bat

:: 创建 www 目录用于伪装
if not exist "www" mkdir www
echo ^<html^>^<head^>^<title^>Welcome^</title^>^</head^> > www\index.html
echo ^<body^>^<h1^>Welcome to Our Website^</h1^>^</body^>^</html^> >> www\index.html

:: 生成编译说明
echo [步骤3] 生成编译说明...
echo # ProxyBridge 编译说明 > README.md
echo. >> README.md
echo ## 环境要求 >> README.md
echo - Visual Studio 2022 或更高版本 >> README.md
echo - CMake 3.20+ >> README.md
echo - Boost 1.75+ >> README.md
echo - OpenSSL 1.1+ >> README.md
echo. >> README.md
echo ## 编译步骤 >> README.md
echo 1. 安装 vcpkg: >> README.md
echo    git clone https://github.com/Microsoft/vcpkg.git >> README.md
echo    cd vcpkg >> README.md
echo    .\bootstrap-vcpkg.bat >> README.md
echo. >> README.md
echo 2. 安装依赖: >> README.md
echo    .\vcpkg install boost:x64-windows openssl:x64-windows fmt:x64-windows >> README.md
echo. >> README.md
echo 3. 编译 ProxyBridge: >> README.md
echo    mkdir build ^&^& cd build >> README.md
echo    cmake .. -DCMAKE_TOOLCHAIN_FILE=path\to\vcpkg\scripts\buildsystems\vcpkg.cmake >> README.md
echo    cmake --build . --config Release >> README.md
echo. >> README.md
echo ## 使用方法 >> README.md
echo 编译完成后，将 proxy.exe 复制到此目录，然后运行 start_proxy.bat >> README.md

cd ..

echo [完成] ProxyBridge 配置已生成到 ProxyBridge 目录
echo.
echo 下一步：
echo 1. 根据 ProxyBridge\README.md 编译程序
echo 2. 将编译好的 proxy.exe 复制到 ProxyBridge 目录
echo 3. 运行 ProxyBridge\start_proxy.bat 启动服务
echo.
pause
goto MENU

:COMPARE
echo.
echo ====================================
echo        3proxy vs ProxyBridge 对比
echo ====================================
echo.

echo 功能对比表：
echo.
echo 功能项目              3proxy    ProxyBridge
echo ================================================
echo SOCKS4/5 支持          ✓         ✓
echo HTTP/HTTPS 代理        ✓         ✓ (增强)
echo 用户认证               ✓         ✓
echo SSL/TLS 加密           ✓         ✓ (更强)
echo 流量混淆               ✗         ✓ (Scramble)
echo 噪声注入               ✗         ✓
echo Web 伪装               ✗         ✓ (nginx 伪装)
echo 地区过滤               ✗         ✓ (IP库支持)
echo 多证书 SNI             ✗         ✓
echo 现代异步架构           ✗         ✓ (C++20 协程)
echo 内存使用               中         低
echo CPU 使用               中         低
echo 配置复杂度             中         简单 (JSON)
echo 维护难度               高         低
echo 社区支持               一般       活跃
echo ================================================
echo.

echo 推荐选择：
echo - 如果需要简单快速部署：3proxy
echo - 如果需要高性能和安全性：ProxyBridge
echo - 如果需要逃避检测：ProxyBridge
echo - 如果需要现代化管理：ProxyBridge
echo.
pause
goto MENU

:GENERATE_CONFIG
echo.
echo ====================================
echo       生成 ProxyBridge 高级配置
echo ====================================
echo.

set /p listen_port="SOCKS5 端口 (默认 1080): "
if "%listen_port%"=="" set listen_port=1080

set /p http_port="HTTP 端口 (默认 8080): "
if "%http_port%"=="" set http_port=8080

set /p username="管理员用户名 (默认 admin): "
if "%username%"=="" set username=admin

set /p password="管理员密码 (默认 ProxyBridge2024!): "
if "%password%"=="" set password=ProxyBridge2024!

set /p enable_scramble="启用流量混淆? (y/n, 默认 y): "
if "%enable_scramble%"=="" set enable_scramble=y

echo.
echo 正在生成配置文件...

echo { > proxybridge_config.json
echo   "server": { >> proxybridge_config.json
echo     "listen": [ >> proxybridge_config.json
echo       "0.0.0.0:%listen_port%", >> proxybridge_config.json
echo       "0.0.0.0:%http_port%" >> proxybridge_config.json
echo     ], >> proxybridge_config.json
echo     "auth_users": [ >> proxybridge_config.json
echo       {"username": "%username%", "password": "%password%"}, >> proxybridge_config.json
echo       {"username": "user", "password": "UserPass123"} >> proxybridge_config.json
echo     ], >> proxybridge_config.json

if /i "%enable_scramble%"=="y" (
    echo     "scramble": true, >> proxybridge_config.json
    echo     "noise_length": 4096, >> proxybridge_config.json
) else (
    echo     "scramble": false, >> proxybridge_config.json
    echo     "noise_length": 0, >> proxybridge_config.json
)

echo     "tcp_timeout": 300, >> proxybridge_config.json
echo     "udp_timeout": 60, >> proxybridge_config.json
echo     "ssl_cert_path": "./certs/", >> proxybridge_config.json
echo     "disable_insecure": false >> proxybridge_config.json
echo   }, >> proxybridge_config.json
echo   "security": { >> proxybridge_config.json
echo     "allow_regions": ["中国", "香港", "台湾"], >> proxybridge_config.json
echo     "deny_regions": [] >> proxybridge_config.json
echo   }, >> proxybridge_config.json
echo   "disguise": { >> proxybridge_config.json
echo     "doc_directory": "./www/", >> proxybridge_config.json
echo     "autoindex": true, >> proxybridge_config.json
echo     "version_string": "nginx/1.20.2" >> proxybridge_config.json
echo   } >> proxybridge_config.json
echo } >> proxybridge_config.json

echo [完成] 配置文件已生成：proxybridge_config.json
echo.
echo 配置摘要：
echo - SOCKS5 端口: %listen_port%
echo - HTTP 端口: %http_port%
echo - 管理员账户: %username% / %password%
echo - 流量混淆: %enable_scramble%
echo.
pause
goto MENU

:EXIT
echo.
echo 感谢使用！如需技术支持，请参考：
echo - ProxyBridge GitHub: https://github.com/your-repo/ProxyBridge
echo - 3proxy 官网: https://3proxy.org/
echo.
pause
exit /b 0