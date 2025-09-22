#if !defined(__8574F64B_F7CE_4AD6_8994_35C03493C0E4__)
#define __8574F64B_F7CE_4AD6_8994_35C03493C0E4__

#include "util.h"

#include <mutex>
#include <memory>
#include <atomic>
#include <thread>
#include <limits>
#include <vector>

#include <boost/asio/io_context.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/signal_set.hpp>

#include <boost/nowide/args.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/program_options.hpp>
#include <boost/process.hpp>

#include <algorithm>
#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <random>
#include <regex>
#include <thread>
#include <tuple>
#include <typeinfo>
#include <any>
#include <bitset>
#include <deque>
#include <future>
#include <optional>
#include <string_view>
#include <variant>

#include "../include/ibridge.h"
#include "../include/xsiumio.hpp"

class BridgeService final : public IBridge {
public:
	enum class Status {
		Stoped = 0,
		Starting = 1,
		Started = 2,
		Stoping = 3,
		Running = 4,
	};
public:
	static BridgeService* CreateOrGet();
	static void Destroy();
public:
	bool Start(const char*, unsigned long) override final;
	void Stop() override final;
	bool Ready() const override final;
private:
	BridgeService();
	~BridgeService();
	void Init();
	void UnInit();
	void Process();
	bool Working() const;
private:
	std::vector<std::string> server_listens;
	std::vector<std::string> auth_users;
	std::vector<std::string> users_rate_limit;
	std::vector<std::string> deny_region;
	std::vector<std::string> allow_region;

	std::string ipip_db;
	std::string doc_dir;
	std::string log_dir;
	std::string local_ip;
	std::string proxy_pass;
	std::string ssl_cert_dir;
	std::string ssl_cacert_dir;
	std::string ssl_ciphers;
	std::string ssl_dhparam;
	std::string proxy_ssl_name;

	bool transparent = false;
	bool autoindex = true;
	bool htpasswd = false;

	bool disable_http = false;
	bool disable_insecure = false;
	bool disable_logs = true;
	bool disable_socks = false;
	bool disable_udp = false;

	bool happyeyeballs = true;
	bool connect_v6only = false;
	bool connect_v4only = false;
	bool proxy_pass_ssl = false;
	bool reuse_port = false;
	bool scramble = false;
	bool ssl_prefer_server_ciphers = false;

	int64_t linux_so_mark = 0;
	uint16_t noise_length = 0;
	int udp_timeout = 0;
	int tcp_timeout = 0;
	int rate_limit = 0;
public:
	const std::vector<std::string>& ServerListens() const;
	const std::vector<std::string>& AuthUsers() const;
	const std::vector<std::string>& UsersRateLimit() const;
	const std::vector<std::string>& DenyRegion() const;
	void DenyRegion(const std::vector<std::string>&);
	const std::vector<std::string>& AllowRegion() const;
	void AllowRegion(const std::vector<std::string>&);
	const std::string& IpipDb() const;
	const std::string& DocDir() const;
	const std::string& LogDir() const;
	const std::string& LocalIp() const;
	const std::string& ProxyPass() const;
	const std::string& SslCertDir() const;
	const std::string& SslCacertDir() const;
	const std::string& SslCiphers() const;
	const std::string& SslDhparam() const;
	const std::string& ProxySslName() const;
	const bool& Transparent() const;
	const bool& Autoindex() const;
	const bool& Htpasswd() const;
	const bool& DisableHttp() const;
	const bool& DisableInsecure() const;
	const bool& DisableLogs() const;
	const bool& DisableSocks() const;
	const bool& DisableUdp() const;
	const bool& Happyeyeballs() const;
	const bool& ConnectV6only() const;
	const bool& ConnectV4only() const;
	const bool& ProxyPassSsl() const;
	const bool& ReusePort() const;
	const bool& Scramble() const;
	const bool& SslPreferServerCiphers() const;
	const int64_t& LinuxSoMark() const;
	const uint16_t& NoiseLength() const;
	const int& UdpTimeout() const;
	const int& TcpTimeout() const;
	const int& Ratelimit() const;
private:
	Status status_ = Status::Stoped;
	std::atomic_bool open_ = false;
	std::vector<std::thread> threads_;
	std::unique_ptr<boost::asio::io_context> ioc_= std::make_unique<boost::asio::io_context>(1);
	std::unique_ptr<boost::program_options::variables_map> variables_map_;
	std::unique_ptr<std::mutex> mtx_ = std::make_unique<std::mutex>();
	chromium::xsiumio::IXSiumio configure_;
	void InitConfigure();
};

/// /*_ Memade®（新生™） _**/
/// /*_ Sun, 07 Sep 2025 00:32:40 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__8574F64B_F7CE_4AD6_8994_35C03493C0E4__
