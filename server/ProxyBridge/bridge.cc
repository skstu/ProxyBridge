#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable: 4005)
# pragma warning(disable: 4244)
#endif

#include "proxy/proxy_server.hpp"
#include "proxy/socks_client.hpp"
#include "proxy/logging.hpp"

#include "proxy/use_awaitable.hpp"
#include "proxy/ipip.hpp"

#ifdef USE_SNMALLOC
# define NO_BOOTSTRAP_ALLOCATOR
# ifndef NDEBUG
#  define NDEBUG
# endif
# include "src/snmalloc/override/new.cc"
#endif // USE_SNMALLOC


#include "bridge.hpp"
#ifdef _MSC_VER
# pragma warning(pop)
#endif

#if _WIN32
#include <tlhelp32.h>
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")
#endif


namespace po = boost::program_options;
namespace net = boost::asio;
using namespace proxy;

namespace std {
	static std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& users)
	{
		for (auto it = users.begin(); it != users.end();)
		{
			os << *it;
			if (++it == users.end())
				break;
			os << " ";
		}

		return os;
	}
}///namespace std
namespace {
	using server_ptr = std::shared_ptr<proxy_server>;

	static server_ptr gServerPtr;

	//////////////////////////////////////////////////////////////////////////

	static net::awaitable<void>
		start_proxy_server(net::io_context& ioc, server_ptr& server, std::function<void()> on_started)
	{
		proxy_server_option opt;
		auto& listens = opt.listens_;

		// 解析监听端口
		for (const auto& listen : BridgeService::CreateOrGet()->ServerListens()) {
			std::string host, port;
			bool v6only = false;
			if (!Util::parse_endpoint_string(listen, host, port, v6only)) {
				std::cerr << "Parse endpoint fail: " << listen << std::endl;
				co_return;
			}
			listens.emplace_back(
				tcp::endpoint{ net::ip::make_address(host), (unsigned short)atoi(port.c_str()) },
				v6only
			);
		}

		// 认证用户
		for (const auto& user : BridgeService::CreateOrGet()->AuthUsers()) {
			if (user.empty()) continue;
			auto pos = user.find(':');
			if (pos == std::string::npos)
				opt.auth_users_.emplace_back(user, "");
			else
				opt.auth_users_.emplace_back(user.substr(0, pos), user.substr(pos + 1));
		}

		// 用户限速
		for (const auto& user : BridgeService::CreateOrGet()->UsersRateLimit()) {
			if (user.empty()) continue;
			auto pos = user.find(':');
			if (pos == std::string::npos) continue;
			auto name = user.substr(0, pos);
			auto rate = std::atoi(user.substr(pos + 1).c_str());
			opt.users_rate_limit_.insert_or_assign(name, rate);
		}

		// 其他参数配置
		opt.proxy_pass_ = BridgeService::CreateOrGet()->ProxyPass();
		opt.proxy_pass_use_ssl_ = BridgeService::CreateOrGet()->ProxyPassSsl();
		opt.ssl_cert_path_ = BridgeService::CreateOrGet()->SslCertDir();
		opt.ssl_cacert_path_ = BridgeService::CreateOrGet()->SslCacertDir();
		opt.ssl_ciphers_ = BridgeService::CreateOrGet()->SslCiphers();
		opt.proxy_ssl_name_ = BridgeService::CreateOrGet()->ProxySslName();
		opt.disable_http_ = BridgeService::CreateOrGet()->DisableHttp();
		opt.disable_socks_ = BridgeService::CreateOrGet()->DisableSocks();
		opt.disable_udp_ = BridgeService::CreateOrGet()->DisableUdp();
		opt.disable_insecure_ = BridgeService::CreateOrGet()->DisableInsecure();
		opt.scramble_ = BridgeService::CreateOrGet()->Scramble();
		opt.noise_length_ = BridgeService::CreateOrGet()->NoiseLength();
		opt.local_ip_ = BridgeService::CreateOrGet()->LocalIp();
		opt.udp_timeout_ = BridgeService::CreateOrGet()->UdpTimeout();
		opt.tcp_timeout_ = BridgeService::CreateOrGet()->TcpTimeout();
		opt.tcp_rate_limit_ = BridgeService::CreateOrGet()->Ratelimit();
		opt.ipip_db_ = BridgeService::CreateOrGet()->IpipDb();

		// 区域限制
		if (!BridgeService::CreateOrGet()->DenyRegion().empty()) {
			std::vector<std::string> regions;
			for (const auto& region : BridgeService::CreateOrGet()->DenyRegion()) {
				auto ret = strutil::split(region, "|");
				regions.insert(regions.end(), ret.begin(), ret.end());
			}
			BridgeService::CreateOrGet()->DenyRegion(regions);
		}
		const auto& deny_region = BridgeService::CreateOrGet()->DenyRegion();
		opt.deny_regions_.insert(deny_region.begin(), deny_region.end());

		if (!BridgeService::CreateOrGet()->AllowRegion().empty()) {
			std::vector<std::string> regions;
			for (const auto& region : BridgeService::CreateOrGet()->AllowRegion()) {
				auto ret = strutil::split(region, "|");
				regions.insert(regions.end(), ret.begin(), ret.end());
			}
			BridgeService::CreateOrGet()->AllowRegion(regions);
		}
		const auto& allow_region = BridgeService::CreateOrGet()->AllowRegion();
		opt.allow_regions_.insert(allow_region.begin(), allow_region.end());

		// 其他选项
		opt.reuse_port_ = BridgeService::CreateOrGet()->ReusePort();
		opt.happyeyeballs_ = BridgeService::CreateOrGet()->Happyeyeballs();
		opt.connect_v4_only_ = BridgeService::CreateOrGet()->ConnectV4only();
		opt.connect_v6_only_ = BridgeService::CreateOrGet()->ConnectV6only();
		opt.transparent_ = BridgeService::CreateOrGet()->Transparent();
		const auto& linux_so_mark = BridgeService::CreateOrGet()->LinuxSoMark();
		if (linux_so_mark > 0 && linux_so_mark <= std::numeric_limits<uint32_t>::max())
			opt.so_mark_.emplace(linux_so_mark);

		opt.doc_directory_ = BridgeService::CreateOrGet()->DocDir();
		opt.autoindex_ = BridgeService::CreateOrGet()->Autoindex();
		opt.htpasswd_ = BridgeService::CreateOrGet()->Htpasswd();

		// 启动服务
		server = proxy_server::make(ioc.get_executor(), opt);
		server->start();

		// 通知 run 成功
		if (on_started)
			on_started();

		co_return;
	}

