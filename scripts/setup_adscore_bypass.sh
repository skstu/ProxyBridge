#!/bin/bash
# setup_adscore_bypass.sh
# AdScore绕过快速配置脚本

echo "=== AdScore绕过代理配置助手 ==="
echo

# 检查是否有OpenVPN运行
check_openvpn() {
    if pgrep -x "openvpn" > /dev/null; then
        echo "✅ 检测到OpenVPN正在运行"
        VPN_RUNNING=true
    else
        echo "❌ OpenVPN未运行"
        VPN_RUNNING=false
    fi
}

# 检查SOCKS5代理端口
check_socks_proxy() {
    if ss -tuln | grep ":1081" > /dev/null 2>&1; then
        echo "✅ 检测到本地SOCKS5代理 (端口1081)"
        LOCAL_SOCKS=true
    else
        echo "❌ 未检测到本地SOCKS5代理"
        LOCAL_SOCKS=false
    fi
}

# 生成代理配置
generate_config() {
    cat > proxy_config_generated.json << EOF
{
    "proxy_server_config": {
        "network": {
            "listen_address": "127.0.0.1",
            "listen_port": 1080
        },
        
        "adscore_bypass": {
            "enabled": true,
            "strategy": "$STRATEGY",
            "upstream_proxy": "$UPSTREAM_PROXY",
            "smart_routing": true
        },
        
        "detection_domains": [
            "adscore.com",
            "doubleclick.net", 
            "googleadservices.com",
            "google-analytics.com"
        ]
    }
}
EOF
    echo "✅ 配置文件已生成: proxy_config_generated.json"
}

# 主配置流程
main() {
    echo "正在检测系统配置..."
    check_openvpn
    check_socks_proxy
    echo
    
    echo "⚠️  检测到您的情况:"
    echo "- Chromium直连: ✅ 通过AdScore检测"
    echo "- Chromium + OpenVPN: ✅ 通过检测"  
    echo "- Chromium + 代理桥: ❌ 被检测"
    echo
    echo "说明代理桥本身被检测，需要优化透明度"
    echo
    
    echo "解决方案选择:"
    echo "1) 代理桥 + OpenVPN上游 (推荐，解决IP检测)"
    echo "2) 只使用HTTP CONNECT协议 (避免SOCKS5检测)"
    echo "3) 完全透明模式 (移除所有检测特征)"
    echo "4) 购买住宅代理服务 (最佳效果)"
    echo "5) 诊断模式 (分析被检测原因)"
    echo
    
    read -p "请输入选择 (1-5): " choice
    
    case $choice in
        1)
            STRATEGY="proxy_bridge_with_openvpn"
            if [ "$VPN_RUNNING" = true ]; then
                UPSTREAM_PROXY="socks5://127.0.0.1:1081"
                echo "✅ 已配置代理桥使用OpenVPN作为上游代理"
                echo "这样既解决了IP检测问题，又绕过了代理桥检测"
            else
                echo "❌ 需要先启动OpenVPN连接"
                echo "启动OpenVPN后，代理桥将自动使用VPN出口IP"
                UPSTREAM_PROXY="socks5://127.0.0.1:1081"
            fi
            ;;
            
        2)
            STRATEGY="http_connect_only"
            UPSTREAM_PROXY=""
            echo "✅ 已配置为HTTP CONNECT协议"
            echo "避免SOCKS5握手可能的检测特征"
            echo "注意: 需要客户端支持HTTP代理协议"
            ;;
            
        3)
            STRATEGY="full_transparent"
            UPSTREAM_PROXY=""
            echo "✅ 已配置完全透明模式"
            echo "移除所有可能的检测特征，最大化透明度"
            echo "如果仍被检测，建议使用上游代理"
            ;;
            
        4)
            STRATEGY="residential_proxy"
            echo
            echo "住宅代理推荐 (针对AdScore检测优化):"
            echo "- Smartproxy: 性价比高，AdScore通过率95%+"
            echo "- IPRoyal: 价格实惠，稳定性好"
            echo "- Bright Data: 最高质量，企业级"
            echo
            read -p "请输入住宅代理地址: " proxy_url
            UPSTREAM_PROXY="$proxy_url"
            ;;
            
        5)
            echo "🔍 诊断模式启动"
            echo
            echo "请按以下步骤进行诊断:"
            echo "1. 使用Wireshark抓包对比直连和代理的流量差异"
            echo "2. 检查是否SOCKS5握手被检测"
            echo "3. 对比TCP连接选项和时序"
            echo "4. 测试不同的代理协议"
            echo
            echo "详细诊断指南: docs/PROXY_TRANSPARENCY_DIAGNOSIS.md"
            
            STRATEGY="diagnostic"
            UPSTREAM_PROXY=""
            ;;
            
        *)
            echo "无效选择"
            exit 1
            ;;
    esac
    
    generate_config
    echo
    echo "=== 配置完成 ==="
    echo
    
    case $choice in
        1)
            echo "配置说明: 代理桥 + OpenVPN上游"
            echo "- 流量路径: Chromium → 代理桥 → OpenVPN → 目标服务器"
            echo "- 优点: 解决IP检测 + 绕过代理桥检测"
            echo "- 需要: 确保OpenVPN在端口1081提供SOCKS5代理"
            ;;
        2)
            echo "配置说明: HTTP CONNECT协议"
            echo "- 浏览器配置: HTTP代理 127.0.0.1:1080"
            echo "- 避免: SOCKS5协议可能的检测特征"
            echo "- 测试: curl -x http://127.0.0.1:1080 https://adscore.com/"
            ;;
        3)
            echo "配置说明: 完全透明模式"
            echo "- 特点: 移除所有检测和修改功能"
            echo "- 风险: 如果仍被检测，需要其他方案"
            echo "- 适用: 确认代理桥透明度的基准测试"
            ;;
    esac
    
    echo
    echo "下一步操作:"
    echo "1. 编译代理服务器: make clean && make"
    echo "2. 启动服务器: ./proxy_server --config proxy_config_generated.json"
    echo "3. 配置浏览器代理: 127.0.0.1:1080"
    echo "4. 测试AdScore: https://adscore.com/demo/"
    echo
    echo "🎯 期望结果: 与OpenVPN直连相同的检测结果"
}

