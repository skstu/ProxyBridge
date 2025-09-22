# 🛡️ SOCKS5反检测终极配置指南

## 问题分析
即使使用了透明传输，仍可能被检测的原因：

### 1. **网络层特征**
- TCP窗口大小异常
- 数据包时序过于规律
- 连接建立模式被识别

### 2. **应用层特征**  
- SOCKS5握手时序
- 数据传输模式
- 连接复用特征

## 🎯 终极反检测方案

### 方案1: 修改Chromium启动参数
```bash
chrome.exe \
  --proxy-server="socks5://127.0.0.1:1080" \
  --disable-features=VizDisplayCompositor \
  --disable-gpu-process-crash-limit \
  --disable-software-rasterizer \
  --disable-background-timer-throttling \
  --disable-renderer-backgrounding \
  --disable-backgrounding-occluded-windows \
  --disable-ipc-flooding-protection
```

### 方案2: 系统级TCP参数调整 (Windows)
```cmd
# 在管理员命令行执行
netsh int tcp set global chimney=enabled
netsh int tcp set global rss=enabled
netsh int tcp set global autotuninglevel=normal
netsh int tcp set global ecncapability=enabled
```

### 方案3: 使用系统代理而非应用代理
- 设置系统代理而不是Chrome内代理
- 避免应用层的代理检测

### 方案4: 端口跳转 (推荐)
- 代理监听127.0.0.1:1080  
- Chrome连接127.0.0.1:8080
- 8080端口做纯TCP转发到1080

### 方案5: 修改源码的关键点
在 `start_connect_host` 函数中：
```cpp
// 完全不设置任何TCP选项
// remote_socket.set_option(...) // 删除所有这类调用
```

## 🔍 检测是否成功
访问以下网站测试：
- https://whoer.net/
- https://browserleaks.com/
- https://ipleak.net/

如果显示的IP和真实情况一致，且没有"代理检测"警告，则成功。

## ⚠️ 注意事项
1. 重启Chrome浏览器测试
2. 清除浏览器缓存
3. 使用隐私模式测试  
4. 检查系统防火墙设置