	//////////////////////////////////////////////////////////////////////////
	static unsigned short get_free_port(boost::asio::io_context& ioc) {
		using boost::asio::ip::tcp;
		tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), 0));
		unsigned short port = acceptor.local_endpoint().port();
		// acceptor 可以继续用，也可以关闭
		return port;
	}

	static std::optional<std::string> try_as_string(const boost::any& var)
	{
		try {
			return boost::any_cast<std::string>(var);
		}
		catch (const boost::bad_any_cast&) {
			return std::nullopt;
		}
	}

	static std::optional<bool> try_as_bool(const boost::any& var)
	{
		try {
			return boost::any_cast<bool>(var);
		}
		catch (const boost::bad_any_cast&) {
			return std::nullopt;
		}
	}

	static std::optional<int> try_as_int(const boost::any& var)
	{
		try {
			return boost::any_cast<int>(var);
		}
		catch (const boost::bad_any_cast&) {
			return std::nullopt;
		}
	}


	static void print_args(int argc, char** argv, const po::variables_map& vm)
	{
		XLOG_INFO << "Current directory: "
			<< fs::current_path().string();

		if (!vm.count("config"))
		{
			std::vector<std::string> args(argv, argv + argc);
			XLOG_INFO << "Run: " << boost::algorithm::join(args, " ");
			return;
		}

		for (const auto& [key, value] : vm)
		{
			if (value.empty() || key == "config")
				continue;

			if (auto s = try_as_string(value.value()))
			{
				XLOG_INFO << key << " = " << *s;
				continue;
			}

			if (auto b = try_as_bool(value.value()))
			{
				XLOG_INFO << key << " = " << *b;
				continue;
			}

			if (auto i = try_as_int(value.value()))
			{
				XLOG_INFO << key << " = " << *i;
				continue;
			}
		}
	}

