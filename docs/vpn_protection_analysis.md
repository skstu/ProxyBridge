/*
VPN隧道如何保护代理流量：

1. 流量封装：
   原始流量: [HTTP Request] -> Target
   VPN封装后: [VPN Header][Encrypted[HTTP Request]] -> VPN Gateway -> Target

2. 元数据保护：
   - 目标服务器无法看到真实源IP
   - 中间路由器无法分析具体协议
   - 连接时间和模式被VPN流量掩盖

3. 证书链变化：
   直接连接: Client -> Proxy -> Target (暴露Proxy的TLS特征)
   VPN连接: Client -> Proxy -> VPN -> Target (显示为正常用户流量)
*/