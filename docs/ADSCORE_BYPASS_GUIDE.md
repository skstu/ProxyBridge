# AdScore检测绕过指南

## 🎯 概述

本项目为您的ProxyBridge代理服务器添加了强大的反检测功能，专门设计用于绕过AdScore等高级指纹检测系统。

## 🛡️ 核心反检测技术

### 1. 零检测HTTP转发
- **功能**: 完全模拟真实Web服务器行为
- **配置**: `zero_detection_mode_ = true`
- **特点**: 对真实浏览器返回完整网站内容，对代理请求建立透明隧道

### 2. SOCKS5协议混淆
- **功能**: 将SOCKS5协议包装在HTTP响应中
- **配置**: `socks5_obfuscation_ = true`
- **等级**: `obfuscation_level_ = 1-5` (5为最强)

### 3. 超级透明模式
- **功能**: 使用系统原生缓冲区和传输方式
- **配置**: `ultra_transparent_mode_ = true`
- **特点**: 完全模拟直连的网络特征

### 4. 多重协议伪装
- **TLS伪装**: 发送真实TLS握手数据然后切换
- **WebSocket隧道**: 在WebSocket协议内传输代理数据
- **HTTP隧道**: 纯HTTP CONNECT代理模式

### 5. 行为模拟
- **人类行为**: 随机延迟和网络抖动模拟
- **浏览器特征**: 模拟Chrome/Firefox的网络行为
- **时序混淆**: 随机化数据包时序

## ⚙️ 配置选项详解

### 基础反检测配置
```cpp
// 启用零检测模式（最重要）
opt.zero_detection_mode_ = true;

// 启用超级透明传输
opt.ultra_transparent_mode_ = true;

// 启用协议混淆
opt.socks5_obfuscation_ = true;
opt.obfuscation_level_ = 5;  // 1-5级别
```

### 高级伪装配置
```cpp
// 浏览器行为模拟
opt.human_behavior_simulation_ = true;
opt.browser_tls_mimicry_ = true;

// Web服务器伪装
opt.web_server_disguise_mode_ = true;
opt.nginx_camouflage_ = true;

// 深度协议隐藏
opt.stealth_protocol_mode_ = true;
opt.content_sniffing_deception_ = true;
```

### 极限隐蔽配置
```cpp
// 禁用所有检测特征
opt.disable_all_detection_ = true;

// DPI绕过
opt.dpi_bypass_mode_ = true;

// 协议隧道和端口伪装
opt.protocol_tunnel_mode_ = true;
opt.port_masquerade_mode_ = true;
```

## 🚀 快速开始

### 1. 编译代理服务器
```bash
cd ProxyBridge
mkdir build && cd build
cmake ..
make -j4
```

### 2. 使用预设配置
选择以下配置之一：

#### AdScore绕过模式（推荐）
```cpp
auto opt = create_adscore_bypass_config();
```

#### HTTP-only模式
```cpp
auto opt = create_http_only_config();
```

#### 极限隐蔽模式
```cpp
auto opt = create_ultimate_stealth_config();
```

### 3. 配置浏览器
```bash
# Chrome
--proxy-server=socks5://127.0.0.1:1080

# Firefox
网络设置 -> 代理 -> SOCKS v5: 127.0.0.1:1080
```

## 🧪 测试方法

### 1. AdScore测试
1. **直连基准测试**
   - 访问 https://adscore.com/demo/
   - 记录所有检测结果

2. **代理测试**
   - 启用代理后重新访问
   - 对比检测结果

3. **成功标准**
   - 浏览器指纹与直连一致
   - 无"Proxy Detection"警告
   - AdScore评分相近

### 2. 其他检测网站
- https://browserleaks.com/proxy
- https://whoer.net/
- https://ipleak.net/

## 🔧 高级配置

### 混淆等级说明
- **等级1**: 基础HTTP包装
- **等级2**: 添加随机延迟
- **等级3**: 数据包填充
- **等级4**: TLS握手伪装
- **等级5**: 完整协议复用

### 性能优化
```cpp
// 高性能模式（降低混淆）
opt.obfuscation_level_ = 2;
opt.human_behavior_simulation_ = false;

// 高隐蔽模式（牺牲性能）
opt.obfuscation_level_ = 5;
opt.human_behavior_simulation_ = true;
opt.dpi_bypass_mode_ = true;
```

## 📊 检测绕过原理

### 1. 协议层绕过
- **SOCKS5混淆**: 将SOCKS协议包装在HTTP中
- **TLS伪装**: 模拟真实TLS连接
- **WebSocket隧道**: 使用标准Web协议

### 2. 行为层绕过
- **时序模拟**: 模拟真实网络延迟
- **缓冲区模拟**: 使用浏览器相同的缓冲区大小
- **连接模拟**: 模拟浏览器连接模式

### 3. 内容层绕过
- **HTTP响应**: 返回真实网站内容
- **错误伪装**: 错误信息伪装成服务器错误
- **Header伪装**: 完整的HTTP服务器头部

## ⚠️ 注意事项

### 使用限制
- 代理IP仍会被检测（这是正常的）
- 首次连接可能稍慢
- 高混淆等级会影响性能

### 安全考虑
- 仅用于合法的隐私保护目的
- 遵守当地法律法规
- 不要用于恶意活动

### 兼容性
- 支持所有主流浏览器
- 兼容HTTP/HTTPS网站
- 支持WebSocket连接

## 🐛 故障排除

### 常见问题

#### 1. 仍被检测为代理
```bash
# 尝试更高的混淆等级
opt.obfuscation_level_ = 5;
opt.disable_all_detection_ = true;

# 或切换到HTTP-only模式
opt.disable_socks5_completely_ = true;
```

#### 2. 连接速度慢
```bash
# 降低混淆等级
opt.obfuscation_level_ = 2;
opt.human_behavior_simulation_ = false;
```

#### 3. 某些网站无法访问
```bash
# 启用更多伪装模式
opt.web_server_disguise_mode_ = true;
opt.content_sniffing_deception_ = true;
```

## 📈 效果评估

### 成功指标
- ✅ AdScore检测通过
- ✅ 浏览器指纹一致
- ✅ WebRTC检测通过
- ✅ DNS泄露防护

### 失败指标
- ❌ 显示"Proxy Detected"
- ❌ 异常的浏览器指纹
- ❌ 连接被拒绝

## 🔄 更新日志

### v2.0 (当前版本)
- 新增零检测HTTP转发
- 添加SOCKS5协议混淆
- 实现多重协议伪装
- 支持行为模拟

### 计划功能
- AI驱动的自适应绕过
- 更多协议支持
- 云端特征库更新

---

💡 **提示**: 如果您在使用过程中遇到问题，请先尝试不同的配置模式，大多数检测问题都可以通过调整混淆等级和伪装模式来解决。