#if _WIN32

	// 获取进程命令行
	static std::wstring GetProcessCommandLine(DWORD pid) {
		std::wstring cmdline;
		HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
		if (FAILED(hr)) return cmdline;
		IWbemLocator* pLoc = nullptr;
		hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
		if (FAILED(hr)) { CoUninitialize(); return cmdline; }
		IWbemServices* pSvc = nullptr;
		hr = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), nullptr, nullptr, 0, 0, 0, 0, &pSvc);
		if (FAILED(hr)) { pLoc->Release(); CoUninitialize(); return cmdline; }
		hr = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE);
		if (FAILED(hr)) { pSvc->Release(); pLoc->Release(); CoUninitialize(); return cmdline; }
		WCHAR query[128];
		swprintf_s(query, L"SELECT CommandLine FROM Win32_Process WHERE ProcessId = %u", pid);
		IEnumWbemClassObject* pEnumerator = nullptr;
		hr = pSvc->ExecQuery(_bstr_t(L"WQL"), _bstr_t(query), WBEM_FLAG_FORWARD_ONLY, nullptr, &pEnumerator);
		if (SUCCEEDED(hr)) {
			IWbemClassObject* pObj = nullptr;
			ULONG uReturn = 0;
			if (pEnumerator->Next(WBEM_INFINITE, 1, &pObj, &uReturn) == S_OK) {
				VARIANT vtProp;
				hr = pObj->Get(L"CommandLine", 0, &vtProp, 0, 0);
				if (SUCCEEDED(hr) && vtProp.vt == VT_BSTR && vtProp.bstrVal)
					cmdline = vtProp.bstrVal;
				VariantClear(&vtProp);
				pObj->Release();
			}
			pEnumerator->Release();
		}
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return cmdline;
	}

	// 查找主进程ID
	static DWORD FindChromiumBrowserProcessId(DWORD parent_pid) {
		DWORD result = 0;
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnap == INVALID_HANDLE_VALUE)
			return 0;
		PROCESSENTRY32 pe = { sizeof(pe) };
		if (Process32First(hSnap, &pe)) {
			do {
				if (pe.th32ParentProcessID == parent_pid) {
					std::wstring cmd = GetProcessCommandLine(pe.th32ProcessID);
					if (cmd.find(L"--type=browser") != std::wstring::npos || cmd.find(L"--type=") == std::wstring::npos) {
						result = pe.th32ProcessID;
						break;
					}
				}
			} while (Process32Next(hSnap, &pe));
		}
		CloseHandle(hSnap);
		return result;
	}
	static bool IsProcessRunning(const unsigned int& pid) {
		bool result = false;
		do {
			if (pid <= 0)
				break;
			// Try to open process with minimal rights to query status.
			HANDLE proc = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE,
				static_cast<DWORD>(pid));
			if (!proc) {
				DWORD err = ::GetLastError();
				// If invalid parameter, no such PID.
				if (err == ERROR_INVALID_PARAMETER)
					break;
			}
			DWORD exit_code = 0;
			BOOL ok = ::GetExitCodeProcess(proc, &exit_code);
			::CloseHandle(proc);
			if (!ok)
				break;
			if (!(exit_code == STILL_ACTIVE))
				break;
			result = true;
		} while (0);
		return result;
	}
	DWORD SpawnProcess(const char* proc, const char** args,
		const char** envp) {
		DWORD resultPid = 0;
		do {
			STARTUPINFOW si;
			PROCESS_INFORMATION pi;
			ZeroMemory(&si, sizeof(si));
			ZeroMemory(&pi, sizeof(pi));
			si.cb = sizeof(si);
			si.dwFlags = STARTF_USESHOWWINDOW;
			si.wShowWindow = /*(show_flag == 0) ? SW_HIDE : */SW_SHOW;

			if (!proc)
				break;

			// Build command line: quoted executable path + arguments
			std::string proc_ws = proc;
			std::string full_cmd = "\"";
			full_cmd.append(proc_ws);
			full_cmd.append("\"");

			if (args && (*args)) {
				size_t i = 0;
				while (args[i] != NULL) {
					full_cmd.append(" ");
					full_cmd.append(args[i]);
					++i;
				}
			}

			// CreateProcess requires a writable buffer for lpCommandLine
			std::vector<wchar_t> cmd_buf(full_cmd.begin(), full_cmd.end());
			cmd_buf.push_back(L'\0');

			BOOL inherit_handles = FALSE;
			DWORD creation_flags = 0;
			//if (show_flag == 0) {
			//	creation_flags |= CREATE_NO_WINDOW;
			//}

			// Use NULL lpApplicationName and pass full command line (writable)
			BOOL status = CreateProcessW(
				NULL,                   // lpApplicationName
				cmd_buf.data(),         // lpCommandLine (writable)
				NULL,                   // lpProcessAttributes
				NULL,                   // lpThreadAttributes
				inherit_handles,        // bInheritHandles
				creation_flags,         // dwCreationFlags
				NULL,                   // lpEnvironment
				NULL,                   // lpCurrentDirectory
				&si,                    // lpStartupInfo
				&pi);                   // lpProcessInformation

			if (status == FALSE) {
				// optional: DWORD err = GetLastError();
				break;
			}

			if (status == TRUE)
				resultPid = pi.dwProcessId;

			// Close returned handles to avoid leaks; process continues running.
			if (pi.hThread) {
				CloseHandle(pi.hThread);
				pi.hThread = NULL;
			}
			if (pi.hProcess) {
				CloseHandle(pi.hProcess);
				pi.hProcess = NULL;
			}
		} while (0);
		return resultPid;
	}
