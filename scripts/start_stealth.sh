#!/bin/bash
# ProxyBridge 隐身斗篷启动脚本

echo "======================================"
echo "   ProxyBridge 隐身斗篷代理服务器"
echo "======================================"
echo

# 检查配置文件
if [ ! -f "config/stealth_config.json" ]; then
    echo "[错误] 配置文件不存在: config/stealth_config.json"
    exit 1
fi

# 创建必要的目录
mkdir -p certs www logs

# 生成示例网站内容用于伪装
if [ ! -f "www/index.html" ]; then
    cat > www/index.html << 'EOF'
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>企业级网络解决方案</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background: #f5f5f5; }
        .container { max-width: 1200px; margin: 0 auto; background: white; padding: 40px; border-radius: 8px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
        h1 { color: #2c3e50; text-align: center; margin-bottom: 30px; }
        .services { display: grid; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); gap: 20px; margin: 30px 0; }
        .service { padding: 20px; border: 1px solid #ddd; border-radius: 5px; }
        .footer { text-align: center; margin-top: 40px; color: #666; }
    </style>
</head>
<body>
    <div class="container">
        <h1>企业级网络解决方案</h1>
        <p>我们提供专业的网络基础设施服务，为企业客户提供稳定、安全的网络连接解决方案。</p>
        
        <div class="services">
            <div class="service">
                <h3>网络架构设计</h3>
                <p>专业的网络拓扑设计和优化服务，确保企业网络的高效运行。</p>
            </div>
            <div class="service">
                <h3>安全防护</h3>
                <p>多层安全防护体系，保障企业数据和网络通信安全。</p>
            </div>
            <div class="service">
                <h3>性能监控</h3>
                <p>实时网络性能监控和分析，及时发现和解决网络问题。</p>
            </div>
        </div>
        
        <div class="footer">
            <p>&copy; 2024 网络科技有限公司. 保留所有权利.</p>
        </div>
    </div>
</body>
</html>
EOF
    echo "[信息] 已创建伪装网站内容"
fi

echo "启动参数："
echo "- 监听端口: 1080 (SOCKS5), 8080 (HTTP), 443 (HTTPS)"
echo "- 隐身隧道: 启用"
echo "- 协议伪装: HTTPS"
echo "- 流量混淆: 启用"
echo "- 深度包检测规避: 启用"
echo "- 协议跳跃: 启用 (每30分钟切换)"
echo "- 时序抖动: 启用 (0-100ms)"
echo

# 启动服务
echo "正在启动 ProxyBridge 隐身代理服务器..."
./proxy \
  --config config/stealth_config.json \
  --stealth_tunnel true \
  --stealth_protocol https \
  --enable_traffic_analysis_evasion true \
  --enable_deep_packet_inspection_evasion true \
  --enable_protocol_hopping true \
  --enable_timing_jitter true \
  --scramble true \
  --noise_length 4096 \
  --doc_directory ./www/ \
  --autoindex true \
  --ssl_cert_path ./certs/ \
  2>&1 | tee logs/proxy_stealth.log