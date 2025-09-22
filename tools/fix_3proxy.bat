@echo off
:: 3proxy 快速修复脚本
:: 解决 "Failed to start service: No error" 问题

title 3proxy 快速修复工具

echo.
echo ╔═══════════════════════════════════════════════════════════════╗
echo ║                    3proxy 快速修复工具                        ║
echo ║                                                               ║
echo ║  此工具将帮助您解决 3proxy 服务安装和启动问题                    ║
echo ╚═══════════════════════════════════════════════════════════════╝
echo.

:: 检查管理员权限
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo [错误] 需要管理员权限！
    echo.
    echo 解决方法：
    echo 1. 右键点击此脚本
    echo 2. 选择"以管理员身份运行"
    echo.
    pause
    exit /b 1
)

echo [✓] 检测到管理员权限
echo.

:: 步骤1：停止并删除可能存在的服务
echo [步骤1] 清理现有 3proxy 服务...
sc stop 3proxy >nul 2>&1
sc delete 3proxy >nul 2>&1
echo [✓] 已清理现有服务

:: 步骤2：检查配置文件
echo.
echo [步骤2] 检查配置文件...
if not exist "3proxy.cfg" (
    echo [!] 未找到 3proxy.cfg，正在创建默认配置...
    
    echo # 3proxy 配置文件 > 3proxy.cfg
    echo # 日志设置 >> 3proxy.cfg
    echo log 3proxy.log D >> 3proxy.cfg
    echo logformat "- %%Y-%%m-%%d %%H:%%M:%%S %%z %%N.%%p %%E %%U %%C:%%c %%R:%%r %%O %%I %%h %%T" >> 3proxy.cfg
    echo. >> 3proxy.cfg
    echo # 用户认证（用户名:加密类型:密码） >> 3proxy.cfg
    echo users admin:CL:admin123 >> 3proxy.cfg
    echo users user:CL:user123 >> 3proxy.cfg
    echo. >> 3proxy.cfg
    echo # 启用用户认证 >> 3proxy.cfg
    echo auth strong >> 3proxy.cfg
    echo. >> 3proxy.cfg
    echo # 允许所有IP访问 >> 3proxy.cfg
    echo allow * >> 3proxy.cfg
    echo. >> 3proxy.cfg
    echo # SOCKS5 代理服务，监听端口 1080 >> 3proxy.cfg
    echo socks -p1080 >> 3proxy.cfg
    echo. >> 3proxy.cfg
    echo # HTTP 代理服务，监听端口 8080 >> 3proxy.cfg
    echo proxy -p8080 >> 3proxy.cfg
    
    echo [✓] 已创建默认配置文件
) else (
    echo [✓] 找到现有配置文件
)

:: 步骤3：检查端口占用
echo.
echo [步骤3] 检查端口占用情况...
netstat -an | find ":1080" >nul
if %errorLevel% equ 0 (
    echo [!] 警告：端口 1080 已被占用
    netstat -ano | find ":1080"
)

netstat -an | find ":8080" >nul
if %errorLevel% equ 0 (
    echo [!] 警告：端口 8080 已被占用
    netstat -ano | find ":8080"
)

:: 步骤4：验证 3proxy.exe
echo.
echo [步骤4] 验证 3proxy.exe...
if not exist "3proxy.exe" (
    echo [错误] 未找到 3proxy.exe 文件！
    echo.
    echo 请确保：
    echo 1. 3proxy.exe 文件存在于当前目录
    echo 2. 文件未被杀毒软件误删
    echo 3. 下载的是正确版本的 3proxy
    echo.
    pause
    exit /b 1
)

:: 获取文件信息
for %%A in (3proxy.exe) do echo [✓] 文件大小: %%~zA 字节

:: 步骤5：测试配置文件语法
echo.
echo [步骤5] 测试配置文件语法...
3proxy.exe -t
if %errorLevel% neq 0 (
    echo [错误] 配置文件语法错误！
    echo 请检查 3proxy.cfg 文件格式
    pause
    exit /b 1
)
echo [✓] 配置文件语法正确

:: 步骤6：安装服务
echo.
echo [步骤6] 安装 3proxy 服务...
3proxy.exe --install
if %errorLevel% equ 0 (
    echo [✓] 服务安装成功
) else (
    echo [错误] 服务安装失败，错误码: %errorLevel%
    
    echo.
    echo 可能的解决方案：
    echo 1. 检查 Windows 服务控制管理器是否正常
    echo 2. 重启计算机后重试
    echo 3. 使用不同版本的 3proxy
    echo 4. 检查系统是否支持当前版本
    echo.
    pause
    exit /b 1
)

:: 步骤7：启动服务
echo.
echo [步骤7] 启动 3proxy 服务...
sc start 3proxy
if %errorLevel% equ 0 (
    echo [✓] 服务启动成功！
) else (
    echo [!] 服务启动失败，尝试手动启动...
    
    :: 尝试查看详细错误信息
    sc query 3proxy
    
    echo.
    echo 尝试直接运行测试...
    timeout /t 2 /nobreak >nul
    
    start "3proxy测试" cmd /k "3proxy.exe 3proxy.cfg && pause"
    echo [i] 已启动测试窗口，请查看是否有错误信息
)

:: 步骤8：验证服务状态
echo.
echo [步骤8] 验证服务状态...
sc query 3proxy
echo.

:: 步骤9：测试代理连接
echo [步骤9] 测试代理连接...
echo.
echo 测试命令（请在新的命令提示符中运行）：
echo.
echo SOCKS5 代理测试：
echo curl --socks5 admin:admin123@127.0.0.1:1080 http://httpbin.org/ip
echo.
echo HTTP 代理测试：
echo curl --proxy admin:admin123@127.0.0.1:8080 http://httpbin.org/ip
echo.

:: 显示配置信息
echo.
echo ╔═══════════════════════════════════════════════════════════════╗
echo ║                         配置信息                               ║
echo ╠═══════════════════════════════════════════════════════════════╣
echo ║  SOCKS5 代理:  127.0.0.1:1080                                ║
echo ║  HTTP 代理:    127.0.0.1:8080                                ║
echo ║                                                               ║
echo ║  用户账户:                                                     ║
echo ║    管理员:     admin / admin123                               ║
echo ║    普通用户:   user / user123                                 ║
echo ║                                                               ║
echo ║  日志文件:     3proxy.log                                     ║
echo ║  配置文件:     3proxy.cfg                                     ║
echo ╚═══════════════════════════════════════════════════════════════╝
echo.

echo [完成] 3proxy 安装和配置完成！
echo.
echo 服务管理命令：
echo - 启动服务: sc start 3proxy
echo - 停止服务: sc stop 3proxy
echo - 查看状态: sc query 3proxy
echo - 卸载服务: sc stop 3proxy ^&^& sc delete 3proxy
echo.

pause