#endif

}///namespace

BridgeService::BridgeService() {
	Init();
}
BridgeService::~BridgeService() {
	UnInit();
}
void BridgeService::Init() {
	[[maybe_unused]] boost::nowide::args a(__argc, __argv);
	Util::platform_init();

	std::string config;

	boost::program_options::options_description desc("Options");
	desc.add_options()
		("help,h", "Help message.")
		("config", po::value<std::string>(&config)->value_name("config.conf"), "Load configuration options from specified file.")
		("target", po::value<std::string>(&target)->value_name(""), "Open target process path name.")
		("chromium", po::value<std::string>(&chromium)->value_name("")->default_value(""), "Chromium command line - base64 encode.")
		("server_listen", po::value<std::vector<std::string>>(&server_listens)->default_value({ "[::0]:1080" })->value_name("ip:port [ip:port ...]"), "Specify server listening address and port.")

		("reuse_port", po::value<bool>(&reuse_port)->default_value(false, "false"), "Enable TCP SO_REUSEPORT option (available since Linux 3.9).")
		("happyeyeballs", po::value<bool>(&happyeyeballs)->default_value(true, "true"), "Enable Happy Eyeballs algorithm for TCP connections.")

		("v6only", po::value<bool>(&connect_v6only)->default_value(false, "false"), "Enable IPv6 only mode for TCP connections.")
		("v4only", po::value<bool>(&connect_v4only)->default_value(false, "false"), "Enable IPv4 only mode for TCP connections.")

		("local_ip", po::value<std::string>(&local_ip), "Specify local IP for client TCP connection to server.")

		("transparent", po::value<bool>(&transparent)->default_value(false, "false"), "Enable transparent proxy mode(only linux).")
		("so_mark", po::value<int64_t>(&linux_so_mark)->default_value(-1), "Set SO_MARK for linux transparent proxy mode.")

		("udp_timeout", po::value<int>(&udp_timeout)->default_value(60), "Set UDP timeout for UDP connections.")
		("tcp_timeout", po::value<int>(&tcp_timeout)->default_value(-1), "Set TCP timeout for TCP connections.")
		("rate_limit", po::value<int>(&rate_limit)->default_value(-1), "Set TCP rate limit for connection.")

		("auth_users", po::value<std::vector<std::string>>(&auth_users)->multitoken()->default_value(std::vector<std::string>{""}), "List of authorized users(default user: jack:1111) (e.g: user1:passwd1 user2:passwd2).")
		("users_rate_limit", po::value<std::vector<std::string>>(&users_rate_limit)->multitoken(), "List of users rate limit (e.g: user1:1000000 user2:800000).")

		("allow_region", po::value<std::vector<std::string>>(&allow_region)->multitoken(), "Allow region (e.g: 北京|河南|武汉|192.168.1.2|192.168.1.0/24|2001:0db8::1|2001:db8::/32).")
		("deny_region", po::value<std::vector<std::string>>(&deny_region)->multitoken(), "Deny region (e.g: 广东|上海|山东|192.168.1.2|192.168.1.0/24|2001:0db8::1|2001:db8::/32).")

		("proxy_pass", po::value<std::string>(&proxy_pass)->default_value("")->value_name(""), "Specify next proxy pass (e.g: socks5://user:passwd@ip:port).")
		("proxy_pass_ssl", po::value<bool>(&proxy_pass_ssl)->default_value(false, "false")->value_name(""), "Enable SSL for the next proxy pass.")

		("ssl_certificate_dir", po::value<std::string>(&ssl_cert_dir)->value_name("path"), "Directory containing SSL certificates.")
		("ssl_cacert_dir", po::value<std::string>(&ssl_cacert_dir)->value_name("path"), "Directory containing SSL CA certificates.")

		("ssl_sni", po::value<std::string>(&proxy_ssl_name)->value_name("sni"), "Specifies SNI for multiple SSL certificates on one IP (Deprecated, using proxy_ssl_name instead).")
		("proxy_ssl_name", po::value<std::string>(&proxy_ssl_name)->value_name("sni"), "Specifies SNI for multiple SSL certificates on one IP.")

		("ssl_ciphers", po::value<std::string>(&ssl_ciphers)->value_name("ssl_ciphers"), "Specify enabled SSL ciphers")
		("ssl_prefer_server_ciphers", po::value<bool>(&ssl_prefer_server_ciphers)->default_value(false, "false")->value_name(""), "Prefer server ciphers over client ciphers for SSLv3 and TLS protocols.")

		("ipip_db", po::value<std::string>(&ipip_db)->value_name("")->default_value("17monipdb.datx"), "Specify ipip database filename.")
		("http_doc", po::value<std::string>(&doc_dir)->value_name("doc"), "Specify document root directory for HTTP server.")
		("htpasswd", po::value<bool>(&htpasswd)->value_name("")->default_value(false, "false"), "Enable WWW-Authenticate for HTTP server.")

		("autoindex", po::value<bool>(&autoindex)->default_value(false, "false"), "Enable directory listing.")
		("logs_path", po::value<std::string>(&log_dir)->value_name(""), "Specify directory for log files.")

		("disable_logs", po::value<bool>(&disable_logs)->value_name("")->default_value(false), "Disable logging.")
		("disable_http", po::value<bool>(&disable_http)->value_name("")->default_value(false, "false"), "Disable HTTP protocol.")
		("disable_socks", po::value<bool>(&disable_socks)->value_name("")->default_value(false, "false"), "Disable SOCKS proxy protocol.")
		("disable_udp", po::value<bool>(&disable_udp)->value_name("")->default_value(false, "false"), "Disable UDP protocol.")
		("disable_insecure", po::value<bool>(&disable_insecure)->value_name("")->default_value(false, "false"), "Disable insecure protocol.")

		("scramble", po::value<bool>(&scramble)->value_name("")->default_value(false, "false"), "Noise-based data security.")
		("noise_length", po::value<uint16_t>(&noise_length)->value_name("length")->default_value(0x0fff), "Length of the noise data.")
		;

	ioc_ = std::make_unique<boost::asio::io_context>(1);
	//	server_listens.clear();
	//	port_ = get_free_port(*ioc_);
	//	std::string defaultAddress("127.0.0.1:");
	//#if _DEBUG
	//	defaultAddress.append("55668");
	//#else
	//	defaultAddress.append(std::to_string(port_));
	//#endif
	//	server_listens.emplace_back(defaultAddress);

	variables_map_ = std::make_unique<boost::program_options::variables_map>();

	boost::program_options::store(boost::program_options::command_line_parser(__argc, __argv)
		.options(desc)
		.style(boost::program_options::command_line_style::unix_style |
			boost::program_options::command_line_style::allow_long_disguise)
		.run(),
		*variables_map_);
	boost::program_options::notify(*variables_map_);
}
void BridgeService::UnInit() {
}
bool BridgeService::Start() {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	do {
		if (open_.load())
			break;
		open_.store(true);
		status_ = Status::Starting;
		threads_.emplace_back([this]() {Process(); });
	} while (0);
	return open_.load();
}
void BridgeService::Stop() {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	do {
		if (!open_.load())
			break;
		do {
			if (status_ == Status::Started)
				break;
			if (target_pid > 0 && !IsProcessRunning(target_pid))
				break;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		} while (1);
		open_.store(false);
		status_ = Status::Stoping;
		if (gServerPtr)
			gServerPtr->close();
		ioc_->stop();
		for (auto& t : threads_)
			t.join();
		threads_.clear();
		status_ = Status::Stoped;
	} while (0);
}
const unsigned short& BridgeService::Port() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return port_;
}
const std::vector<std::string>& BridgeService::ServerListens() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return server_listens;
}
const std::vector<std::string>& BridgeService::AuthUsers() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return auth_users;
}
const std::vector<std::string>& BridgeService::UsersRateLimit() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return users_rate_limit;
}
void BridgeService::DenyRegion(const std::vector<std::string>& v) {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	deny_region.clear();
	deny_region = v;
}
const std::vector<std::string>& BridgeService::DenyRegion() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return deny_region;
}
const std::vector<std::string>& BridgeService::AllowRegion() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return allow_region;
}
void BridgeService::AllowRegion(const std::vector<std::string>& v) {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	allow_region.clear();
	allow_region = v;
}
const std::string& BridgeService::IpipDb() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return ipip_db;
}
const std::string& BridgeService::DocDir() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return doc_dir;
}
const std::string& BridgeService::LogDir() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return log_dir;
}
const std::string& BridgeService::LocalIp() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return local_ip;
}
const std::string& BridgeService::ProxyPass() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return proxy_pass;
}
const std::string& BridgeService::SslCertDir() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return ssl_cert_dir;
}
const std::string& BridgeService::SslCacertDir() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return ssl_cacert_dir;
}
const std::string& BridgeService::SslCiphers() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return ssl_ciphers;
}
const std::string& BridgeService::SslDhparam() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return ssl_dhparam;
}
const std::string& BridgeService::ProxySslName() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return proxy_ssl_name;
}
const bool& BridgeService::Transparent() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return transparent;
}
const bool& BridgeService::Autoindex() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return autoindex;
}
const bool& BridgeService::Htpasswd() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return htpasswd;
}
const bool& BridgeService::DisableHttp() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return disable_http;
}
const bool& BridgeService::DisableInsecure() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return disable_insecure;
}
const bool& BridgeService::DisableLogs() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return disable_logs;
}
const bool& BridgeService::DisableSocks() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return disable_socks;
}
const bool& BridgeService::DisableUdp() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return disable_udp;
}
const bool& BridgeService::Happyeyeballs() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return happyeyeballs;
}
const bool& BridgeService::ConnectV6only() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return connect_v6only;
}
const bool& BridgeService::ConnectV4only() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return connect_v4only;
}
const bool& BridgeService::ProxyPassSsl() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return proxy_pass_ssl;
}
const bool& BridgeService::ReusePort() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return reuse_port;
}
const bool& BridgeService::Scramble() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return scramble;
}
const bool& BridgeService::SslPreferServerCiphers() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return ssl_prefer_server_ciphers;
}
const int64_t& BridgeService::LinuxSoMark() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return linux_so_mark;
}
const uint16_t& BridgeService::NoiseLength() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return noise_length;
}
const int& BridgeService::UdpTimeout() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return udp_timeout;
}
const int& BridgeService::TcpTimeout() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return tcp_timeout;
}
const int& BridgeService::Ratelimit() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return rate_limit;
}
void BridgeService::Process() {
	do {
		std::function<void()> on_started = [this]() {
			status_ = Status::Started;
			do {
				//if (port_ <= 0)
				//	break;
				if (target.empty())
					break;
				if (!chromium.empty()) {
					std::string tmp(chromium);
					size_t desize = beast::detail::base64::decoded_size(tmp.size());
					chromium.resize(tmp.size(), 0x00);
					auto [len, _] = beast::detail::base64::decode((char*)chromium.data(), tmp.data(), tmp.size());
					chromium.resize(len, 0x00);
				}
				DWORD curPid = GetCurrentProcessId();
				std::string strPid(std::to_string(curPid));
				strPid.insert(0, "--xsppid=");
				/*
								"--host-resolver-rules=MAP * ~NOTFOUND , EXCLUDE 127.0.0.1",
				"--proxy-bypass-list=<-loopback>",
				*/
				std::vector<const char*> args = { strPid.c_str(),chromium.c_str(),nullptr };
				target_pid = SpawnProcess(target.c_str(), &args[0], nullptr);
				if (target_pid <= 0)
					break;

				status_ = Status::Running;
			} while (0);
			};

		boost::asio::co_spawn(*ioc_, start_proxy_server(*ioc_, gServerPtr, on_started), boost::asio::detached);
		ioc_->run();
	} while (0);
}
void BridgeService::Perform() {
	Start();
	do {
		if (!Working())
			break;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	} while (1);
	Stop();
}
bool BridgeService::Working() const {
	std::lock_guard<std::mutex> lck{ *mtx_ };
	return status_ == Status::Starting;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
static BridgeService* __gpsBridgeService = nullptr;
BridgeService* BridgeService::CreateOrGet() {
	if (!__gpsBridgeService)
		__gpsBridgeService = new BridgeService();
	return __gpsBridgeService;
}
void BridgeService::Destroy() {
	if (__gpsBridgeService) {
		__gpsBridgeService->Stop();
		delete __gpsBridgeService;
		__gpsBridgeService = nullptr;
	}
}
