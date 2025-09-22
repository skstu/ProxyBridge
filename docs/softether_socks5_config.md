# SoftEther VPN Client SOCKS5 代理配置示例

## 1. 命令行配置方式

```bash
# 连接到 VPN Client 服务
vpncmd localhost /CLIENT

# 创建新的连接配置
NicCreate "MyVPN"

# 设置 SOCKS5 代理
AccountCreate "MyConnection" /SERVER:vpn.server.com:443 /HUB:VPN /USERNAME:vpnuser /NICNAME:"MyVPN"

# 配置 SOCKS5 代理设置
AccountProxySet "MyConnection" /TYPE:SOCKS5 /SERVER:your.proxy.server.com:1080 /USERNAME:proxy_user /PASSWORD:proxy_pass
```

## 2. 配置文件方式 (vpn_client.config)

```ini
# VPN Client Configuration File

declare root
{
    uint ConfigRevision 1
    
    declare VPN_Client
    {
        declare Account
        {
            declare MyConnection
            {
                string ServerName vpn.server.com
                uint ServerPort 443
                string ProxyType SOCKS5
                string ProxyName your.proxy.server.com
                uint ProxyPort 1080
                string ProxyUsername proxy_user
                string ProxyPassword proxy_pass
                bool ProxyDhcp false
                
                # VPN 连接设置
                string HubName VPN
                string Username vpnuser
                string Password vpnpassword
                
                # 其他设置...
            }
        }
    }
}
```

## 3. 批处理脚本配置

```batch
@echo off
echo 配置 SoftEther VPN Client SOCKS5 代理连接

vpncmd localhost /CLIENT /CSV /CMD NicCreate MyVPN
vpncmd localhost /CLIENT /CSV /CMD AccountCreate MyConnection /SERVER:vpn.server.com:443 /HUB:VPN /USERNAME:vpnuser /NICNAME:MyVPN
vpncmd localhost /CLIENT /CSV /CMD AccountProxySet MyConnection /TYPE:SOCKS5 /SERVER:your.proxy.server.com:1080 /USERNAME:proxy_user /PASSWORD:proxy_pass
vpncmd localhost /CLIENT /CSV /CMD AccountConnect MyConnection

echo VPN 连接已配置并启动
pause
```