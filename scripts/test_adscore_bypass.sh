#!/bin/bash

# AdScore检测绕过测试脚本
# 用于自动化测试不同配置的效果

echo "🛡️ AdScore检测绕过测试工具"
echo "=================================="

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 测试配置数组
declare -a TEST_CONFIGS=(
    "standard:标准SOCKS5:1080"
    "transparent:超级透明模式:1081" 
    "http_only:HTTP-only模式:8080"
    "stealth:隐蔽模式:1082"
    "ultimate:极限模式:8443"
)

# 测试URL列表
declare -a TEST_URLS=(
    "https://adscore.com/demo/"
    "https://browserleaks.com/proxy"
    "https://whoer.net/"
    "https://ipleak.net/"
)

print_header() {
    echo -e "${BLUE}=====================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}=====================================${NC}"
}

print_success() {
    echo -e "${GREEN}✅ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠️  $1${NC}"
}

print_error() {
    echo -e "${RED}❌ $1${NC}"
}

# 检查代理是否运行
check_proxy() {
    local port=$1
    local config_name=$2
    
    echo -n "检查代理端口 $port..."
    if nc -z localhost $port 2>/dev/null; then
        print_success "代理服务器 ($config_name) 正在运行"
        return 0
    else
        print_error "代理服务器 ($config_name) 未运行"
        return 1
    fi
}

# 运行代理检测测试
test_proxy_detection() {
    local proxy_url=$1
    local test_name=$2
    
    echo ""
    print_header "测试配置: $test_name"
    echo "代理地址: $proxy_url"
    
    # 使用curl测试代理连接
    echo -n "测试代理连接..."
    if curl -s --proxy "$proxy_url" --connect-timeout 10 "https://httpbin.org/ip" > /dev/null; then
        print_success "代理连接正常"
    else
        print_error "代理连接失败"
        return 1
    fi
    
    # 测试AdScore检测
    echo -n "测试AdScore检测..."
    local adscore_result=$(curl -s --proxy "$proxy_url" --connect-timeout 30 "https://adscore.com/demo/" | grep -i "proxy\|detected\|blocked" || echo "clean")
    
    if [[ "$adscore_result" == "clean" ]]; then
        print_success "AdScore检测通过"
    else
        print_warning "AdScore可能检测到代理特征"
        echo "检测内容: $adscore_result"
    fi
    
    # 测试IP泄露
    echo -n "测试IP泄露..."
    local real_ip=$(curl -s "https://httpbin.org/ip" | grep -o '"origin": "[^"]*"' | cut -d'"' -f4)
    local proxy_ip=$(curl -s --proxy "$proxy_url" "https://httpbin.org/ip" | grep -o '"origin": "[^"]*"' | cut -d'"' -f4)
    
    if [[ "$real_ip" != "$proxy_ip" ]]; then
        print_success "IP已成功更改 ($real_ip -> $proxy_ip)"
    else
        print_error "IP未更改，可能存在泄露"
    fi
    
    return 0
}

# 生成Chrome启动命令
generate_chrome_command() {
    local proxy_type=$1
    local proxy_port=$2
    
    if [[ "$proxy_type" == "http" ]]; then
        echo "google-chrome --proxy-server=http://127.0.0.1:$proxy_port --user-data-dir=/tmp/chrome-proxy-test"
    else
        echo "google-chrome --proxy-server=socks5://127.0.0.1:$proxy_port --user-data-dir=/tmp/chrome-proxy-test"
    fi
}

# 生成Firefox配置
generate_firefox_config() {
    local proxy_type=$1
    local proxy_port=$2
    
    echo "Firefox代理配置:"
    echo "网络设置 -> 手动代理配置"
    if [[ "$proxy_type" == "http" ]]; then
        echo "HTTP代理: 127.0.0.1 端口: $proxy_port"
    else
        echo "SOCKS v5: 127.0.0.1 端口: $proxy_port"
    fi
}

# 主测试流程
main_test() {
    print_header "AdScore检测绕过自动化测试"
    
    echo "开始扫描可用的代理配置..."
    echo ""
    
    # 检测可用的代理配置
    available_proxies=()
    
    for config in "${TEST_CONFIGS[@]}"; do
        IFS=':' read -r config_id config_name config_port <<< "$config"
        
        if check_proxy "$config_port" "$config_name"; then
            if [[ "$config_id" == "http_only" ]]; then
                available_proxies+=("http://127.0.0.1:$config_port:$config_name")
            else
                available_proxies+=("socks5://127.0.0.1:$config_port:$config_name")
            fi
        fi
    done
    
    if [[ ${#available_proxies[@]} -eq 0 ]]; then
        print_error "未发现运行中的代理服务器"
        echo ""
        echo "请先启动代理服务器："
        echo "1. 编译项目: cd build && make"
        echo "2. 运行代理: ./stealth_proxy_config"
        exit 1
    fi
    
    echo ""
    print_success "发现 ${#available_proxies[@]} 个可用代理配置"
    echo ""
    
    # 测试每个可用的代理
    for proxy_config in "${available_proxies[@]}"; do
        IFS=':' read -r proxy_type proxy_host proxy_port proxy_name <<< "$proxy_config"
        local proxy_url="${proxy_type}://${proxy_host}:${proxy_port}"
        
        test_proxy_detection "$proxy_url" "$proxy_name"
        echo ""
        echo "按Enter继续下一个测试..."
        read -r
    done
    
    print_header "测试完成"
    
    # 生成浏览器配置说明
    echo ""
    echo "🌐 浏览器配置说明:"
    echo ""
    
    for proxy_config in "${available_proxies[@]}"; do
        IFS=':' read -r proxy_type proxy_host proxy_port proxy_name <<< "$proxy_config"
        
        echo "[$proxy_name]"
        if [[ "$proxy_type" == "http" ]]; then
            echo "Chrome: $(generate_chrome_command "http" "$proxy_port")"
            echo ""
            generate_firefox_config "http" "$proxy_port"
        else
            echo "Chrome: $(generate_chrome_command "socks" "$proxy_port")"
            echo ""
            generate_firefox_config "socks" "$proxy_port"
        fi
        echo ""
    done
}

# 快速测试模式
quick_test() {
    echo "🚀 快速测试模式"
    echo ""
    
    # 检查最常用的端口
    common_ports=(1080 8080 1081 1082)
    
    for port in "${common_ports[@]}"; do
        if nc -z localhost $port 2>/dev/null; then
            if [[ $port -eq 8080 ]]; then
                test_proxy_detection "http://127.0.0.1:$port" "HTTP代理 ($port)"
            else
                test_proxy_detection "socks5://127.0.0.1:$port" "SOCKS5代理 ($port)"
            fi
            break
        fi
    done
}

# 显示使用帮助
show_help() {
    echo "AdScore检测绕过测试工具"
    echo ""
    echo "用法:"
    echo "  $0 [选项]"
    echo ""
    echo "选项:"
    echo "  -h, --help     显示帮助信息"
    echo "  -q, --quick    快速测试模式"
    echo "  -f, --full     完整测试模式 (默认)"
    echo ""
    echo "示例:"
    echo "  $0              # 运行完整测试"
    echo "  $0 --quick      # 运行快速测试" 
    echo ""
}

# 命令行参数处理
case "${1:-}" in
    -h|--help)
        show_help
        exit 0
        ;;
    -q|--quick)
        quick_test
        ;;
    -f|--full|"")
        main_test
        ;;
    *)
        echo "未知选项: $1"
        show_help
        exit 1
        ;;
esac