# Windows版本 (PowerShell)
generate_windows_script() {
    cat > setup_adscore_bypass.ps1 << 'EOF'
# setup_adscore_bypass.ps1
# Windows AdScore绕过配置脚本

Write-Host "=== AdScore绕过代理配置助手 (Windows) ===" -ForegroundColor Green
Write-Host ""

# 检查OpenVPN进程
$vpnProcess = Get-Process -Name "openvpn*" -ErrorAction SilentlyContinue
if ($vpnProcess) {
    Write-Host "✅ 检测到OpenVPN正在运行" -ForegroundColor Green
    $VPN_RUNNING = $true
} else {
    Write-Host "❌ OpenVPN未运行" -ForegroundColor Red
    $VPN_RUNNING = $false
}

# 检查端口1081
$portCheck = netstat -an | Select-String ":1081"
if ($portCheck) {
    Write-Host "✅ 检测到本地代理端口1081" -ForegroundColor Green
    $LOCAL_SOCKS = $true
} else {
    Write-Host "❌ 未检测到本地代理端口1081" -ForegroundColor Red  
    $LOCAL_SOCKS = $false
}

Write-Host ""
Write-Host "推荐解决方案："
Write-Host "1. 如果有VPN: 配置VPN输出SOCKS5代理到端口1081"
Write-Host "2. 购买住宅代理服务 (成功率最高)"
Write-Host "3. 使用代理链模式"
Write-Host ""

Write-Host "Chrome配置示例："
Write-Host 'chrome.exe --proxy-server="socks5://127.0.0.1:1080"' -ForegroundColor Yellow
Write-Host ""

Write-Host "测试网址："
Write-Host "https://adscore.com/demo/" -ForegroundColor Cyan
Write-Host "https://ipleak.net/" -ForegroundColor Cyan
EOF
    
    echo "✅ Windows脚本已生成: setup_adscore_bypass.ps1"
}

# 检查操作系统
if [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "win32" ]]; then
    echo "检测到Windows系统，生成PowerShell脚本..."
    generate_windows_script
else
    main
fi