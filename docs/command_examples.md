# ProxyBridge 命令行配置示例

## 基础配置

### 1. 简单代理服务器
```bash
# 在端口 1080 启动基础代理服务
./proxy --server_listen "[::0]:1080" --auth_users "admin:password123"
```

### 2. 多端口监听
```bash
# 同时监听多个端口
./proxy --server_listen "0.0.0.0:1080" "[::0]:8080" "127.0.0.1:9999"
```

## SSL/TLS 配置

### 3. 带 SSL 证书的安全代理
```bash
./proxy \
  --server_listen "[::0]:443" \
  --ssl_certificate_dir "/path/to/certs" \
  --ssl_ciphers "ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305:!aNULL:!MD5:!RC4" \
  --ssl_prefer_server_ciphers true \
  --auth_users "secure_user:VeryStrongPass@2024"
```

### 4. 上游代理 SSL 配置
```bash
./proxy \
  --server_listen "0.0.0.0:8080" \
  --proxy_pass "socks5://user:pass@upstream.proxy.com:1080" \
  --proxy_pass_ssl true \
  --proxy_ssl_name "secure.proxy.com" \
  --ssl_ciphers "TLS_AES_128_GCM_SHA256:TLS_AES_256_GCM_SHA384:ECDHE-RSA-AES128-GCM-SHA256"
```

## 反检测配置

### 5. AdScore 反检测配置
```bash
./proxy \
  --server_listen "0.0.0.0:443" \
  --ssl_certificate_dir "/etc/ssl/certs" \
  --ssl_ciphers "TLS_AES_128_GCM_SHA256:TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256" \
  --scramble true \
  --noise_length 2048 \
  --disable_insecure true \
  --http_doc "/var/www/html" \
  --autoindex false \
  --auth_users "api_user:ApiKey123!@#"
```

### 6. 浏览器流量模拟
```bash
./proxy \
  --server_listen "[::0]:8080" \
  --ssl_ciphers "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-SHA256" \
  --http_doc "/opt/fake_website" \
  --autoindex true \
  --proxy_pass "https://real.proxy.com:443" \
  --proxy_ssl_name "api.real.proxy.com"
```

## 高性能配置

### 7. 高并发优化配置
```bash
./proxy \
  --server_listen "0.0.0.0:1080" \
  --reuse_port true \
  --happyeyeballs true \
  --tcp_timeout 300 \
  --udp_timeout 60 \
  --rate_limit 10485760 \
  --auth_users "admin:SecurePass2024" "user1:UserPass123" \
  --users_rate_limit "admin:20971520" "user1:5242880"
```

### 8. 透明代理配置 (Linux)
```bash
./proxy \
  --server_listen "0.0.0.0:8080" \
  --transparent true \
  --so_mark 100 \
  --local_ip "192.168.1.100" \
  --disable_http false \
  --disable_socks false
```

## 地区限制配置

### 9. 地区访问控制
```bash
./proxy \
  --server_listen "[::0]:1080" \
  --ipip_db "/opt/ipip/17monipdb.datx" \
  --allow_region "中国" "香港" "台湾" "192.168.0.0/16" \
  --deny_region "美国" "日本" "10.0.0.0/8" \
  --auth_users "china_user:ChinaPass123"
```

### 10. IP 白名单配置
```bash
./proxy \
  --server_listen "127.0.0.1:1080" \
  --allow_region "192.168.1.0/24" "10.0.0.0/8" "2001:db8::/32" \
  --auth_users "local_admin:LocalAdminPass"
```

## Web 服务器伪装

### 11. 完整 Web 服务器伪装
```bash
./proxy \
  --server_listen "[::0]:80" "[::0]:443" \
  --ssl_certificate_dir "/etc/letsencrypt/live" \
  --http_doc "/var/www/html" \
  --autoindex true \
  --htpasswd true \
  --auth_users "webadmin:WebAdmin2024" "visitor:VisitorPass" \
  --ssl_ciphers "ECDHE+AESGCM:ECDHE+CHACHA20:DHE+AESGCM:DHE+CHACHA20:!aNULL:!MD5:!DSS"
```

### 12. 隐蔽代理服务
```bash
./proxy \
  --server_listen "0.0.0.0:443" \
  --ssl_certificate_dir "/opt/certs" \
  --http_doc "/opt/fake_site" \
  --htpasswd false \
  --scramble true \
  --noise_length 4096 \
  --ssl_prefer_server_ciphers true \
  --proxy_pass "socks5://hidden:service@127.0.0.1:9050"
```

## 多层代理配置

### 13. 双重代理配置
```bash
./proxy \
  --server_listen "0.0.0.0:8080" \
  --proxy_pass "https://user:pass@intermediate.proxy.com:443" \
  --proxy_pass_ssl true \
  --proxy_ssl_name "secure.intermediate.com" \
  --ssl_ciphers "ECDHE-RSA-CHACHA20-POLY1305:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-AES256-GCM-SHA384" \
  --scramble true \
  --auth_users "tunnel_user:TunnelPass123"
```

### 14. Tor 网络集成
```bash
./proxy \
  --server_listen "127.0.0.1:8080" \
  --proxy_pass "socks5://127.0.0.1:9050" \
  --scramble true \
  --noise_length 1024 \
  --disable_logs true \
  --auth_users "tor_user:AnonymousPass"
```

## 开发调试配置

### 15. 开发测试配置
```bash
./proxy \
  --server_listen "127.0.0.1:1080" \
  --logs_path "/tmp/proxy_logs" \
  --disable_logs false \
  --auth_users "dev:dev123" \
  --rate_limit 1048576 \
  --tcp_timeout 30
```

### 16. 最小化配置
```bash
./proxy \
  --server_listen "0.0.0.0:1080" \
  --auth_users "simple:password"
```

## 安全增强配置

### 17. 最高安全级别
```bash
./proxy \
  --server_listen "[::0]:443" \
  --ssl_certificate_dir "/etc/ssl/private" \
  --ssl_ciphers "ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305" \
  --ssl_prefer_server_ciphers true \
  --scramble true \
  --noise_length 4095 \
  --disable_insecure true \
  --disable_logs true \
  --auth_users "secure_admin:Str0ng!P@ssw0rd#2024" \
  --users_rate_limit "secure_admin:52428800"
```

### 18. 仅 SOCKS 安全代理
```bash
./proxy \
  --server_listen "127.0.0.1:1080" \
  --disable_http true \
  --disable_udp false \
  --disable_insecure true \
  --scramble true \
  --auth_users "socks_only:SocksOnlyPass123"
```

## 配置文件示例

### 19. 使用配置文件
```bash
# 创建配置文件 proxy.conf
./proxy --config proxy.conf
```

### 20. IPv6 专用配置
```bash
./proxy \
  --server_listen "[::]:1080" "[2001:db8::1]:8080" \
  --v6only true \
  --auth_users "ipv6_user:IPv6Pass2024" \
  --allow_region "2001:db8::/32" "fe80::/10"
```

## 注意事项

1. **密码安全**: 使用强密码，避免使用默认密码
2. **端口选择**: 避免使用常见的代理端口，选择随机端口
3. **SSL 配置**: 使用最新的加密算法，禁用过时的协议
4. **噪声注入**: 在需要反检测时启用 scramble 和适当的 noise_length
5. **日志管理**: 在生产环境中考虑禁用日志或使用安全的日志路径
6. **地区限制**: 根据实际需求配置 IP 白名单和黑名单
7. **性能调优**: 根据服务器性能调整速率限制和超时设置