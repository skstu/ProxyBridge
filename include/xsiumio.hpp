#if !defined(__5E6CBAF3_846B_4EF0_B587_0B9D7DC7F803__)
#define __5E6CBAF3_846B_4EF0_B587_0B9D7DC7F803__

#include "rapidjson.h"

namespace chromium {
namespace xsiumio {
typedef unsigned int tfRequestType;
typedef unsigned long long tfIdentifyType;
typedef unsigned long long tfSeedType;
constexpr char kObjectKey[] = "xsiumio";
constexpr char kProjectDirnameChromium[] = "Chromium";
constexpr char kProjectDirnameDefaultUserData[] = "default";
constexpr char kProjectDirnameComponents[] = "components";
constexpr char kProjectDirnameExtensions[] = "extensions";
constexpr char kProjectDirnameRoute[] = "route";
constexpr char kProjectDirnameFpsdb[] = "fpsdb";
constexpr char kProjectDirnameDeviceOutput[] = "devices";
constexpr char kProjectDirnameTmp[] = "tmp";
constexpr char kProjectDirnameCache[] = "cache";
constexpr char kProjectDirnameLogs[] = "logs";
constexpr char kProjectDirnameConfigures[] = "configures";
constexpr char kProjectFilenameConfigure[] = "configure.json";
constexpr char kProjectFilenameTmpcfgpath[] = "cfgpath.cfg";
constexpr char kProjectFilenameStartupEnv[] = ".startup";
constexpr char kProjectFilenameUddPath[] = "udd.path";
constexpr char kProjectFilenameChromiumProcessId[] = "chromium.pid";

constexpr char kWebRTCIPHandlingDefault[] = "default";
constexpr char kWebRTCIPHandlingDefaultPublicAndPrivateInterfaces[] =
    "default_public_and_private_interfaces";
constexpr char kWebRTCIPHandlingDefaultPublicInterfaceOnly[] =
    "default_public_interface_only";
constexpr char kWebRTCIPHandlingDisableNonProxiedUdp[] =
    "disable_non_proxied_udp";
enum class WebRTCIPHandlingPolicy {
  kDefault = 0,
  kDefaultPublicAndPrivateInterfaces = 1,
  kDefaultPublicInterfaceOnly = 2,
  kDisableNonProxiedUdp = 3,
};
enum class HttpRequestType {
  kUnknown = 0,
  kGet = 1,
  kPost = 2,
  kPut = 3,
  kDelete = 4
};
enum class RequestType : tfRequestType {
  kDefault = 0,
  kCreateBrowser = 1,
  kDestroyBrowser = 2,
  kChromiumInit = 3,
  kResetBrowser = 4,
  kServiceAsyncNotify = 5,
  kBegin = kDefault,
  kEnd = kResetBrowser,
  kUnknown = 0xFFFF,
};
inline const std::map<RequestType, std::string> &RequestTypePathMap() {
  static const std::map<RequestType, std::string> *m =
      new std::map<RequestType, std::string>{
          {RequestType::kDefault, "/"},
          {RequestType::kCreateBrowser, "browser/create"},
          {RequestType::kDestroyBrowser, "browser/destroy"},
          {RequestType::kChromiumInit, "chromium/init"},
          {RequestType::kResetBrowser, "browser/reset"},
          {RequestType::kServiceAsyncNotify, "service/async_notify"},
      };
  return *m;
}
inline const std::map<std::string, RequestType> &PathRequestTypeMap() {
  static const std::map<std::string, RequestType> *m =
      new std::map<std::string, RequestType>{
          {"/", RequestType::kDefault},
          {"browser/create", RequestType::kCreateBrowser},
          {"browser/destroy", RequestType::kDestroyBrowser},
          {"chromium/init", RequestType::kChromiumInit},
          {"browser/reset", RequestType::kResetBrowser},
          {"service/async_notify", RequestType::kServiceAsyncNotify},
      };
  return *m;
}
class IXSiumio {
public:
  enum class ProxyType { HTTP, SOCKS4, SOCKS5, SOCKS5H };
  static bool ParseProxyCredentialsUrl(const std::string &authString,
                                       ProxyType &type, std::string &scheme,
                                       int &port, std::string &username,
                                       std::string &password,
                                       std::string &host) {
    bool result = false;
    do {
      if (authString.empty())
        break;
      std::regex re(
          R"(^(https?|socks4|socks5h?|ftp)://(?:([^:]+):([^@]+)@)?([^\:]+):(\d+)$)");
      std::smatch m;
      if (!std::regex_match(authString, m, re))
        break;
      scheme = m[1];
      username = m[2];
      password = m[3];
      host = m[4];
      std::string portStr = m[5];
      port = portStr.empty() ? 80 : std::strtol(portStr.c_str(), nullptr, 10);
      if (scheme.find("socks5h") != std::string::npos) {
        type = ProxyType::SOCKS5H;
      } else if (scheme.find("socks5") != std::string::npos) {
        type = ProxyType::SOCKS5;
      } else if (scheme.find("socks4") != std::string::npos) {
        type = ProxyType::SOCKS4;
      } else {
        type = ProxyType::HTTP;
      }
      result = true;
    } while (0);
    return result;
  }
  struct Bridge {
    inline Bridge();
    inline ~Bridge();
    bool enable = false;
    bool single_process = true;
    std::string config =
        "config.conf"; // Load configuration options from specified file.
    std::string server_listen =
        "0.0.0.0:55668"; // Specify server listening address and port.
    bool reuse_port =
        false; // Enable TCP SO_REUSEPORT option (available since Linux 3.9).
    bool happyeyeballs =
        true;            // Enable Happy Eyeballs algorithm for TCP connections.
    bool v6only = false; // Enable IPv6 only mode for TCP connections.
    bool v4only = false; // Enable IPv4 only mode for TCP connections.
    std::string
        local_ip; // Specify local IP for client TCP connection to server.
    bool transparent = false; // Enable transparent proxy mode(only linux).
    int64_t so_mark = -1;     // Set SO_MARK for linux transparent proxy mode.
    int udp_timeout = 60;     // Set UDP timeout for UDP connections.
    int tcp_timeout = -1;     // Set TCP timeout for TCP connections.
    int rate_limit = -1;      // Set TCP rate limit for connection.
    std::vector<std::string> auth_users = {
        "martell:5858668",
    }; // List of authorized users(default user: jack:1111) (e.g: user1:passwd1
    // user2:passwd2).
    std::vector<std::string> users_rate_limit; // List of users rate limit (e.g:
    // user1:1000000 user2:800000).
    std::vector<std::string> allow_region; // Allow region (e.g:
    // 北京|河南|武汉|192.168.1.2|192.168.1.0/24|2001:0db8::1|2001:db8::/32).
    std::vector<std::string> deny_region; // Deny region (e.g:
    // 广东|上海|山东|192.168.1.2|192.168.1.0/24|2001:0db8::1|2001:db8::/32).
    std::string proxy_pass; // Specify next proxy pass (e.g:
    // socks5://user:passwd@ip:port).
    bool proxy_pass_ssl = false; // Enable SSL for the next proxy pass.
    std::string ssl_certificate_dir =
        ""; // Directory containing SSL certificates.
    std::string ssl_cacert_dir =
        "";              // Directory containing SSL CA certificates.
    std::string ssl_sni; // Specifies SNI for multiple SSL certificates on one
    // IP (Deprecated, using proxy_ssl_name instead).
    std::string proxy_ssl_name; // Specifies SNI for multiple SSL certificates
    // on one IP.
    std::string ssl_ciphers; // Specify enabled SSL ciphers
    bool ssl_prefer_server_ciphers =
        false; // Prefer server ciphers over client ciphers for SSLv3 and TLS
    // protocols.
    std::string ipip_db = "17monipdb.datx"; // Specify ipip database filename.
    std::string http_doc =
        "/tmp/";           // Specify document root directory for HTTP server.
    bool htpasswd = false; // Enable WWW-Authenticate for HTTP server.
    bool autoindex = true; // Enable directory listing.
    std::string logs_path = "";    // Specify directory for log files.
    bool disable_logs = true;      // Disable logging.
    bool disable_http = false;     // Disable HTTP protocol.
    bool disable_socks = false;    // Disable SOCKS proxy protocol.
    bool disable_udp = false;      // Disable UDP protocol.
    bool disable_insecure = false; // Disable insecure protocol.
    bool scramble = false;         // Noise-based data security.
    uint16_t noise_length = 0;     // Length of the noise data.

    bool IsEnable() const {
      return enable;
    }
    inline std::string GetCmdlineString() const {
      std::string result;
      if (!server_listen.empty()) {
        result.append("--server_listen=\"").append(server_listen).append("\" ");
      }
      result.append("--reuse_port=")
          .append(reuse_port ? "true" : "false")
          .append(" ");
      result.append("--happyeyeballs=")
          .append(happyeyeballs ? "true" : "false")
          .append(" ");
      result.append("--v6only=").append(v6only ? "true" : "false").append(" ");
      result.append("--v4only=").append(v4only ? "true" : "false").append(" ");
      if (!local_ip.empty()) {
        result.append("--local_ip=\"").append(local_ip).append("\" ");
      }
      result.append("--transparent=")
          .append(transparent ? "true" : "false")
          .append(" ");
      result.append("--so_mark=").append(std::to_string(so_mark)).append(" ");
      result.append("--udp_timeout=")
          .append(std::to_string(udp_timeout))
          .append(" ");
      result.append("--tcp_timeout=")
          .append(std::to_string(tcp_timeout))
          .append(" ");
      result.append("--rate_limit=")
          .append(std::to_string(rate_limit))
          .append(" ");
      if (!auth_users.empty()) {
        result.append("--auth_users=\"").append(auth_users[0]).append("\" ");
      }
      if (!proxy_pass.empty()) {
        result.append("--proxy_pass=\"").append(proxy_pass).append("\" ");
      }
      result.append("--proxy_pass_ssl=")
          .append(proxy_pass_ssl ? "true" : "false")
          .append(" ");
      if (!ssl_certificate_dir.empty()) {
        result.append("--ssl_certificate_dir=\"")
            .append(ssl_certificate_dir)
            .append("\" ");
      }
      if (!ssl_cacert_dir.empty()) {
        result.append("--ssl_cacert_dir=\"")
            .append(ssl_cacert_dir)
            .append("\" ");
      }
      if (!ssl_sni.empty()) {
        result.append("--ssl_sni=\"").append(ssl_sni).append("\" ");
      }
      if (!proxy_ssl_name.empty()) {
        result.append("--proxy_ssl_name=\"")
            .append(proxy_ssl_name)
            .append("\" ");
      }
      if (!ssl_ciphers.empty()) {
        result.append("--ssl_ciphers=\"").append(ssl_ciphers).append("\" ");
      }
      result.append("--ssl_prefer_server_ciphers=")
          .append(ssl_prefer_server_ciphers ? "true" : "false")
          .append(" ");
      if (!ipip_db.empty()) {
        result.append("--ipip_db=\"").append(ipip_db).append("\" ");
      }
      if (!http_doc.empty()) {
        result.append("--http_doc=\"").append(http_doc).append("\" ");
      }
      result.append("--htpasswd=")
          .append(htpasswd ? "true" : "false")
          .append(" ");
      result.append("--autoindex=")
          .append(autoindex ? "true" : "false")
          .append(" ");
      if (!logs_path.empty()) {
        result.append("--logs_path=\"").append(logs_path).append("\" ");
      }
      result.append("--disable_logs=")
          .append(disable_logs ? "true" : "false")
          .append(" ");
      result.append("--disable_http=")
          .append(disable_http ? "true" : "false")
          .append(" ");
      result.append("--disable_socks=")
          .append(disable_socks ? "true" : "false")
          .append(" ");
      result.append("--disable_udp=")
          .append(disable_udp ? "true" : "false")
          .append(" ");
      result.append("--disable_insecure=")
          .append(disable_insecure ? "true" : "false")
          .append(" ");
      result.append("--scramble=")
          .append(scramble ? "true" : "false")
          .append(" ");
      result.append("--noise_length=")
          .append(std::to_string(noise_length))
          .append(" ");
      return result;
    }
    inline void operator>>(std::string &output) const {
      output.clear();
      output.append("single_process")
          .append("=")
          .append(single_process ? "true" : "false")
          .append("\n");
      if (!proxy_pass.empty()) {
        output.append("proxy_pass").append("=").append(proxy_pass).append("\n");
      }
      output.append("server_listen")
          .append("=")
          .append(server_listen)
          .append("\n");
      for (const auto &u : auth_users) {
        output.append("auth_users").append("=").append(u).append("\n");
      }
      output.append("disable_insecure")
          .append("=")
          .append(disable_insecure ? "true" : "false")
          .append("\n");
      output.append("http_doc").append("=").append(http_doc).append("\n");
      output.append("autoindex")
          .append("=")
          .append(autoindex ? "true" : "false")
          .append("\n");
      output.append("disable_logs")
          .append("=")
          .append(disable_logs ? "true" : "false")
          .append("\n");
      output.append("scramble")
          .append("=")
          .append(scramble ? "true" : "false")
          .append("\n");
    }
    inline void operator>>(rapidjson::Document &doc) const {
      if (!doc.IsObject())
        return;
      rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();
      rapidjson::Value obj(rapidjson::Type::kObjectType);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("enable"), enable, allocator);
      RAPIDJSON_ADDMEMBER_STRING(obj, std::string("config"), config, allocator);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("single_process"),
                               single_process, allocator);
      RAPIDJSON_ADDMEMBER_STRING(obj, std::string("server_listen"),
                                 server_listen, allocator);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("reuse_port"), reuse_port,
                               allocator);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("happyeyeballs"), happyeyeballs,
                               allocator);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("v6only"), v6only, allocator);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("v4only"), v4only, allocator);
      RAPIDJSON_ADDMEMBER_STRING(obj, std::string("local_ip"), local_ip,
                                 allocator);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("transparent"), transparent,
                               allocator);
      RAPIDJSON_ADDMEMBER_INT64(obj, std::string("so_mark"), so_mark,
                                allocator);
      RAPIDJSON_ADDMEMBER_INT(obj, std::string("udp_timeout"), udp_timeout,
                              allocator);
      RAPIDJSON_ADDMEMBER_INT(obj, std::string("tcp_timeout"), tcp_timeout,
                              allocator);
      RAPIDJSON_ADDMEMBER_INT(obj, std::string("rate_limit"), rate_limit,
                              allocator);
      RAPIDJSON_ADDMEMBER_STRING(obj, std::string("proxy_pass"), proxy_pass,
                                 allocator);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("proxy_pass_ssl"),
                               proxy_pass_ssl, allocator);
      RAPIDJSON_ADDMEMBER_STRING(obj, std::string("ssl_certificate_dir"),
                                 ssl_certificate_dir, allocator);
      RAPIDJSON_ADDMEMBER_STRING(obj, std::string("ssl_cacert_dir"),
                                 ssl_cacert_dir, allocator);
      RAPIDJSON_ADDMEMBER_STRING(obj, std::string("ssl_sni"), ssl_sni,
                                 allocator);
      RAPIDJSON_ADDMEMBER_STRING(obj, std::string("proxy_ssl_name"),
                                 proxy_ssl_name, allocator);
      RAPIDJSON_ADDMEMBER_STRING(obj, std::string("ssl_ciphers"), ssl_ciphers,
                                 allocator);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("ssl_prefer_server_ciphers"),
                               ssl_prefer_server_ciphers, allocator);
      RAPIDJSON_ADDMEMBER_STRING(obj, std::string("ipip_db"), ipip_db,
                                 allocator);
      RAPIDJSON_ADDMEMBER_STRING(obj, std::string("http_doc"), http_doc,
                                 allocator);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("htpasswd"), htpasswd,
                               allocator);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("autoindex"), autoindex,
                               allocator);
      RAPIDJSON_ADDMEMBER_STRING(obj, std::string("logs_path"), logs_path,
                                 allocator);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("disable_logs"), disable_logs,
                               allocator);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("disable_http"), disable_http,
                               allocator);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("disable_socks"), disable_socks,
                               allocator);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("disable_udp"), disable_udp,
                               allocator);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("disable_insecure"),
                               disable_insecure, allocator);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("scramble"), scramble,
                               allocator);
      RAPIDJSON_ADDMEMBER_UINT(obj, std::string("noise_length"), noise_length,
                               allocator);
      rapidjson::Value deny_regionArray(rapidjson::Type::kArrayType);
      for (const auto &arg : deny_region) {
        if (arg.empty())
          continue;
        rapidjson::Value item(rapidjson::Type::kStringType);
        item.SetString(arg.c_str(),
                       static_cast<rapidjson::SizeType>(arg.size()), allocator);
        deny_regionArray.PushBack(item, allocator);
      }
      RAPIDJSON_ADDMEMBER_OBJECT(obj, std::string("deny_region"),
                                 deny_regionArray, allocator);

      rapidjson::Value allow_regionArray(rapidjson::Type::kArrayType);
      for (const auto &arg : allow_region) {
        if (arg.empty())
          continue;
        rapidjson::Value item(rapidjson::Type::kStringType);
        item.SetString(arg.c_str(),
                       static_cast<rapidjson::SizeType>(arg.size()), allocator);
        allow_regionArray.PushBack(item, allocator);
      }
      RAPIDJSON_ADDMEMBER_OBJECT(obj, std::string("allow_region"),
                                 allow_regionArray, allocator);

      rapidjson::Value auth_usersArray(rapidjson::Type::kArrayType);
      for (const auto &arg : auth_users) {
        if (arg.empty())
          continue;
        rapidjson::Value item(rapidjson::Type::kStringType);
        item.SetString(arg.c_str(),
                       static_cast<rapidjson::SizeType>(arg.size()), allocator);
        auth_usersArray.PushBack(item, allocator);
      }
      RAPIDJSON_ADDMEMBER_OBJECT(obj, std::string("auth_users"),
                                 auth_usersArray, allocator);

      rapidjson::Value users_rate_limitArray(rapidjson::Type::kArrayType);
      for (const auto &arg : users_rate_limit) {
        if (arg.empty())
          continue;
        rapidjson::Value item(rapidjson::Type::kStringType);
        item.SetString(arg.c_str(),
                       static_cast<rapidjson::SizeType>(arg.size()), allocator);
        users_rate_limitArray.PushBack(item, allocator);
      }
      RAPIDJSON_ADDMEMBER_OBJECT(obj, std::string("users_rate_limit"),
                                 users_rate_limitArray, allocator);

      doc.AddMember("bridge", obj, allocator);
    }
    inline void operator<<(const rapidjson::Value &v) {
      do {
        if (!v.IsObject())
          return;
        if (v.HasMember("enable") && v["enable"].IsBool())
          enable = v["enable"].GetBool();
        if (v.HasMember("single_process") && v["single_process"].IsBool())
          single_process = v["single_process"].GetBool();
        if (v.HasMember("config") && v["config"].IsString())
          config = v["config"].GetString();
        if (v.HasMember("server_listen") && v["server_listen"].IsString())
          server_listen = v["server_listen"].GetString();
        if (v.HasMember("reuse_port") && v["reuse_port"].IsBool())
          reuse_port = v["reuse_port"].GetBool();
        if (v.HasMember("happyeyeballs") && v["happyeyeballs"].IsBool())
          happyeyeballs = v["happyeyeballs"].GetBool();
        if (v.HasMember("v6only") && v["v6only"].IsBool())
          v6only = v["v6only"].GetBool();
        if (v.HasMember("v4only") && v["v4only"].IsBool())
          v4only = v["v4only"].GetBool();
        if (v.HasMember("local_ip") && v["local_ip"].IsString())
          local_ip = v["local_ip"].GetString();
        if (v.HasMember("transparent") && v["transparent"].IsBool())
          transparent = v["transparent"].GetBool();
        if (v.HasMember("so_mark") && v["so_mark"].IsInt64())
          so_mark = v["so_mark"].GetInt64();
        if (v.HasMember("udp_timeout") && v["udp_timeout"].IsInt())
          udp_timeout = v["udp_timeout"].GetInt();
        if (v.HasMember("tcp_timeout") && v["tcp_timeout"].IsInt())
          tcp_timeout = v["tcp_timeout"].GetInt();
        if (v.HasMember("rate_limit") && v["rate_limit"].IsInt())
          rate_limit = v["rate_limit"].GetInt();
        if (v.HasMember("auth_users") && v["auth_users"].IsArray()) {
          auth_users.clear();
          for (const auto &user : v["auth_users"].GetArray()) {
            if (user.IsString()) {
              auth_users.push_back(user.GetString());
            }
          }
        }
        if (v.HasMember("users_rate_limit") &&
            v["users_rate_limit"].IsArray()) {
          users_rate_limit.clear();
          for (const auto &user : v["users_rate_limit"].GetArray()) {
            if (user.IsString()) {
              users_rate_limit.push_back(user.GetString());
            }
          }
        }
        if (v.HasMember("allow_region") && v["allow_region"].IsArray()) {
          allow_region.clear();
          for (const auto &region : v["allow_region"].GetArray()) {
            if (region.IsString()) {
              allow_region.push_back(region.GetString());
            }
          }
        }
        if (v.HasMember("deny_region") && v["deny_region"].IsArray()) {
          deny_region.clear();
          for (const auto &region : v["deny_region"].GetArray()) {
            if (region.IsString()) {
              deny_region.push_back(region.GetString());
            }
          }
        }
        if (v.HasMember("proxy_pass") && v["proxy_pass"].IsString())
          proxy_pass = v["proxy_pass"].GetString();
        if (v.HasMember("proxy_pass_ssl") && v["proxy_pass_ssl"].IsBool())
          proxy_pass_ssl = v["proxy_pass_ssl"].GetBool();
        if (v.HasMember("ssl_certificate_dir") &&
            v["ssl_certificate_dir"].IsString())
          ssl_certificate_dir = v["ssl_certificate_dir"].GetString();
        if (v.HasMember("ssl_cacert_dir") && v["ssl_cacert_dir"].IsString())
          ssl_cacert_dir = v["ssl_cacert_dir"].GetString();
        if (v.HasMember("ssl_sni") && v["ssl_sni"].IsString())
          ssl_sni = v["ssl_sni"].GetString();
        if (v.HasMember("proxy_ssl_name") && v["proxy_ssl_name"].IsString())
          proxy_ssl_name = v["proxy_ssl_name"].GetString();
        if (v.HasMember("ssl_ciphers") && v["ssl_ciphers"].IsString())
          ssl_ciphers = v["ssl_ciphers"].GetString();
        if (v.HasMember("ssl_prefer_server_ciphers") &&
            v["ssl_prefer_server_ciphers"].IsBool())
          ssl_prefer_server_ciphers = v["ssl_prefer_server_ciphers"].GetBool();
        if (v.HasMember("ipip_db") && v["ipip_db"].IsString())
          ipip_db = v["ipip_db"].GetString();
        if (v.HasMember("http_doc") && v["http_doc"].IsString())
          http_doc = v["http_doc"].GetString();
        if (v.HasMember("logs_path") && v["logs_path"].IsString())
          logs_path = v["logs_path"].GetString();
        if (v.HasMember("noise_length") && v["noise_length"].IsUint())
          noise_length =
              static_cast<decltype(noise_length)>(v["noise_length"].GetUint());
        if (v.HasMember("htpasswd") && v["htpasswd"].IsBool())
          htpasswd = v["htpasswd"].GetBool();
        if (v.HasMember("autoindex") && v["autoindex"].IsBool())
          autoindex = v["autoindex"].GetBool();
        if (v.HasMember("disable_logs") && v["disable_logs"].IsBool())
          disable_logs = v["disable_logs"].GetBool();
        if (v.HasMember("disable_http") && v["disable_http"].IsBool())
          disable_http = v["disable_http"].GetBool();
        if (v.HasMember("disable_socks") && v["disable_socks"].IsBool())
          disable_socks = v["disable_socks"].GetBool();
        if (v.HasMember("disable_udp") && v["disable_udp"].IsBool())
          disable_udp = v["disable_udp"].GetBool();
        if (v.HasMember("disable_insecure") && v["disable_insecure"].IsBool())
          disable_insecure = v["disable_insecure"].GetBool();
        if (v.HasMember("scramble") && v["scramble"].IsBool())
          scramble = v["scramble"].GetBool();
      } while (0);
    }
  };
  struct Server {
    inline Server();
    inline ~Server();
    bool enable = false;
    std::string host = "localhost";
    int port = 65533;
    void operator<<(const rapidjson::Value &v) {
      do {
        if (!v.IsObject())
          return;
        if (v.HasMember("enable") && v["enable"].IsBool())
          enable = v["enable"].GetBool();
        if (v.HasMember("host") && v["host"].IsString())
          host = v["host"].GetString();
        if (v.HasMember("port") && v["port"].IsInt())
          port = v["port"].GetInt();
      } while (0);
    }
    void operator>>(rapidjson::Document &doc) const {
      if (!doc.IsObject())
        return;
      rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();
      rapidjson::Value obj(rapidjson::Type::kObjectType);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("enable"), enable, allocator);
      RAPIDJSON_ADDMEMBER_STRING(obj, std::string("host"), host, allocator);
      RAPIDJSON_ADDMEMBER_INT(obj, std::string("port"), port, allocator);
      doc.AddMember("server", obj, allocator);
    }
  };
  struct Proxy {
    ProxyType type = ProxyType::SOCKS5H;
    std::string scheme = "SOCKS5H";
    std::string username = "";
    std::string password = "";
    std::string host = "127.0.0.1";
    int port = 80;

    bool enable = false;
    bool credentials = true;
    std::string url;
    inline Proxy();
    inline ~Proxy();

    inline void operator<<(const rapidjson::Value &v) {
      do {
        if (!v.IsObject())
          return;
        if (v.HasMember("enable") && v["enable"].IsBool())
          enable = v["enable"].GetBool();
        if (v.HasMember("credentials") && v["credentials"].IsBool())
          credentials = v["credentials"].GetBool();
        if (v.HasMember("url") && v["url"].IsString()) {
          url = v["url"].GetString();
          if (credentials) {
            *this << url;
          }
        }
      } while (0);
    }
    inline void operator>>(rapidjson::Document &doc) const {
      if (!doc.IsObject())
        return;
      rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();
      rapidjson::Value obj(rapidjson::Type::kObjectType);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("enable"), enable, allocator);
      RAPIDJSON_ADDMEMBER_STRING(obj, std::string("url"), url, allocator);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("credentials"), credentials,
                               allocator);
      doc.AddMember("proxy", obj, allocator);
    }
    inline void operator<<(const std::string &authString) {
      ParseProxyCredentialsUrl(authString, type, scheme, port, username,
                               password, host);
    }
  };
  struct Jss {
  public:
    inline Jss();
    inline ~Jss();

    bool enable = false;
    std::string frameImplDidClearWindowObject;

    inline void operator>>(rapidjson::Document &doc) const {
      if (!doc.IsObject())
        return;
      rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();
      rapidjson::Value obj(rapidjson::Type::kObjectType);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("enable"), enable, allocator);
      RAPIDJSON_ADDMEMBER_STRING(obj,
                                 std::string("frameImplDidClearWindowObject"),
                                 frameImplDidClearWindowObject, allocator);
      doc.AddMember("jss", obj, allocator);
    }
    inline void operator<<(const rapidjson::Value &value) {
      do {
        if (!value.IsObject())
          break;
        if (value.HasMember("enable") && value["enable"].IsBool()) {
          enable = value["enable"].GetBool();
        }
        if (value.HasMember("frameImplDidClearWindowObject") &&
            value["frameImplDidClearWindowObject"].IsString() &&
            value["frameImplDidClearWindowObject"].GetStringLength() > 0) {
          frameImplDidClearWindowObject =
              value["frameImplDidClearWindowObject"].GetString();
        }
      } while (0);
    }
  };
  struct Startup {
    inline Startup();
    inline ~Startup();

    bool enable = false;
    bool enable_cleanup_udd = false;
    std::string homepage;
    std::string search_engine;
    unsigned int startup_pid = 0;
    unsigned int search_engine_id = 0;
    std::vector<std::string> urls;
    std::vector<std::string> args;
    inline void operator>>(rapidjson::Document &doc) const {
      if (!doc.IsObject())
        return;
      rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();
      rapidjson::Value obj(rapidjson::Type::kObjectType);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("enable"), enable, allocator);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("enable_cleanup_udd"),
                               enable_cleanup_udd, allocator);
      RAPIDJSON_ADDMEMBER_STRING(obj, std::string("homepage"), homepage,
                                 allocator);
      RAPIDJSON_ADDMEMBER_STRING(obj, std::string("search_engine"),
                                 search_engine, allocator);
      RAPIDJSON_ADDMEMBER_UINT(obj, std::string("search_engine_id"),
                               search_engine_id, allocator);
      RAPIDJSON_ADDMEMBER_UINT(obj, std::string("startup_pid"), startup_pid,
                               allocator);
      rapidjson::Value urlsArray(rapidjson::Type::kArrayType);
      for (const auto &url : urls) {
        if (url.empty())
          continue;
        rapidjson::Value urlValue(rapidjson::Type::kStringType);
        urlValue.SetString(url.c_str(),
                           static_cast<rapidjson::SizeType>(url.size()),
                           allocator);
        urlsArray.PushBack(urlValue, allocator);
      }
      RAPIDJSON_ADDMEMBER_OBJECT(obj, std::string("urls"), urlsArray,
                                 allocator);
      rapidjson::Value argsArray(rapidjson::Type::kArrayType);
      for (const auto &arg : args) {
        if (arg.empty())
          continue;
        rapidjson::Value argValue(rapidjson::Type::kStringType);
        argValue.SetString(arg.c_str(),
                           static_cast<rapidjson::SizeType>(arg.size()),
                           allocator);
        argsArray.PushBack(argValue, allocator);
      }
      RAPIDJSON_ADDMEMBER_OBJECT(obj, std::string("args"), argsArray,
                                 allocator);
      doc.AddMember("startup", obj, allocator);
    }
    inline void operator<<(const rapidjson::Value &value) {
      enable = false;
      homepage.clear();
      search_engine.clear();
      args.clear();
      urls.clear();
      search_engine_id = 0;
      do {
        if (!value.IsObject())
          break;
        if (value.HasMember("args") && value["args"].IsArray()) {
          const rapidjson::Value &refObj = value["args"];
          for (rapidjson::SizeType i = 0; i < refObj.Size(); ++i) {
            if (!refObj[i].IsString())
              break;
            args.emplace_back(refObj[i].GetString());
          }
        }
        if (value.HasMember("enable") && value["enable"].IsBool()) {
          enable = value["enable"].GetBool();
        }
        if (value.HasMember("startup_pid") && value["startup_pid"].IsUint()) {
          startup_pid = value["startup_pid"].GetUint();
        }
        if (value.HasMember("enable_cleanup_udd") &&
            value["enable_cleanup_udd"].IsBool()) {
          enable_cleanup_udd = value["enable_cleanup_udd"].GetBool();
        }
        if (value.HasMember("homepage") && value["homepage"].IsString()) {
          homepage = value["homepage"].GetString();
        }
        if (value.HasMember("search_engine") &&
            value["search_engine"].IsString()) {
          search_engine = value["search_engine"].GetString();
        }
        if (value.HasMember("search_engine_id") &&
            value["search_engine_id"].IsUint()) {
          search_engine_id = value["search_engine_id"].GetUint();
        }
        if (value.HasMember("urls") && value["urls"].IsArray()) {
          const rapidjson::Value &refObj = value["urls"];
          for (rapidjson::SizeType i = 0; i < refObj.Size(); ++i) {
            if (!refObj[i].IsString())
              break;
            urls.emplace_back(refObj[i].GetString());
          }
        }
      } while (0);
    }
  };
  struct Cookies {
    inline Cookies();
    inline ~Cookies();

    bool enable = false;
    std::set<std::string> blocklist;
    inline void operator>>(rapidjson::Document &doc) const {
      if (!doc.IsObject())
        return;
      rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();
      rapidjson::Value obj(rapidjson::Type::kObjectType);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("enable"), enable, allocator);
      rapidjson::Value blocklistArray(rapidjson::Type::kArrayType);
      for (const auto &item : blocklist) {
        if (item.empty())
          continue;
        rapidjson::Value itemValue(rapidjson::Type::kStringType);
        itemValue.SetString(item.c_str(),
                            static_cast<rapidjson::SizeType>(item.size()),
                            allocator);
        blocklistArray.PushBack(itemValue, allocator);
      }
      RAPIDJSON_ADDMEMBER_OBJECT(obj, std::string("blocklist"), blocklistArray,
                                 allocator);
      doc.AddMember("cookies", obj, allocator);
    }
    inline void operator<<(const rapidjson::Value &value) {
      blocklist.clear();
      enable = false;
      do {
        if (!value.IsObject())
          break;
        if (value.HasMember("enable") && value["enable"].IsBool())
          enable = value["enable"].GetBool();
        if (value.HasMember("blocklist") && value["blocklist"].IsArray()) {
          const rapidjson::Value &refObj = value["blocklist"];
          for (rapidjson::SizeType i = 0; i < refObj.Size(); ++i) {
            if (!refObj[i].IsString())
              break;
            std::string tmp(refObj[i].GetString());
            if (tmp.empty())
              continue;
            std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
            blocklist.emplace(tmp);
          }
        }
      } while (0);
    }
  };
  struct Storage {
    inline Storage();
    inline ~Storage();

    bool enable = false;
    std::set<std::string> blocklist;
    inline void operator>>(rapidjson::Document &doc) const {
      if (!doc.IsObject())
        return;
      rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();
      rapidjson::Value obj(rapidjson::Type::kObjectType);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("enable"), enable, allocator);
      rapidjson::Value blocklistArray(rapidjson::Type::kArrayType);
      for (const auto &item : blocklist) {
        if (item.empty())
          continue;
        rapidjson::Value itemValue(rapidjson::Type::kStringType);
        itemValue.SetString(item.c_str(),
                            static_cast<rapidjson::SizeType>(item.size()),
                            allocator);
        blocklistArray.PushBack(itemValue, allocator);
      }
      RAPIDJSON_ADDMEMBER_OBJECT(obj, std::string("blocklist"), blocklistArray,
                                 allocator);
      doc.AddMember("storage", obj, allocator);
    }
    inline void operator<<(const rapidjson::Value &value) {
      blocklist.clear();
      enable = false;
      do {
        if (!value.IsObject())
          break;
        if (value.HasMember("enable") && value["enable"].IsBool())
          enable = value["enable"].GetBool();
        if (value.HasMember("blocklist") && value["blocklist"].IsArray()) {
          const rapidjson::Value &refObj = value["blocklist"];
          for (rapidjson::SizeType i = 0; i < refObj.Size(); ++i) {
            std::string tmp(refObj[i].GetString());
            if (tmp.empty())
              continue;
            std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
            blocklist.emplace(tmp);
          }
        }
      } while (0);
    }
  };

  struct WebRTCIPHandling {
    inline WebRTCIPHandling();
    inline ~WebRTCIPHandling();

    WebRTCIPHandlingPolicy policy =
        WebRTCIPHandlingPolicy::kDefaultPublicInterfaceOnly;

    std::string public_ip;   //!@ Public IP, can be empty
    std::string internal_ip; //!@ Internal network IP
    int port = 0;            //!@ Port, default is 0
    inline void operator>>(rapidjson::Document &doc) const {
      if (!doc.IsObject())
        return;
      rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();
      rapidjson::Value obj(rapidjson::Type::kObjectType);
      std::string policyStr;
      switch (policy) {
      case WebRTCIPHandlingPolicy::kDefault:
        policyStr = kWebRTCIPHandlingDefault;
        break;
      case WebRTCIPHandlingPolicy::kDefaultPublicAndPrivateInterfaces:
        policyStr = kWebRTCIPHandlingDefaultPublicAndPrivateInterfaces;
        break;
      case WebRTCIPHandlingPolicy::kDefaultPublicInterfaceOnly:
        policyStr = kWebRTCIPHandlingDefaultPublicInterfaceOnly;
        break;
      case WebRTCIPHandlingPolicy::kDisableNonProxiedUdp:
        policyStr = kWebRTCIPHandlingDisableNonProxiedUdp;
        break;
      }
      RAPIDJSON_ADDMEMBER_STRING(obj, std::string("policy"), policyStr,
                                 allocator);
      RAPIDJSON_ADDMEMBER_INT(obj, std::string("port"), port, allocator);
      RAPIDJSON_ADDMEMBER_STRING(obj, std::string("public_ip"), public_ip,
                                 allocator);
      RAPIDJSON_ADDMEMBER_STRING(obj, std::string("internal_ip"), internal_ip,
                                 allocator);
      doc.AddMember("webrtcIPHandling", obj, allocator);
    }
    inline void operator<<(const rapidjson::Value &value) {
      do {
        if (!value.IsObject())
          break;
        if (value.HasMember("policy") && value["policy"].IsString()) {
          std::string policyStr = value["policy"].GetString();
          if (policyStr == kWebRTCIPHandlingDefault) {
            policy = WebRTCIPHandlingPolicy::kDefault;
          } else if (policyStr ==
                     kWebRTCIPHandlingDefaultPublicAndPrivateInterfaces) {
            policy = WebRTCIPHandlingPolicy::kDefaultPublicAndPrivateInterfaces;
          } else if (policyStr == kWebRTCIPHandlingDefaultPublicInterfaceOnly) {
            policy = WebRTCIPHandlingPolicy::kDefaultPublicInterfaceOnly;
          } else if (policyStr == kWebRTCIPHandlingDisableNonProxiedUdp) {
            policy = WebRTCIPHandlingPolicy::kDisableNonProxiedUdp;
          }
        }
        if (value.HasMember("port") && value["port"].IsInt()) {
          port = value["port"].GetInt();
        }
        if (value.HasMember("public_ip") && value["public_ip"].IsString()) {
          public_ip = value["public_ip"].GetString();
        }
        if (value.HasMember("internal_ip") && value["internal_ip"].IsString()) {
          internal_ip = value["internal_ip"].GetString();
        }
      } while (0);
    }
  };

  struct Fingerprint {
    struct Hash {
      inline Hash();
      inline ~Hash();

      bool random = false;
      double base = 0.0;
      double from = 0.0;
      double to = 0.0;

      inline void operator>>(rapidjson::Document &doc) const {
        if (!doc.IsObject())
          return;
        auto &allocator = doc.GetAllocator();
        rapidjson::Value obj(rapidjson::Type::kObjectType);
        RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("random"), random, allocator);
        RAPIDJSON_ADDMEMBER_DOUBLE(obj, std::string("base"), base, allocator);
        RAPIDJSON_ADDMEMBER_DOUBLE(obj, std::string("from"), from, allocator);
        RAPIDJSON_ADDMEMBER_DOUBLE(obj, std::string("to"), to, allocator);
        doc.AddMember("hash", obj, allocator);
      }
      inline void operator<<(const rapidjson::Value &v) {
        do {
          if (!v.IsObject())
            break;
          if (v.HasMember("random") && v["random"].IsBool()) {
            random = v["random"].GetBool();
          }
          if (v.HasMember("base")) {
            if (v["base"].IsFloat()) {
              base = static_cast<double>(v["base"].GetFloat());
            } else if (v["base"].IsDouble()) {
              base = v["base"].GetDouble();
            } else if (v["base"].IsInt()) {
              base = static_cast<double>(v["base"].GetInt() * 1.0);
            } else if (v["base"].IsUint()) {
              base = static_cast<double>(v["base"].GetUint() * 1.0);
            } else if (v["base"].IsInt64()) {
              base = static_cast<double>(v["base"].GetInt64() * 1.0);
            } else if (v["base"].IsUint64()) {
              base = static_cast<double>(v["base"].GetUint64() * 1.0);
            }
          }
          if (v.HasMember("from")) {
            if (v["from"].IsFloat()) {
              from = static_cast<double>(v["from"].GetFloat());
            } else if (v["from"].IsDouble()) {
              from = v["from"].GetDouble();
            } else if (v["from"].IsInt()) {
              from = static_cast<double>(v["from"].GetInt() * 1.0);
            } else if (v["from"].IsUint()) {
              from = static_cast<double>(v["from"].GetUint() * 1.0);
            } else if (v["from"].IsInt64()) {
              from = static_cast<double>(v["from"].GetInt64() * 1.0);
            } else if (v["from"].IsUint64()) {
              from = static_cast<double>(v["from"].GetUint64() * 1.0);
            }
          }
          if (v.HasMember("to")) {
            if (v["to"].IsFloat()) {
              to = static_cast<double>(v["to"].GetFloat());
            } else if (v["to"].IsDouble()) {
              to = v["to"].GetDouble();
            } else if (v["to"].IsInt()) {
              to = static_cast<double>(v["to"].GetInt() * 1.0);
            } else if (v["to"].IsUint()) {
              to = static_cast<double>(v["to"].GetUint() * 1.0);
            } else if (v["to"].IsInt64()) {
              to = static_cast<double>(v["to"].GetInt64() * 1.0);
            } else if (v["to"].IsUint64()) {
              to = static_cast<double>(v["to"].GetUint64() * 1.0);
            }
          }
        } while (0);
      }
    };
    struct Screen {
      struct Dpi {
        inline Dpi();
        inline ~Dpi();

        int x = 96; //!@ Default DPI for X axis
        int y = 96; //!@ Default DPI for Y axis

        void operator=(const Dpi &other) {
          x = other.x;
          y = other.y;
        }
        void operator>>(rapidjson::Document &doc) const {
          if (!doc.IsObject())
            return;
          rapidjson::Document tmpDoc(rapidjson::Type::kObjectType);
          auto &allocHash = tmpDoc.GetAllocator();
          RAPIDJSON_ADDMEMBER_INT(tmpDoc, std::string("x"), x, allocHash);
          RAPIDJSON_ADDMEMBER_INT(tmpDoc, std::string("y"), y, allocHash);
          rapidjson::Value tmpValue(tmpDoc, doc.GetAllocator());
          doc.AddMember(rapidjson::Value("dpi", doc.GetAllocator()).Move(),
                        tmpValue, doc.GetAllocator());
        }
        void operator<<(const rapidjson::Value &v) {
          if (!v.IsObject())
            return;
          if (v.HasMember("x") && v["x"].IsInt()) {
            x = v["x"].GetInt();
          }
          if (v.HasMember("y") && v["y"].IsInt()) {
            y = v["y"].GetInt();
          }
        }
      };
      inline Screen();
      inline ~Screen();

      int height = 0;
      int width = 0;
      int colorDepth = 0;
      int pixelDepth = 0;
      int availLeft = 0;
      int availTop = 0;
      int availHeight = 0;
      int availWidth = 0;
      double devicePixelRatio = 1.0;
      Dpi dpi; //!@ Default DPI for screen, usually 96x96
      bool enable = false;

      void operator=(const Screen &other) {
        height = other.height;
        width = other.width;
        colorDepth = other.colorDepth;
        pixelDepth = other.pixelDepth;
        availLeft = other.availLeft;
        availTop = other.availTop;
        availHeight = other.availHeight;
        availWidth = other.availWidth;
        devicePixelRatio = other.devicePixelRatio;
        dpi = other.dpi;
        enable = other.enable;
      }
      void operator>>(rapidjson::Document &doc) const {
        if (!doc.IsObject())
          return;
        rapidjson::Document tmpDoc(rapidjson::Type::kObjectType);
        auto &allocHash = tmpDoc.GetAllocator();
        RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("enable"), enable,
                                 allocHash);
        RAPIDJSON_ADDMEMBER_INT(tmpDoc, std::string("height"), height,
                                allocHash);
        RAPIDJSON_ADDMEMBER_INT(tmpDoc, std::string("width"), width, allocHash);
        RAPIDJSON_ADDMEMBER_INT(tmpDoc, std::string("colorDepth"), colorDepth,
                                allocHash);
        RAPIDJSON_ADDMEMBER_INT(tmpDoc, std::string("pixelDepth"), pixelDepth,
                                allocHash);
        RAPIDJSON_ADDMEMBER_INT(tmpDoc, std::string("availLeft"), availLeft,
                                allocHash);
        RAPIDJSON_ADDMEMBER_INT(tmpDoc, std::string("availTop"), availTop,
                                allocHash);
        RAPIDJSON_ADDMEMBER_INT(tmpDoc, std::string("availHeight"), availHeight,
                                allocHash);
        RAPIDJSON_ADDMEMBER_INT(tmpDoc, std::string("availWidth"), availWidth,
                                allocHash);
        RAPIDJSON_ADDMEMBER_DOUBLE(tmpDoc, std::string("devicePixelRatio"),
                                   devicePixelRatio, allocHash);
        dpi >> tmpDoc;
        rapidjson::Value tmpValue(tmpDoc, doc.GetAllocator());
        doc.AddMember(rapidjson::Value("screen", doc.GetAllocator()).Move(),
                      tmpValue, doc.GetAllocator());
      }
      void operator<<(const rapidjson::Value &v) {
        if (!v.IsObject())
          return;
        if (v.HasMember("height") && v["height"].IsInt()) {
          height = v["height"].GetInt();
        }
        if (v.HasMember("width") && v["width"].IsInt()) {
          width = v["width"].GetInt();
        }
        if (v.HasMember("colorDepth") && v["colorDepth"].IsInt()) {
          colorDepth = v["colorDepth"].GetInt();
        }
        if (v.HasMember("pixelDepth") && v["pixelDepth"].IsInt()) {
          pixelDepth = v["pixelDepth"].GetInt();
        }
        if (v.HasMember("availLeft") && v["availLeft"].IsInt()) {
          availLeft = v["availLeft"].GetInt();
        }
        if (v.HasMember("availTop") && v["availTop"].IsInt()) {
          availTop = v["availTop"].GetInt();
        }
        if (v.HasMember("availHeight") && v["availHeight"].IsInt()) {
          availHeight = v["availHeight"].GetInt();
        }
        if (v.HasMember("availWidth") && v["availWidth"].IsInt()) {
          availWidth = v["availWidth"].GetInt();
        }
        if (v.HasMember("devicePixelRatio") &&
            v["devicePixelRatio"].IsDouble()) {
          devicePixelRatio = v["devicePixelRatio"].GetDouble();
        }
        if (v.HasMember("enable") && v["enable"].IsBool()) {
          enable = v["enable"].GetBool();
        }
        if (v.HasMember("dpi") && v["dpi"].IsObject()) {
          dpi << v["dpi"];
        }
      }
    };
    struct V8 {
      inline V8();
      inline ~V8();

      bool enable = false;
      Hash hash;

      inline void operator>>(rapidjson::Document &doc) const {
        if (!doc.IsObject())
          return;
        rapidjson::Document tmpDoc(rapidjson::Type::kObjectType);
        auto &allocHash = tmpDoc.GetAllocator();
        RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("enable"), enable,
                                 allocHash);
        hash >> tmpDoc;

        rapidjson::Value tmpValue(tmpDoc, doc.GetAllocator());
        doc.AddMember(rapidjson::Value("v8", doc.GetAllocator()).Move(),
                      tmpValue, doc.GetAllocator());
      }
      inline void operator<<(const rapidjson::Value &v) {
        if (!v.IsObject())
          return;
        if (v.HasMember("enable") && v["enable"].IsBool()) {
          enable = v["enable"].GetBool();
        }
        hash << v["hash"];
      }
    };
    struct Tlspeet {
      inline Tlspeet();
      inline ~Tlspeet();

      //!@ TLS cipher suites
      //!@ TLS extensions
      //!@ TLS curves
      unsigned short grease_seed = 0xAA;
      //!@ Akamai HTTP/2 fingerprint
      bool enable_http2_settings_grease = false;
      //!@ JA3 fingerprint
      bool permute_extensions = false;

      bool enable = false;

      void operator=(const Tlspeet &other) {
        grease_seed = other.grease_seed;
        enable_http2_settings_grease = other.enable_http2_settings_grease;
        permute_extensions = other.permute_extensions;
        enable = other.enable;
      }
      void operator>>(rapidjson::Document &doc) const {
        if (!doc.IsObject())
          return;
        rapidjson::Document tmpDoc(rapidjson::Type::kObjectType);
        auto &allocHash = tmpDoc.GetAllocator();
        RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("enable"), enable,
                                 allocHash);
        RAPIDJSON_ADDMEMBER_UINT(tmpDoc, std::string("grease_seed"),
                                 grease_seed, allocHash);
        RAPIDJSON_ADDMEMBER_BOOL(tmpDoc,
                                 std::string("enable_http2_settings_grease"),
                                 enable_http2_settings_grease, allocHash);
        RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("permute_extensions"),
                                 permute_extensions, allocHash);

        rapidjson::Value tmpValue(tmpDoc, doc.GetAllocator());
        doc.AddMember(rapidjson::Value("tlspeet", doc.GetAllocator()).Move(),
                      tmpValue, doc.GetAllocator());
      }
      void operator<<(const rapidjson::Value &v) {
        if (!v.IsObject())
          return;
        if (v.HasMember("enable") && v["enable"].IsBool()) {
          enable = v["enable"].GetBool();
        }
        if (v.HasMember("grease_seed") && v["grease_seed"].IsUint()) {
          grease_seed = static_cast<unsigned short>(v["grease_seed"].GetUint());
        }
        if (v.HasMember("enable_http2_settings_grease") &&
            v["enable_http2_settings_grease"].IsBool()) {
          enable_http2_settings_grease =
              v["enable_http2_settings_grease"].GetBool();
        }
        if (v.HasMember("permute_extensions") &&
            v["permute_extensions"].IsBool()) {
          permute_extensions = v["permute_extensions"].GetBool();
        }
      }
    };
    struct Math {
      inline Math();
      inline ~Math();

      bool enable = false;
      Hash hash;
      void operator>>(rapidjson::Document &doc) const {
        if (!doc.IsObject())
          return;
        rapidjson::Document tmpDoc(rapidjson::Type::kObjectType);
        auto &allocHash = tmpDoc.GetAllocator();
        RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("enable"), enable,
                                 allocHash);
        hash >> tmpDoc;

        rapidjson::Value tmpValue(tmpDoc, doc.GetAllocator());
        doc.AddMember(rapidjson::Value("math", doc.GetAllocator()).Move(),
                      tmpValue, doc.GetAllocator());
      }
      void operator<<(const rapidjson::Value &v) {
        if (!v.IsObject())
          return;
        if (v.HasMember("enable") && v["enable"].IsBool()) {
          enable = v["enable"].GetBool();
        }
        hash << v["hash"];
      }
    };
    struct Canvas {
      inline Canvas();
      inline ~Canvas();

      bool enable = false;
      Hash hash;
      inline void operator>>(rapidjson::Document &doc) const {
        if (!doc.IsObject())
          return;
        rapidjson::Document tmpDoc(rapidjson::Type::kObjectType);
        auto &allocHash = tmpDoc.GetAllocator();
        RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("enable"), enable,
                                 allocHash);
        hash >> tmpDoc;
        rapidjson::Value tmpValue(tmpDoc, doc.GetAllocator());
        doc.AddMember(rapidjson::Value("canvas", doc.GetAllocator()).Move(),
                      tmpValue, doc.GetAllocator());
      }
      inline void operator<<(const rapidjson::Value &v) {
        if (!v.IsObject())
          return;
        if (v.HasMember("enable") && v["enable"].IsBool()) {
          enable = v["enable"].GetBool();
        }
        hash << v["hash"];
      }
    };
    struct Audio {
      inline Audio();
      inline ~Audio();

      bool enable = false;
      Hash hash;
      inline void operator>>(rapidjson::Document &doc) const {
        if (!doc.IsObject())
          return;
        rapidjson::Document tmpDoc(rapidjson::Type::kObjectType);
        auto &allocHash = tmpDoc.GetAllocator();
        RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("enable"), enable,
                                 allocHash);
        hash >> tmpDoc;

        rapidjson::Value tmpValue(tmpDoc, doc.GetAllocator());
        doc.AddMember(rapidjson::Value("audio", doc.GetAllocator()).Move(),
                      tmpValue, doc.GetAllocator());
      }
      inline void operator<<(const rapidjson::Value &v) {
        if (!v.IsObject())
          return;
        if (v.HasMember("enable") && v["enable"].IsBool()) {
          enable = v["enable"].GetBool();
        }
        hash << v["hash"];
      }
    };
    struct Webgl {
      struct ContextAttributes {
        inline ContextAttributes();
        inline ~ContextAttributes();
        bool enable = false;
        bool alpha = true;
        bool antialias = true;
        bool depth = true;
        bool desynchronized = false;
        bool failIfMajorPerformanceCaveat = false;
        std::string powerPreference =
            "default"; //"default", "low-power", "high-performance"
        bool premultipliedAlpha = true;
        bool preserveDrawingBuffer = false;
        bool stencil = false;
        bool xrCompatible = false;
        inline void operator=(const ContextAttributes &other) {
          enable = other.enable;
          alpha = other.alpha;
          antialias = other.antialias;
          depth = other.depth;
          desynchronized = other.desynchronized;
          failIfMajorPerformanceCaveat = other.failIfMajorPerformanceCaveat;
          powerPreference = other.powerPreference;
          premultipliedAlpha = other.premultipliedAlpha;
          preserveDrawingBuffer = other.preserveDrawingBuffer;
          stencil = other.stencil;
          xrCompatible = other.xrCompatible;
        }
        inline void operator>>(rapidjson::Document &doc) const {
          if (!doc.IsObject())
            return;
          rapidjson::Document tmpDoc(rapidjson::Type::kObjectType);
          auto &allocHash = tmpDoc.GetAllocator();
          RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("alpha"), alpha,
                                   allocHash);
          RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("antialias"), antialias,
                                   allocHash);
          RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("depth"), depth,
                                   allocHash);
          RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("desynchronized"),
                                   desynchronized, allocHash);
          RAPIDJSON_ADDMEMBER_BOOL(tmpDoc,
                                   std::string("failIfMajorPerformanceCaveat"),
                                   failIfMajorPerformanceCaveat, allocHash);
          RAPIDJSON_ADDMEMBER_STRING(tmpDoc, std::string("powerPreference"),
                                     powerPreference, allocHash);
          RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("premultipliedAlpha"),
                                   premultipliedAlpha, allocHash);
          RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("preserveDrawingBuffer"),
                                   preserveDrawingBuffer, allocHash);
          RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("stencil"), stencil,
                                   allocHash);
          RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("xrCompatible"),
                                   xrCompatible, allocHash);
          RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("enable"), enable,
                                   allocHash);
          rapidjson::Value tmpValue(tmpDoc, doc.GetAllocator());
          doc.AddMember(
              rapidjson::Value("contextAttributes", doc.GetAllocator()).Move(),
              tmpValue, doc.GetAllocator());
        }
        inline void operator<<(const std::string &str) {
          rapidjson::Document doc(rapidjson::Type::kObjectType);
          if (doc.Parse(str.data(), str.size()).HasParseError())
            return;
          operator<<(doc);
        }
        inline void operator<<(const rapidjson::Value &v) {
          if (!v.IsObject())
            return;
          if (v.HasMember("enable") && v["enable"].IsBool()) {
            enable = v["enable"].GetBool();
          }
          if (v.HasMember("alpha") && v["alpha"].IsBool()) {
            alpha = v["alpha"].GetBool();
          }
          if (v.HasMember("antialias") && v["antialias"].IsBool()) {
            antialias = v["antialias"].GetBool();
          }
          if (v.HasMember("depth") && v["depth"].IsBool()) {
            depth = v["depth"].GetBool();
          }
          if (v.HasMember("desynchronized") && v["desynchronized"].IsBool()) {
            desynchronized = v["desynchronized"].GetBool();
          }
          if (v.HasMember("failIfMajorPerformanceCaveat") &&
              v["failIfMajorPerformanceCaveat"].IsBool()) {
            failIfMajorPerformanceCaveat =
                v["failIfMajorPerformanceCaveat"].GetBool();
          }
          if (v.HasMember("powerPreference") &&
              v["powerPreference"].IsString()) {
            powerPreference = v["powerPreference"].GetString();
          }
          if (v.HasMember("premultipliedAlpha") &&
              v["premultipliedAlpha"].IsBool()) {
            premultipliedAlpha = v["premultipliedAlpha"].GetBool();
          }
          if (v.HasMember("preserveDrawingBuffer") &&
              v["preserveDrawingBuffer"].IsBool()) {
            preserveDrawingBuffer = v["preserveDrawingBuffer"].GetBool();
          }
          if (v.HasMember("stencil") && v["stencil"].IsBool()) {
            stencil = v["stencil"].GetBool();
          }
          if (v.HasMember("xrCompatible") && v["xrCompatible"].IsBool()) {
            xrCompatible = v["xrCompatible"].GetBool();
          }
        }
      };
      struct ShaderPrecisionFormat {
        struct _8DF_ {
          inline _8DF_();
          inline ~_8DF_();
          int rangeMin = 0;
          int rangeMax = 0;
          int precision = 0;
          void operator=(const _8DF_ &other) {
            rangeMin = other.rangeMin;
            rangeMax = other.rangeMax;
            precision = other.precision;
          }
        };
        struct _8B30_ {
          inline _8B30_();
          inline ~_8B30_();
          _8DF_ _8DF0;
          _8DF_ _8DF1;
          _8DF_ _8DF2;
          _8DF_ _8DF3;
          _8DF_ _8DF4;
          _8DF_ _8DF5;
          void operator=(const _8B30_ &other) {
            _8DF0 = other._8DF0;
            _8DF1 = other._8DF1;
            _8DF2 = other._8DF2;
            _8DF3 = other._8DF3;
            _8DF4 = other._8DF4;
            _8DF5 = other._8DF5;
          }
          void operator>>(rapidjson::Document &doc) const {
            if (!doc.IsObject())
              return;
            rapidjson::Document tmpDoc(rapidjson::Type::kObjectType);
            auto &allocHash = tmpDoc.GetAllocator();
            rapidjson::Value _8DF0Obj(rapidjson::Type::kObjectType);
            RAPIDJSON_ADDMEMBER_INT(_8DF0Obj, std::string("rangeMin"),
                                    _8DF0.rangeMin, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF0Obj, std::string("rangeMax"),
                                    _8DF0.rangeMax, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF0Obj, std::string("precision"),
                                    _8DF0.precision, allocHash);
            tmpDoc.AddMember(rapidjson::Value("8DF0", allocHash).Move(),
                             _8DF0Obj, allocHash);
            rapidjson::Value _8DF1Obj(rapidjson::Type::kObjectType);
            RAPIDJSON_ADDMEMBER_INT(_8DF1Obj, std::string("rangeMin"),
                                    _8DF1.rangeMin, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF1Obj, std::string("rangeMax"),
                                    _8DF1.rangeMax, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF1Obj, std::string("precision"),
                                    _8DF1.precision, allocHash);
            tmpDoc.AddMember(rapidjson::Value("8DF1", allocHash).Move(),
                             _8DF1Obj, allocHash);
            rapidjson::Value _8DF2obj(rapidjson::Type::kObjectType);
            RAPIDJSON_ADDMEMBER_INT(_8DF2obj, std::string("rangeMin"),
                                    _8DF2.rangeMin, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF2obj, std::string("rangeMax"),
                                    _8DF2.rangeMax, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF2obj, std::string("precision"),
                                    _8DF2.precision, allocHash);
            tmpDoc.AddMember(rapidjson::Value("8DF2", allocHash).Move(),
                             _8DF2obj, allocHash);
            rapidjson::Value _8DF3Obj(rapidjson::Type::kObjectType);
            RAPIDJSON_ADDMEMBER_INT(_8DF3Obj, std::string("rangeMin"),
                                    _8DF3.rangeMin, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF3Obj, std::string("rangeMax"),
                                    _8DF3.rangeMax, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF3Obj, std::string("precision"),
                                    _8DF3.precision, allocHash);
            tmpDoc.AddMember(rapidjson::Value("8DF3", allocHash).Move(),
                             _8DF3Obj, allocHash);
            rapidjson::Value _8DF4obj(rapidjson::Type::kObjectType);
            RAPIDJSON_ADDMEMBER_INT(_8DF4obj, std::string("rangeMin"),
                                    _8DF4.rangeMin, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF4obj, std::string("rangeMax"),
                                    _8DF4.rangeMax, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF4obj, std::string("precision"),
                                    _8DF4.precision, allocHash);
            tmpDoc.AddMember(rapidjson::Value("8DF4", allocHash).Move(),
                             _8DF4obj, allocHash);
            rapidjson::Value _8DF5obj(rapidjson::Type::kObjectType);
            RAPIDJSON_ADDMEMBER_INT(_8DF5obj, std::string("rangeMin"),
                                    _8DF5.rangeMin, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF5obj, std::string("rangeMax"),
                                    _8DF5.rangeMax, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF5obj, std::string("precision"),
                                    _8DF5.precision, allocHash);
            tmpDoc.AddMember(rapidjson::Value("8DF5", allocHash).Move(),
                             _8DF5obj, allocHash);

            rapidjson::Value tmpValue(tmpDoc, doc.GetAllocator());
            doc.AddMember(rapidjson::Value("8B30", doc.GetAllocator()).Move(),
                          tmpValue, doc.GetAllocator());
          }
          void operator<<(const std::string &str) {
            rapidjson::Document doc(rapidjson::Type::kObjectType);
            if (doc.Parse(str.data(), str.size()).HasParseError())
              return;
            operator<<(doc);
          }
          void operator<<(const rapidjson::Value &v) {
            do {
              if (!v.IsObject())
                break;
              if (v.HasMember("8DF0") && v["8DF0"].IsObject()) {
                const rapidjson::Value &refObj = v["8DF0"];
                if (refObj.HasMember("rangeMin") &&
                    refObj["rangeMin"].IsInt()) {
                  _8DF0.rangeMin = refObj["rangeMin"].GetInt();
                }
                if (refObj.HasMember("rangeMax") &&
                    refObj["rangeMax"].IsInt()) {
                  _8DF0.rangeMax = refObj["rangeMax"].GetInt();
                }
                if (refObj.HasMember("precision") &&
                    refObj["precision"].IsInt()) {
                  _8DF0.precision = refObj["precision"].GetInt();
                }
              }
              if (v.HasMember("8DF1") && v["8DF1"].IsObject()) {
                const rapidjson::Value &refObj = v["8DF1"];
                if (refObj.HasMember("rangeMin") &&
                    refObj["rangeMin"].IsInt()) {
                  _8DF1.rangeMin = refObj["rangeMin"].GetInt();
                }
                if (refObj.HasMember("rangeMax") &&
                    refObj["rangeMax"].IsInt()) {
                  _8DF1.rangeMax = refObj["rangeMax"].GetInt();
                }
                if (refObj.HasMember("precision") &&
                    refObj["precision"].IsInt()) {
                  _8DF1.precision = refObj["precision"].GetInt();
                }
              }
              if (v.HasMember("8DF2") && v["8DF2"].IsObject()) {
                const rapidjson::Value &refObj = v["8DF2"];
                if (refObj.HasMember("rangeMin") &&
                    refObj["rangeMin"].IsInt()) {
                  _8DF2.rangeMin = refObj["rangeMin"].GetInt();
                }
                if (refObj.HasMember("rangeMax") &&
                    refObj["rangeMax"].IsInt()) {
                  _8DF2.rangeMax = refObj["rangeMax"].GetInt();
                }
                if (refObj.HasMember("precision") &&
                    refObj["precision"].IsInt()) {
                  _8DF2.precision = refObj["precision"].GetInt();
                }
              }
              if (v.HasMember("8DF3") && v["8DF3"].IsObject()) {
                const rapidjson::Value &refObj = v["8DF3"];
                if (refObj.HasMember("rangeMin") &&
                    refObj["rangeMin"].IsInt()) {
                  _8DF3.rangeMin = refObj["rangeMin"].GetInt();
                }
                if (refObj.HasMember("rangeMax") &&
                    refObj["rangeMax"].IsInt()) {
                  _8DF3.rangeMax = refObj["rangeMax"].GetInt();
                }
                if (refObj.HasMember("precision") &&
                    refObj["precision"].IsInt()) {
                  _8DF3.precision = refObj["precision"].GetInt();
                }
              }
              if (v.HasMember("8DF4") && v["8DF4"].IsObject()) {
                const rapidjson::Value &refObj = v["8DF4"];
                if (refObj.HasMember("rangeMin") &&
                    refObj["rangeMin"].IsInt()) {
                  _8DF4.rangeMin = refObj["rangeMin"].GetInt();
                }
                if (refObj.HasMember("rangeMax") &&
                    refObj["rangeMax"].IsInt()) {
                  _8DF4.rangeMax = refObj["rangeMax"].GetInt();
                }
                if (refObj.HasMember("precision") &&
                    refObj["precision"].IsInt()) {
                  _8DF4.precision = refObj["precision"].GetInt();
                }
              }
              if (v.HasMember("8DF5") && v["8DF5"].IsObject()) {
                const rapidjson::Value &refObj = v["8DF5"];
                if (refObj.HasMember("rangeMin") &&
                    refObj["rangeMin"].IsInt()) {
                  _8DF5.rangeMin = refObj["rangeMin"].GetInt();
                }
                if (refObj.HasMember("rangeMax") &&
                    refObj["rangeMax"].IsInt()) {
                  _8DF5.rangeMax = refObj["rangeMax"].GetInt();
                }
                if (refObj.HasMember("precision") &&
                    refObj["precision"].IsInt()) {
                  _8DF5.precision = refObj["precision"].GetInt();
                }
              }
            } while (0);
          }
        };
        struct _8B31_ {
          inline _8B31_();
          inline ~_8B31_();
          _8DF_ _8DF0;
          _8DF_ _8DF1;
          _8DF_ _8DF2;
          _8DF_ _8DF3;
          _8DF_ _8DF4;
          _8DF_ _8DF5;
          void operator=(const _8B31_ &other) {
            _8DF0 = other._8DF0;
            _8DF1 = other._8DF1;
            _8DF2 = other._8DF2;
            _8DF3 = other._8DF3;
            _8DF4 = other._8DF4;
            _8DF5 = other._8DF5;
          }
          void operator>>(rapidjson::Document &doc) const {
            if (!doc.IsObject())
              return;
            rapidjson::Document tmpDoc(rapidjson::Type::kObjectType);
            auto &allocHash = tmpDoc.GetAllocator();
            rapidjson::Value _8DF0Obj(rapidjson::Type::kObjectType);
            RAPIDJSON_ADDMEMBER_INT(_8DF0Obj, std::string("rangeMin"),
                                    _8DF0.rangeMin, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF0Obj, std::string("rangeMax"),
                                    _8DF0.rangeMax, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF0Obj, std::string("precision"),
                                    _8DF0.precision, allocHash);
            tmpDoc.AddMember(rapidjson::Value("8DF0", allocHash).Move(),
                             _8DF0Obj, allocHash);
            rapidjson::Value _8DF1Obj(rapidjson::Type::kObjectType);
            RAPIDJSON_ADDMEMBER_INT(_8DF1Obj, std::string("rangeMin"),
                                    _8DF1.rangeMin, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF1Obj, std::string("rangeMax"),
                                    _8DF1.rangeMax, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF1Obj, std::string("precision"),
                                    _8DF1.precision, allocHash);
            tmpDoc.AddMember(rapidjson::Value("8DF1", allocHash).Move(),
                             _8DF1Obj, allocHash);
            rapidjson::Value _8DF2obj(rapidjson::Type::kObjectType);
            RAPIDJSON_ADDMEMBER_INT(_8DF2obj, std::string("rangeMin"),
                                    _8DF2.rangeMin, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF2obj, std::string("rangeMax"),
                                    _8DF2.rangeMax, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF2obj, std::string("precision"),
                                    _8DF2.precision, allocHash);
            tmpDoc.AddMember(rapidjson::Value("8DF2", allocHash).Move(),
                             _8DF2obj, allocHash);
            rapidjson::Value _8DF3Obj(rapidjson::Type::kObjectType);
            RAPIDJSON_ADDMEMBER_INT(_8DF3Obj, std::string("rangeMin"),
                                    _8DF3.rangeMin, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF3Obj, std::string("rangeMax"),
                                    _8DF3.rangeMax, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF3Obj, std::string("precision"),
                                    _8DF3.precision, allocHash);
            tmpDoc.AddMember(rapidjson::Value("8DF3", allocHash).Move(),
                             _8DF3Obj, allocHash);
            rapidjson::Value _8DF4obj(rapidjson::Type::kObjectType);
            RAPIDJSON_ADDMEMBER_INT(_8DF4obj, std::string("rangeMin"),
                                    _8DF4.rangeMin, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF4obj, std::string("rangeMax"),
                                    _8DF4.rangeMax, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF4obj, std::string("precision"),
                                    _8DF4.precision, allocHash);
            tmpDoc.AddMember(rapidjson::Value("8DF4", allocHash).Move(),
                             _8DF4obj, allocHash);
            rapidjson::Value _8DF5obj(rapidjson::Type::kObjectType);
            RAPIDJSON_ADDMEMBER_INT(_8DF5obj, std::string("rangeMin"),
                                    _8DF5.rangeMin, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF5obj, std::string("rangeMax"),
                                    _8DF5.rangeMax, allocHash);
            RAPIDJSON_ADDMEMBER_INT(_8DF5obj, std::string("precision"),
                                    _8DF5.precision, allocHash);
            tmpDoc.AddMember(rapidjson::Value("8DF5", allocHash).Move(),
                             _8DF5obj, allocHash);
            rapidjson::Value tmpValue(tmpDoc, doc.GetAllocator());
            doc.AddMember(rapidjson::Value("8B31", doc.GetAllocator()).Move(),
                          tmpValue, doc.GetAllocator());
          }
          void operator<<(const rapidjson::Value &v) {
            do {
              if (!v.IsObject())
                break;
              if (v.HasMember("8DF0") && v["8DF0"].IsObject()) {
                const rapidjson::Value &refObj = v["8DF0"];
                if (refObj.HasMember("rangeMin") &&
                    refObj["rangeMin"].IsInt()) {
                  _8DF0.rangeMin = refObj["rangeMin"].GetInt();
                }
                if (refObj.HasMember("rangeMax") &&
                    refObj["rangeMax"].IsInt()) {
                  _8DF0.rangeMax = refObj["rangeMax"].GetInt();
                }
                if (refObj.HasMember("precision") &&
                    refObj["precision"].IsInt()) {
                  _8DF0.precision = refObj["precision"].GetInt();
                }
              }
              if (v.HasMember("8DF1") && v["8DF1"].IsObject()) {
                const rapidjson::Value &refObj = v["8DF1"];
                if (refObj.HasMember("rangeMin") &&
                    refObj["rangeMin"].IsInt()) {
                  _8DF1.rangeMin = refObj["rangeMin"].GetInt();
                }
                if (refObj.HasMember("rangeMax") &&
                    refObj["rangeMax"].IsInt()) {
                  _8DF1.rangeMax = refObj["rangeMax"].GetInt();
                }
                if (refObj.HasMember("precision") &&
                    refObj["precision"].IsInt()) {
                  _8DF1.precision = refObj["precision"].GetInt();
                }
              }
              if (v.HasMember("8DF2") && v["8DF2"].IsObject()) {
                const rapidjson::Value &refObj = v["8DF2"];
                if (refObj.HasMember("rangeMin") &&
                    refObj["rangeMin"].IsInt()) {
                  _8DF2.rangeMin = refObj["rangeMin"].GetInt();
                }
                if (refObj.HasMember("rangeMax") &&
                    refObj["rangeMax"].IsInt()) {
                  _8DF2.rangeMax = refObj["rangeMax"].GetInt();
                }
                if (refObj.HasMember("precision") &&
                    refObj["precision"].IsInt()) {
                  _8DF2.precision = refObj["precision"].GetInt();
                }
              }
              if (v.HasMember("8DF3") && v["8DF3"].IsObject()) {
                const rapidjson::Value &refObj = v["8DF3"];
                if (refObj.HasMember("rangeMin") &&
                    refObj["rangeMin"].IsInt()) {
                  _8DF3.rangeMin = refObj["rangeMin"].GetInt();
                }
                if (refObj.HasMember("rangeMax") &&
                    refObj["rangeMax"].IsInt()) {
                  _8DF3.rangeMax = refObj["rangeMax"].GetInt();
                }
                if (refObj.HasMember("precision") &&
                    refObj["precision"].IsInt()) {
                  _8DF3.precision = refObj["precision"].GetInt();
                }
              }
              if (v.HasMember("8DF4") && v["8DF4"].IsObject()) {
                const rapidjson::Value &refObj = v["8DF4"];
                if (refObj.HasMember("rangeMin") &&
                    refObj["rangeMin"].IsInt()) {
                  _8DF4.rangeMin = refObj["rangeMin"].GetInt();
                }
                if (refObj.HasMember("rangeMax") &&
                    refObj["rangeMax"].IsInt()) {
                  _8DF4.rangeMax = refObj["rangeMax"].GetInt();
                }
                if (refObj.HasMember("precision") &&
                    refObj["precision"].IsInt()) {
                  _8DF4.precision = refObj["precision"].GetInt();
                }
              }
              if (v.HasMember("8DF5") && v["8DF5"].IsObject()) {
                const rapidjson::Value &refObj = v["8DF5"];
                if (refObj.HasMember("rangeMin") &&
                    refObj["rangeMin"].IsInt()) {
                  _8DF5.rangeMin = refObj["rangeMin"].GetInt();
                }
                if (refObj.HasMember("rangeMax") &&
                    refObj["rangeMax"].IsInt()) {
                  _8DF5.rangeMax = refObj["rangeMax"].GetInt();
                }
                if (refObj.HasMember("precision") &&
                    refObj["precision"].IsInt()) {
                  _8DF5.precision = refObj["precision"].GetInt();
                }
              }
            } while (0);
          }
        };

        inline ShaderPrecisionFormat();
        inline ~ShaderPrecisionFormat();

        bool enable = false;
        _8B30_ _8B30;
        _8B31_ _8B31;

        void operator=(const ShaderPrecisionFormat &other) {
          enable = other.enable;
          _8B30 = other._8B30;
          _8B31 = other._8B31;
        }
        void operator>>(rapidjson::Document &doc) const {
          if (!doc.IsObject())
            return;
          rapidjson::Document tmpDoc(rapidjson::Type::kObjectType);

          _8B30 >> tmpDoc;
          _8B31 >> tmpDoc;
          auto &allocHash = tmpDoc.GetAllocator();
          RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("enable"), enable,
                                   allocHash);
          rapidjson::Value tmpValue(tmpDoc, doc.GetAllocator());
          doc.AddMember(
              rapidjson::Value("shaderPrecisionFormat", doc.GetAllocator())
                  .Move(),
              tmpValue, doc.GetAllocator());
        }
        void operator<<(const rapidjson::Value &value) {
          do {
            if (!value.IsObject())
              break;
            if (value.HasMember("enable") && value["enable"].IsBool()) {
              enable = value["enable"].GetBool();
            }
            if (!value.HasMember("8B30") || !value["8B30"].IsObject() ||
                !value.HasMember("8B31") || !value["8B31"].IsObject()) {
              break;
            }
            _8B30 << value["8B30"];
            _8B31 << value["8B31"];
          } while (0);
        }
      };
      inline Webgl();
      inline ~Webgl();

      bool enable = false;
      Hash hash;
      ContextAttributes contextAttributes;
      ShaderPrecisionFormat shaderPrecisionFormat;
      std::map<int, std::string> getParameter;
      inline void operator>>(rapidjson::Document &doc) const {
        if (!doc.IsObject())
          return;
        rapidjson::Document tmpDoc(rapidjson::Type::kObjectType);
        auto &allocHash = tmpDoc.GetAllocator();
        RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("enable"), enable,
                                 allocHash);
        hash >> tmpDoc;

        rapidjson::Value getParameterObj(rapidjson::Type::kObjectType);
        for (const auto &item : getParameter) {
          if (item.second.empty())
            continue;
          rapidjson::Value keyValue(rapidjson::Type::kStringType);
          keyValue.SetString(std::to_string(item.first).c_str(),
                             static_cast<rapidjson::SizeType>(
                                 std::to_string(item.first).size()),
                             allocHash);
          rapidjson::Value valueValue(rapidjson::Type::kStringType);
          valueValue.SetString(
              item.second.c_str(),
              static_cast<rapidjson::SizeType>(item.second.size()), allocHash);
          getParameterObj.AddMember(keyValue, valueValue, allocHash);
        }
        RAPIDJSON_ADDMEMBER_OBJECT(tmpDoc, std::string("getParameter"),
                                   getParameterObj, allocHash);
        contextAttributes >> tmpDoc;
        shaderPrecisionFormat >> tmpDoc;
        rapidjson::Value tmpValue(tmpDoc, doc.GetAllocator());
        doc.AddMember(rapidjson::Value("webgl", doc.GetAllocator()).Move(),
                      tmpValue, doc.GetAllocator());
      }
      inline void operator<<(const rapidjson::Value &v) {
        if (!v.IsObject())
          return;
        if (v.HasMember("enable") && v["enable"].IsBool()) {
          enable = v["enable"].GetBool();
        }
        if (v.HasMember("hash") && v["hash"].IsObject()) {
          hash << v["hash"];
        }
        getParameter.clear();
        if (v.HasMember("getParameter") && v["getParameter"].IsObject()) {
          const rapidjson::Value &refObj = v["getParameter"];
          for (auto itr = refObj.MemberBegin(); itr != refObj.MemberEnd();
               ++itr) {
            if (!itr->value.IsString())
              continue;
            if (itr->name.GetStringLength() <= 0)
              continue;
            int key = strtol(itr->name.GetString(), nullptr, 10);
            std::string value = itr->value.GetString();
            getParameter[key] = value;
          }
        }
        if (v.HasMember("contextAttributes") &&
            v["contextAttributes"].IsObject()) {
          contextAttributes << v["contextAttributes"];
        }
        if (v.HasMember("shaderPrecisionFormat") &&
            v["shaderPrecisionFormat"].IsObject()) {
          shaderPrecisionFormat << v["shaderPrecisionFormat"];
        }
      }
    };

    struct Disk {
      inline Disk();
      inline ~Disk();

      bool enable = false;
      Hash hash;
      int64_t quota = 0;
      int64_t usage = 0;
      inline void operator>>(rapidjson::Document &doc) const {
        if (!doc.IsObject())
          return;
        rapidjson::Document tmpDoc(rapidjson::Type::kObjectType);
        auto &allocHash = tmpDoc.GetAllocator();
        RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("enable"), enable,
                                 allocHash);
        hash >> tmpDoc;
        RAPIDJSON_ADDMEMBER_INT64(tmpDoc, std::string("quota"), quota,
                                  allocHash);
        RAPIDJSON_ADDMEMBER_INT64(tmpDoc, std::string("usage"), usage,
                                  allocHash);
        rapidjson::Value tmpValue(tmpDoc, doc.GetAllocator());
        doc.AddMember(rapidjson::Value("disk", doc.GetAllocator()).Move(),
                      tmpValue, doc.GetAllocator());
      }
      inline void operator<<(const rapidjson::Value &v) {
        if (!v.IsObject())
          return;
        if (v.HasMember("enable") && v["enable"].IsBool()) {
          enable = v["enable"].GetBool();
        }
        if (v.HasMember("hash") && v["hash"].IsObject()) {
          hash << v["hash"];
        }
        if (v.HasMember("quota") && v["quota"].IsInt64()) {
          quota = v["quota"].GetInt64();
        }
        if (v.HasMember("usage") && v["usage"].IsInt64()) {
          usage = v["usage"].GetInt64();
        }
      }
    };
    struct Voice {
      struct Data {
        inline Data();
        inline ~Data();

        std::string lang;
        std::string name;
      };
      inline Voice();
      inline ~Voice();

      bool enable = false;
      std::vector<Data> datas;
      void operator>>(rapidjson::Document &doc) const {
        if (!doc.IsObject())
          return;
        rapidjson::Document tmpDoc(rapidjson::Type::kObjectType);
        auto &allocHash = tmpDoc.GetAllocator();
        RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("enable"), enable,
                                 allocHash);
        rapidjson::Value datasArray(rapidjson::Type::kArrayType);
        for (const auto &item : datas) {
          rapidjson::Value itemObj(rapidjson::Type::kObjectType);
          RAPIDJSON_ADDMEMBER_STRING(itemObj, std::string("lang"), item.lang,
                                     allocHash);
          RAPIDJSON_ADDMEMBER_STRING(itemObj, std::string("name"), item.name,
                                     allocHash);
          datasArray.PushBack(itemObj, allocHash);
        }
        RAPIDJSON_ADDMEMBER_OBJECT(tmpDoc, std::string("datas"), datasArray,
                                   allocHash);
        rapidjson::Value tmpValue(tmpDoc, doc.GetAllocator());
        doc.AddMember(rapidjson::Value("voice", doc.GetAllocator()).Move(),
                      tmpValue, doc.GetAllocator());
      }
      void operator<<(const rapidjson::Value &v) {
        if (!v.IsObject())
          return;
        if (v.HasMember("enable") && v["enable"].IsBool()) {
          enable = v["enable"].GetBool();
        }
        datas.clear();
        if (v.HasMember("datas") && v["datas"].IsArray()) {
          const rapidjson::Value &refObj = v["datas"];
          for (rapidjson::SizeType i = 0; i < refObj.Size(); ++i) {
            if (!refObj[i].IsObject())
              break;
            Data item;
            if (refObj[i].HasMember("lang") && refObj[i]["lang"].IsString()) {
              item.lang = refObj[i]["lang"].GetString();
            }
            if (refObj[i].HasMember("name") && refObj[i]["name"].IsString()) {
              item.name = refObj[i]["name"].GetString();
            }
            datas.emplace_back(item);
          }
        }
      }
    };
    struct UserAgentMetadata {
      struct Brand_version {
        inline Brand_version();
        inline ~Brand_version();
        std::string brand;
        std::string version;
        void operator=(const Brand_version &other) {
          brand = other.brand;
          version = other.version;
        }
      };
      struct Brand_full_version {
        inline Brand_full_version();
        inline ~Brand_full_version();
        std::string brand;
        std::string version;
        void operator=(const Brand_full_version &other) {
          brand = other.brand;
          version = other.version;
        }
      };
      inline UserAgentMetadata();
      inline ~UserAgentMetadata();

      bool enable = false;
      std::vector<Brand_version> brand_version_list;
      std::vector<Brand_full_version> brand_full_version_list;
      std::string full_version = "138.0.7204.158";
      std::string platform = "Windows";
      std::string platform_version = "11.0.0";
      std::string architecture = "x86_64";
      std::string model = "";
      bool mobile = false;
      std::string bitness = "64";
      bool wow64 = false;
      std::vector<std::string> form_factors = {"Desktop"};

      void operator=(const UserAgentMetadata &other) {
        form_factors.clear();
        brand_version_list.clear();
        brand_full_version_list.clear();
        enable = other.enable;
        for (auto &item : other.brand_version_list) {
          brand_version_list.emplace_back(item);
        }
        for (auto &item : other.brand_full_version_list) {
          brand_full_version_list.emplace_back(item);
        }
        full_version = other.full_version;
        platform = other.platform;
        platform_version = other.platform_version;
        architecture = other.architecture;
        model = other.model;
        mobile = other.mobile;
        bitness = other.bitness;
        wow64 = other.wow64;
        for (auto &item : other.form_factors) {
          form_factors.emplace_back(item);
        }
      }
      void operator<<(const rapidjson::Value &value) {
        do {
          if (!value.IsObject())
            break;
          if (value.HasMember("enable") && value["enable"].IsBool()) {
            enable = value["enable"].GetBool();
          }
          brand_version_list.clear();
          if (value.HasMember("brand_version_list") &&
              value["brand_version_list"].IsArray()) {
            const rapidjson::Value &refObj = value["brand_version_list"];
            for (rapidjson::SizeType i = 0; i < refObj.Size(); ++i) {
              if (!refObj[i].IsObject())
                break;
              if (!refObj[i].HasMember("brand") ||
                  !refObj[i]["brand"].IsString() ||
                  !refObj[i].HasMember("version") ||
                  !refObj[i]["version"].IsString())
                break;
              Brand_version item;
              item.brand = refObj[i]["brand"].GetString();
              item.version = refObj[i]["version"].GetString();
              brand_version_list.emplace_back(item);
            }
          }
          brand_full_version_list.clear();
          if (value.HasMember("brand_full_version_list") &&
              value["brand_full_version_list"].IsArray()) {
            const rapidjson::Value &refObj = value["brand_full_version_list"];
            for (rapidjson::SizeType i = 0; i < refObj.Size(); ++i) {
              if (!refObj[i].IsObject())
                break;
              if (!refObj[i].HasMember("brand") ||
                  !refObj[i]["brand"].IsString() ||
                  !refObj[i].HasMember("version") ||
                  !refObj[i]["version"].IsString())
                break;
              Brand_full_version item;
              item.brand = refObj[i]["brand"].GetString();
              item.version = refObj[i]["version"].GetString();
              brand_full_version_list.emplace_back(item);
            }
          }
          if (value.HasMember("full_version") &&
              value["full_version"].IsString()) {
            full_version = value["full_version"].GetString();
          }
          if (value.HasMember("platform") && value["platform"].IsString()) {
            platform = value["platform"].GetString();
          }
          if (value.HasMember("platform_version") &&
              value["platform_version"].IsString()) {
            platform_version = value["platform_version"].GetString();
          }
          if (value.HasMember("architecture") &&
              value["architecture"].IsString()) {
            architecture = value["architecture"].GetString();
          }
          if (value.HasMember("model") && value["model"].IsString()) {
            model = value["model"].GetString();
          }
          if (value.HasMember("mobile") && value["mobile"].IsBool()) {
            mobile = value["mobile"].GetBool();
          }
          if (value.HasMember("bitness") && value["bitness"].IsString()) {
            bitness = value["bitness"].GetString();
          }
          if (value.HasMember("wow64") && value["wow64"].IsBool()) {
            wow64 = value["wow64"].GetBool();
          }
          form_factors.clear();
          if (value.HasMember("form_factors") &&
              value["form_factors"].IsArray()) {
            const rapidjson::Value &refObj = value["form_factors"];
            for (rapidjson::SizeType i = 0; i < refObj.Size(); ++i) {
              if (!refObj[i].IsString())
                break;
              std::string tmp(refObj[i].GetString());
              if (tmp.empty())
                continue;
              // std::transform(tmp.begin(), tmp.end(), tmp.begin(),
              // ::tolower);
              form_factors.emplace_back(tmp);
            }
          }
        } while (0);
      }
      void operator>>(rapidjson::Document &output) const {
        if (!output.IsObject())
          return;
        rapidjson::Document tmpDoc(rapidjson::Type::kObjectType);
        auto &allocHash = tmpDoc.GetAllocator();
        RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("enable"), enable,
                                 allocHash);

        rapidjson::Value brandVersionArray(rapidjson::Type::kArrayType);
        for (const auto &item : brand_version_list) {
          rapidjson::Value itemObj(rapidjson::Type::kObjectType);
          RAPIDJSON_ADDMEMBER_STRING(itemObj, std::string("brand"), item.brand,
                                     allocHash);
          RAPIDJSON_ADDMEMBER_STRING(itemObj, std::string("version"),
                                     item.version, allocHash);
          brandVersionArray.PushBack(itemObj, allocHash);
        }
        RAPIDJSON_ADDMEMBER_OBJECT(tmpDoc, std::string("brand_version_list"),
                                   brandVersionArray, allocHash);

        rapidjson::Value brandFullVersionArray(rapidjson::Type::kArrayType);
        for (const auto &item : brand_full_version_list) {
          rapidjson::Value itemObj(rapidjson::Type::kObjectType);
          RAPIDJSON_ADDMEMBER_STRING(itemObj, std::string("brand"), item.brand,
                                     allocHash);
          RAPIDJSON_ADDMEMBER_STRING(itemObj, std::string("version"),
                                     item.version, allocHash);
          brandFullVersionArray.PushBack(itemObj, allocHash);
        }
        RAPIDJSON_ADDMEMBER_OBJECT(tmpDoc,
                                   std::string("brand_full_version_list"),
                                   brandFullVersionArray, allocHash);

        RAPIDJSON_ADDMEMBER_STRING(tmpDoc, std::string("full_version"),
                                   full_version, allocHash);
        RAPIDJSON_ADDMEMBER_STRING(tmpDoc, std::string("platform"), platform,
                                   allocHash);
        RAPIDJSON_ADDMEMBER_STRING(tmpDoc, std::string("platform_version"),
                                   platform_version, allocHash);
        RAPIDJSON_ADDMEMBER_STRING(tmpDoc, std::string("architecture"),
                                   architecture, allocHash);
        RAPIDJSON_ADDMEMBER_STRING(tmpDoc, std::string("model"), model,
                                   allocHash);
        RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("mobile"), mobile,
                                 allocHash);
        RAPIDJSON_ADDMEMBER_STRING(tmpDoc, std::string("bitness"), bitness,
                                   allocHash);
        RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("wow64"), wow64,
                                 allocHash);
        rapidjson::Value formFactorsArray(rapidjson::Type::kArrayType);
        for (const auto &item : form_factors) {
          if (item.empty())
            continue;
          rapidjson::Value itemValue(rapidjson::Type::kStringType);
          itemValue.SetString(item.c_str(),
                              static_cast<rapidjson::SizeType>(item.size()),
                              allocHash);
          formFactorsArray.PushBack(itemValue, allocHash);
        }
        RAPIDJSON_ADDMEMBER_OBJECT(tmpDoc, std::string("form_factors"),
                                   formFactorsArray, allocHash);
        rapidjson::Value tmpValue(tmpDoc, output.GetAllocator());
        output.AddMember(
            rapidjson::Value("userAgentMetadata", output.GetAllocator()).Move(),
            tmpValue, output.GetAllocator());
      }
    };
    struct CdmRegistry {
      struct CdmEntry final {
        inline CdmEntry();
        inline ~CdmEntry();
        std::string key_system;
        std::string mime_type;
        std::string robustness;
        void operator=(const CdmEntry &edmEntry) {
          key_system = edmEntry.key_system;
          mime_type = edmEntry.mime_type;
          robustness = edmEntry.robustness;
        }
        void operator<<(const rapidjson::Value &v) {
          if (!v.IsObject())
            return;
          if (v.HasMember("key_system") && v["key_system"].IsString()) {
            key_system = v["key_system"].GetString();
          }
          if (v.HasMember("mime_type") && v["mime_type"].IsString()) {
            mime_type = v["mime_type"].GetString();
          }
          if (v.HasMember("robustness") && v["robustness"].IsString()) {
            robustness = v["robustness"].GetString();
          }
        }
        void operator>>(rapidjson::Document &v) const {
          v.SetObject();
          auto &alloc = v.GetAllocator();
          RAPIDJSON_ADDMEMBER_STRING(v, std::string("key_system"), key_system,
                                     alloc);
          RAPIDJSON_ADDMEMBER_STRING(v, std::string("mime_type"), mime_type,
                                     alloc);
          RAPIDJSON_ADDMEMBER_STRING(v, std::string("robustness"), robustness,
                                     alloc);
        }
      };
      inline CdmRegistry();
      inline ~CdmRegistry();
      bool enable = false;
      Hash hash;
      std::vector<CdmEntry> allowlist;
      inline void operator=(const CdmRegistry &other) {
        enable = other.enable;
        hash = other.hash;
        allowlist.clear();
        for (const auto &item : other.allowlist) {
          allowlist.emplace_back(item);
        }
      }
      inline void operator<<(const rapidjson::Value &v) {
        if (!v.IsObject())
          return;
        if (v.HasMember("enable") && v["enable"].IsBool()) {
          enable = v["enable"].GetBool();
        }
        hash << v["hash"];
        allowlist.clear();
        if (v.HasMember("allowlist") && v["allowlist"].IsArray()) {
          const rapidjson::Value &refObj = v["allowlist"];
          for (rapidjson::SizeType i = 0; i < refObj.Size(); ++i) {
            if (!refObj[i].IsObject())
              break;
            CdmEntry item;
            item << refObj[i];
            allowlist.emplace_back(item);
          }
        }
      }
      inline void operator>>(rapidjson::Document &doc) const {
        if (!doc.IsObject())
          return;
        rapidjson::Document tmpDoc(rapidjson::Type::kObjectType);
        auto &alloc = tmpDoc.GetAllocator();
        RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("enable"), enable, alloc);
        hash >> tmpDoc;

        rapidjson::Value allowlistArray(rapidjson::Type::kArrayType);
        for (const auto &item : allowlist) {
          rapidjson::Document itemObj(rapidjson::Type::kObjectType);
          item >> itemObj;
          rapidjson::Value copiedItemObj;
          copiedItemObj.CopyFrom(itemObj, alloc);
          allowlistArray.PushBack(copiedItemObj, alloc);
        }
        RAPIDJSON_ADDMEMBER_OBJECT(tmpDoc, std::string("allowlist"),
                                   allowlistArray, alloc);

        rapidjson::Value tmpValue(tmpDoc, doc.GetAllocator());
        doc.AddMember(
            rapidjson::Value("cdmRegistry", doc.GetAllocator()).Move(),
            tmpValue, doc.GetAllocator());
      }
    };
    struct Font {
      inline Font();
      inline ~Font();

      bool enable = false;
      Hash hash;
      std::set<std::string> allowlist;
      inline void operator>>(rapidjson::Document &doc) const {
        if (!doc.IsObject())
          return;
        rapidjson::Document tmpDoc(rapidjson::Type::kObjectType);
        auto &allocHash = tmpDoc.GetAllocator();
        RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("enable"), enable,
                                 allocHash);
        hash >> tmpDoc;

        rapidjson::Value allowlistArray(rapidjson::Type::kArrayType);
        for (const auto &item : allowlist) {
          if (item.empty())
            continue;
          rapidjson::Value itemValue(rapidjson::Type::kStringType);
          itemValue.SetString(item.c_str(),
                              static_cast<rapidjson::SizeType>(item.size()),
                              allocHash);
          allowlistArray.PushBack(itemValue, allocHash);
        }
        RAPIDJSON_ADDMEMBER_OBJECT(tmpDoc, std::string("allowlist"),
                                   allowlistArray, allocHash);

        rapidjson::Value tmpValue(tmpDoc, doc.GetAllocator());
        doc.AddMember(rapidjson::Value("font", doc.GetAllocator()).Move(),
                      tmpValue, doc.GetAllocator());
      }
      inline void operator<<(const rapidjson::Value &v) {
        if (!v.IsObject())
          return;
        if (v.HasMember("enable") && v["enable"].IsBool()) {
          enable = v["enable"].GetBool();
        }
        hash << v["hash"];
        allowlist.clear();
        if (v.HasMember("allowlist") && v["allowlist"].IsArray()) {
          const rapidjson::Value &refObj = v["allowlist"];
          for (rapidjson::SizeType i = 0; i < refObj.Size(); ++i) {
            if (!refObj[i].IsString())
              break;
            std::string tmp(refObj[i].GetString());
            if (tmp.empty())
              continue;
            std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
            allowlist.emplace(tmp);
          }
        }
      }
    };
    struct Product {
      inline Product();
      inline ~Product();
      std::string name = "Microsoft Edge";     //!@ Google Chrome
      std::string version = "138.0.3351.140";  //!@ Google Chrome version
      std::string chromium = "138.0.7204.158"; //!@ Google Chrome version

      void operator<<(const rapidjson::Value &v) {
        if (!v.IsObject())
          return;
        if (v.HasMember("name") && v["name"].IsString()) {
          name = v["name"].GetString();
        }
        if (v.HasMember("version") && v["version"].IsString()) {
          version = v["version"].GetString();
        }
        if (v.HasMember("chromium") && v["chromium"].IsString()) {
          chromium = v["chromium"].GetString();
        }
      }
      void operator>>(rapidjson::Document &doc) const {
        if (!doc.IsObject())
          return;
        rapidjson::Document tmpDoc(rapidjson::Type::kObjectType);
        auto &allocHash = tmpDoc.GetAllocator();
        RAPIDJSON_ADDMEMBER_STRING(tmpDoc, std::string("name"), name,
                                   allocHash);
        RAPIDJSON_ADDMEMBER_STRING(tmpDoc, std::string("version"), version,
                                   allocHash);
        RAPIDJSON_ADDMEMBER_STRING(tmpDoc, std::string("chromium"), chromium,
                                   allocHash);
        rapidjson::Value tmpValue(tmpDoc, doc.GetAllocator());
        doc.AddMember(rapidjson::Value("product", doc.GetAllocator()).Move(),
                      tmpValue, doc.GetAllocator());
      }
      void operator=(const Product &other) {
        name = other.name;
        version = other.version;
        chromium = other.chromium;
      }
    };
    inline Fingerprint();
    inline ~Fingerprint();

    bool enable = false;
    bool real_fingerprint_output = false;
    bool navigatorWebdriver =
        false; //!@ blink navigator.webdriver, default is false
    bool do_not_track = true;
    std::string platform = "Win32";
    unsigned int rtt = 0; //!@ Round-trip time in milliseconds, default is 0
    bool netDnsAllowFallbackToSystemtask =
        false; //!@ Allow fallback to system task for DNS resolution
    int hardwareConcurrency = 8;
    int deviceMemory = 16;
    std::string userAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
                            "AppleWebKit/537.36 (KHTML, "
                            "like Gecko) Chrome/138.0.0.0 Safari/537.36";
    std::vector<std::string> languages = {
        "en-US", "en"}; //!@ Language list, default is ["en-US", "en"]
    Product product;
    Canvas canvas;
    Webgl webgl;
    Audio audio;
    Math math;
    Tlspeet tlspeet;
    V8 v8;
    Font font;
    CdmRegistry cdmRegistry;
    Screen screen;
    Disk disk;
    UserAgentMetadata userAgentMetadata;
    Voice voice;
    inline void operator>>(rapidjson::Document &doc) const {
      if (!doc.IsObject())
        return;
      rapidjson::Document fpsDoc(rapidjson::kObjectType);
      auto &fpsAllocator = fpsDoc.GetAllocator();
      RAPIDJSON_ADDMEMBER_BOOL(fpsDoc, std::string("enable"), enable,
                               fpsAllocator);
      RAPIDJSON_ADDMEMBER_BOOL(fpsDoc, std::string("real_fingerprint_output"),
                               real_fingerprint_output, fpsAllocator);
      RAPIDJSON_ADDMEMBER_BOOL(fpsDoc, std::string("navigatorWebdriver"),
                               navigatorWebdriver, fpsAllocator);
      RAPIDJSON_ADDMEMBER_BOOL(fpsDoc, std::string("do_not_track"),
                               do_not_track, fpsAllocator);
      RAPIDJSON_ADDMEMBER_STRING(fpsDoc, std::string("platform"), platform,
                                 fpsAllocator);
      RAPIDJSON_ADDMEMBER_INT(fpsDoc, std::string("hardwareConcurrency"),
                              hardwareConcurrency, fpsAllocator);
      RAPIDJSON_ADDMEMBER_INT(fpsDoc, std::string("deviceMemory"), deviceMemory,
                              fpsAllocator);
      RAPIDJSON_ADDMEMBER_STRING(fpsDoc, std::string("userAgent"), userAgent,
                                 fpsAllocator);
      RAPIDJSON_ADDMEMBER_UINT(fpsDoc, std::string("rtt"), rtt, fpsAllocator);
      RAPIDJSON_ADDMEMBER_BOOL(fpsDoc,
                               std::string("netDnsAllowFallbackToSystemtask"),
                               netDnsAllowFallbackToSystemtask, fpsAllocator);
      rapidjson::Value languagesArray(rapidjson::Type::kArrayType);
      for (const auto &lang : languages) {
        if (lang.empty())
          continue;
        rapidjson::Value langValue(rapidjson::Type::kStringType);
        langValue.SetString(lang.c_str(),
                            static_cast<rapidjson::SizeType>(lang.size()),
                            fpsAllocator);
        languagesArray.PushBack(langValue, fpsAllocator);
      }
      RAPIDJSON_ADDMEMBER_OBJECT(fpsDoc, std::string("languages"),
                                 languagesArray, fpsAllocator);
      product >> fpsDoc;
      userAgentMetadata >> fpsDoc;
      voice >> fpsDoc;
      canvas >> fpsDoc;
      webgl >> fpsDoc;
      audio >> fpsDoc;
      math >> fpsDoc;
      v8 >> fpsDoc;
      font >> fpsDoc;
      screen >> fpsDoc;
      tlspeet >> fpsDoc;
      disk >> fpsDoc;
      cdmRegistry >> fpsDoc;
      rapidjson::Value fpsValue(fpsDoc, doc.GetAllocator());
      doc.AddMember(rapidjson::Value("fps", doc.GetAllocator()).Move(),
                    fpsValue, doc.GetAllocator());
    }
    inline void operator<<(const rapidjson::Value &v) {
      if (!v.IsObject())
        return;
      if (v.HasMember("enable") && v["enable"].IsBool()) {
        enable = v["enable"].GetBool();
      }
      if (v.HasMember("real_fingerprint_output") &&
          v["real_fingerprint_output"].IsBool()) {
        real_fingerprint_output = v["real_fingerprint_output"].GetBool();
      }
      if (v.HasMember("navigatorWebdriver") &&
          v["navigatorWebdriver"].IsBool()) {
        navigatorWebdriver = v["navigatorWebdriver"].GetBool();
      }
      if (v.HasMember("do_not_track") && v["do_not_track"].IsBool()) {
        do_not_track = v["do_not_track"].GetBool();
      }
      if (v.HasMember("rtt") && v["rtt"].IsUint()) {
        rtt = v["rtt"].GetUint();
      }
      if (v.HasMember("disk") && v["disk"].IsObject()) {
        disk << v["disk"];
      }
      if (v.HasMember("netDnsAllowFallbackToSystemtask") &&
          v["netDnsAllowFallbackToSystemtask"].IsBool()) {
        netDnsAllowFallbackToSystemtask =
            v["netDnsAllowFallbackToSystemtask"].GetBool();
      }
      if (v.HasMember("product") && v["product"].IsObject()) {
        product << v["product"];
      }
      if (v.HasMember("userAgentMetadata") &&
          v["userAgentMetadata"].IsObject()) {
        userAgentMetadata << v["userAgentMetadata"];
      }
      if (v.HasMember("voice") && v["voice"].IsObject()) {
        voice << v["voice"];
      }
      if (v.HasMember("cdmRegistry") && v["cdmRegistry"].IsObject()) {
        cdmRegistry << v["cdmRegistry"];
      }
      if (v.HasMember("platform") && v["platform"].IsString()) {
        platform = v["platform"].GetString();
      }
      if (v.HasMember("hardwareConcurrency") &&
          v["hardwareConcurrency"].IsInt()) {
        hardwareConcurrency = v["hardwareConcurrency"].GetInt();
      }
      if (v.HasMember("deviceMemory") && v["deviceMemory"].IsInt()) {
        deviceMemory = v["deviceMemory"].GetInt();
      }
      if (v.HasMember("userAgent") && v["userAgent"].IsString()) {
        userAgent = v["userAgent"].GetString();
      }
      if (v.HasMember("screen") && v["screen"].IsObject()) {
        screen << v["screen"];
      }
      if (v.HasMember("tlspeet") && v["tlspeet"].IsObject()) {
        tlspeet << v["tlspeet"];
      }
      if (v.HasMember("languages") && v["languages"].IsArray()) {
        languages.clear();
        const rapidjson::Value &refObj = v["languages"];
        for (rapidjson::SizeType i = 0; i < refObj.Size(); ++i) {
          if (!refObj[i].IsString())
            break;
          std::string tmp(refObj[i].GetString());
          if (tmp.empty())
            continue;
          languages.emplace_back(tmp);
        }
      }
      if (v.HasMember("canvas") && v["canvas"].IsObject()) {
        canvas << v["canvas"];
      }
      if (v.HasMember("webgl") && v["webgl"].IsObject()) {
        webgl << v["webgl"];
      }
      if (v.HasMember("audio") && v["audio"].IsObject()) {
        audio << v["audio"];
      }
      if (v.HasMember("math") && v["math"].IsObject()) {
        math << v["math"];
      }
      if (v.HasMember("v8") && v["v8"].IsObject()) {
        v8 << v["v8"];
      }
      if (v.HasMember("font") && v["font"].IsObject()) {
        font << v["font"];
      }
    }
  };
  struct DynamicFpsInfo {
    struct Ipinfo {
      inline Ipinfo();
      inline ~Ipinfo();

      std::string ip;
      std::string country;
      std::string region;
      std::string city;
      std::string zip;
      std::string lat;
      std::string lon;
      std::string timezone;
      std::string isp;

      void operator>>(rapidjson::Document &doc) const {
        if (!doc.IsObject())
          return;
        rapidjson::Document tmpDoc(rapidjson::Type::kObjectType);
        auto &allocator = tmpDoc.GetAllocator();
        RAPIDJSON_ADDMEMBER_STRING(tmpDoc, std::string("ip"), ip, allocator);
        RAPIDJSON_ADDMEMBER_STRING(tmpDoc, std::string("country"), country,
                                   allocator);
        RAPIDJSON_ADDMEMBER_STRING(tmpDoc, std::string("region"), region,
                                   allocator);
        RAPIDJSON_ADDMEMBER_STRING(tmpDoc, std::string("city"), city,
                                   allocator);
        RAPIDJSON_ADDMEMBER_STRING(tmpDoc, std::string("zip"), zip, allocator);
        RAPIDJSON_ADDMEMBER_STRING(tmpDoc, std::string("lat"), lat, allocator);
        RAPIDJSON_ADDMEMBER_STRING(tmpDoc, std::string("lon"), lon, allocator);
        RAPIDJSON_ADDMEMBER_STRING(tmpDoc, std::string("timezone"), timezone,
                                   allocator);
        rapidjson::Value tmpValue(tmpDoc, doc.GetAllocator());
        doc.AddMember(rapidjson::Value("ipinfo", doc.GetAllocator()).Move(),
                      tmpValue, doc.GetAllocator());
      }
      void operator<<(const rapidjson::Value &v) {
        if (!v.IsObject())
          return;
        if (v.HasMember("ip") && v["ip"].IsString()) {
          ip = v["ip"].GetString();
        }
        if (v.HasMember("country") && v["country"].IsString()) {
          country = v["country"].GetString();
        }
        if (v.HasMember("region") && v["region"].IsString()) {
          region = v["region"].GetString();
        }
        if (v.HasMember("city") && v["city"].IsString()) {
          city = v["city"].GetString();
        }
        if (v.HasMember("zip") && v["zip"].IsString()) {
          zip = v["zip"].GetString();
        }
        if (v.HasMember("lat") && v["lat"].IsString()) {
          lat = v["lat"].GetString();
        }
        if (v.HasMember("lon") && v["lon"].IsString()) {
          lon = v["lon"].GetString();
        }
        if (v.HasMember("timezone") && v["timezone"].IsString()) {
          timezone = v["timezone"].GetString();
        }
      }
    };
    inline DynamicFpsInfo();
    inline ~DynamicFpsInfo();

    bool enable = false; //!@ Enable dynamic FPS, default is false
    Ipinfo ipinfo;       //!@ IP information, default is empty
    void operator>>(rapidjson::Document &doc) const {
      if (!doc.IsObject())
        return;
      rapidjson::Document tmpDoc(rapidjson::Type::kObjectType);
      auto &allocator = tmpDoc.GetAllocator();
      RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("enable"), enable,
                               allocator);
      ipinfo >> tmpDoc;
      rapidjson::Value tmpValue(tmpDoc, doc.GetAllocator());
      doc.AddMember(rapidjson::Value("dynFpsInfo", doc.GetAllocator()).Move(),
                    tmpValue, doc.GetAllocator());
    }
    void operator<<(const rapidjson::Value &v) {
      if (!v.IsObject())
        return;
      if (v.HasMember("enable") && v["enable"].IsBool()) {
        enable = v["enable"].GetBool();
      }
      if (v.HasMember("ipinfo") && v["ipinfo"].IsObject()) {
        ipinfo << v["ipinfo"];
      }
    }
  };
  struct Extensions {
    inline Extensions();
    inline ~Extensions();

    bool enable = false;
    std::set<std::string> allowlist;
    inline void operator>>(rapidjson::Document &doc) const {
      if (!doc.IsObject())
        return;
      rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();
      rapidjson::Value obj(rapidjson::Type::kObjectType);
      RAPIDJSON_ADDMEMBER_BOOL(obj, std::string("enable"), enable, allocator);
      rapidjson::Value allowlistArray(rapidjson::Type::kArrayType);
      for (const auto &item : allowlist) {
        if (item.empty())
          continue;
        rapidjson::Value itemValue(rapidjson::Type::kStringType);
        itemValue.SetString(item.c_str(),
                            static_cast<rapidjson::SizeType>(item.size()),
                            allocator);
        allowlistArray.PushBack(itemValue, allocator);
      }
      RAPIDJSON_ADDMEMBER_OBJECT(obj, std::string("allowlist"), allowlistArray,
                                 allocator);
      doc.AddMember("extensions", obj, allocator);
    }
    inline void operator<<(const rapidjson::Value &v) {
      allowlist.clear();
      enable = false;
      do {
        if (!v.IsObject())
          break;
        if (v.HasMember("enable") && v["enable"].IsBool()) {
          enable = v["enable"].GetBool();
        }
        if (v.HasMember("allowlist") && v["allowlist"].IsArray()) {
          const rapidjson::Value &refObj = v["allowlist"];
          for (rapidjson::SizeType i = 0; i < refObj.Size(); ++i) {
            if (!refObj[i].IsString())
              break;
            std::string tmp(refObj[i].GetString());
            if (tmp.empty())
              continue;
            std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
            allowlist.emplace(tmp);
          }
        }
      } while (0);
    }
  };

  struct Policy {
    inline Policy();
    inline ~Policy();

    bool enable = false;
    bool reuse = false;
    bool dynamic_proxy = false;
    int reuse_ip = 0;
    int reuse_failed_max = 3;
    int reuse_success_max = 10;
    int dynamic_proxy_type = 0;
    int dynamic_proxy_session_timeout = 10; // seconds

    void operator=(const Policy &other) {
      enable = other.enable;
      reuse = other.reuse;
      dynamic_proxy = other.dynamic_proxy;
      reuse_ip = other.reuse_ip;
      reuse_failed_max = other.reuse_failed_max;
      reuse_success_max = other.reuse_success_max;
      dynamic_proxy_type = other.dynamic_proxy_type;
      dynamic_proxy_session_timeout = other.dynamic_proxy_session_timeout;
    }
    void operator<<(const rapidjson::Value &v) {
      if (!v.IsObject())
        return;
      if (v.HasMember("enable") && v["enable"].IsBool()) {
        enable = v["enable"].GetBool();
      }
      if (v.HasMember("reuse") && v["reuse"].IsBool()) {
        reuse = v["reuse"].GetBool();
      }
      if (v.HasMember("dynamic_proxy") && v["dynamic_proxy"].IsBool()) {
        dynamic_proxy = v["dynamic_proxy"].GetBool();
      }
      if (v.HasMember("reuse_ip") && v["reuse_ip"].IsInt()) {
        reuse_ip = v["reuse_ip"].GetInt();
      }
      if (v.HasMember("reuse_failed_max") && v["reuse_failed_max"].IsInt()) {
        reuse_failed_max = v["reuse_failed_max"].GetInt();
      }
      if (v.HasMember("reuse_success_max") && v["reuse_success_max"].IsInt()) {
        reuse_success_max = v["reuse_success_max"].GetInt();
      }
      if (v.HasMember("dynamic_proxy_type") &&
          v["dynamic_proxy_type"].IsInt()) {
        dynamic_proxy_type = v["dynamic_proxy_type"].GetInt();
      }
      if (v.HasMember("dynamic_proxy_session_timeout") &&
          v["dynamic_proxy_session_timeout"].IsInt()) {
        dynamic_proxy_session_timeout =
            v["dynamic_proxy_session_timeout"].GetInt();
      }
    }
    void operator>>(rapidjson::Document &doc) const {
      if (!doc.IsObject())
        return;
      rapidjson::Document tmpDoc(rapidjson::Type::kObjectType);
      auto &allocHash = tmpDoc.GetAllocator();
      RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("enable"), enable,
                               allocHash);
      RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("reuse"), reuse, allocHash);
      RAPIDJSON_ADDMEMBER_BOOL(tmpDoc, std::string("dynamic_proxy"),
                               dynamic_proxy, allocHash);
      RAPIDJSON_ADDMEMBER_INT(tmpDoc, std::string("reuse_ip"), reuse_ip,
                              allocHash);
      RAPIDJSON_ADDMEMBER_INT(tmpDoc, std::string("reuse_failed_max"),
                              reuse_failed_max, allocHash);
      RAPIDJSON_ADDMEMBER_INT(tmpDoc, std::string("reuse_success_max"),
                              reuse_success_max, allocHash);
      RAPIDJSON_ADDMEMBER_INT(tmpDoc, std::string("dynamic_proxy_type"),
                              dynamic_proxy_type, allocHash);
      RAPIDJSON_ADDMEMBER_INT(tmpDoc,
                              std::string("dynamic_proxy_session_timeout"),
                              dynamic_proxy_session_timeout, allocHash);
      rapidjson::Value tmpValue(tmpDoc, doc.GetAllocator());
      doc.AddMember(rapidjson::Value("policy", doc.GetAllocator()).Move(),
                    tmpValue, doc.GetAllocator());
    }
  };

public:
  inline const tfIdentifyType &GetIdentify() const {
    return identify;
  }
  inline const std::string &GetSuper() const {
    return super;
  }
  inline const tfSeedType &GetSeed() const {
    return seed;
  }
  inline const chromium::xsiumio::RequestType &GetRequestType() const {
    return type;
  }
  inline bool ValidateRequestType() const {
    return type >= RequestType::kBegin && type <= RequestType::kEnd;
  }
  inline bool ValidateIdentify() const {
    return seed > 0 && seed <= 0xFFFFFFFFFFFFFFFF;
  }
  inline bool ValidateSeed() const {
    return seed > 0 && seed <= 0xFFFFFFFFFFFFFFFF;
  }
  inline bool ValidateSuper() const {
    return !super.empty();
  }
  inline const int &GetMode() const {
    return mode;
  }
  inline void
  Super(const std::function<std::string(const std::string &src)> &hashCb) {
    do {
      if (!hashCb)
        break;
      rapidjson::Document doc(rapidjson::kObjectType);
      *this >> doc;
      if (!doc.HasMember(xsiumio::kObjectKey))
        break;
      rapidjson::Value &refObj = doc[xsiumio::kObjectKey];
      for (auto it = refObj.MemberBegin(); it != refObj.MemberEnd(); ++it) {
        if (std::find(hashKeys.begin(), hashKeys.end(), it->name.GetString()) ==
            hashKeys.end()) {
          refObj.RemoveMember(it->name.GetString());
        }
      }
      Json::Sort(refObj, doc.GetAllocator());
      std::string hashSource = Json::toString(refObj);
      super = hashCb(hashSource);
    } while (0);
  }
  inline void operator=(const IXSiumio &other) {
    enable = other.enable;
    identify = other.identify;
    seed = other.seed;
    cookies = other.cookies;
    storage = other.storage;
    jss = other.jss;
    startup = other.startup;
    extensions = other.extensions;
    fps = other.fps;
    bridge = other.bridge;
    proxy = other.proxy;
    webrtcIPHandling = other.webrtcIPHandling;
    dynFpsUrl = other.dynFpsUrl;
    dynFpsInfo = other.dynFpsInfo;
    super = other.super;
    type = other.type;
    mode = other.mode;
    server = other.server;
    policy = other.policy;
  }
  inline void operator>>(rapidjson::Document &doc) const;
  inline void operator<<(const rapidjson::Value &v);
  inline void operator>>(std::string &json) const {
    do {
      rapidjson::Document doc(rapidjson::kObjectType);
      *this >> doc;
      json = Json::toString(doc);
    } while (0);
  }
  inline void operator<<(const std::string &json) {
    do {
      rapidjson::Document doc;
      if (doc.Parse(json.data(), json.size()).HasParseError())
        break;
      if (!doc.IsObject())
        break;
      if (doc.HasMember(kObjectKey)) {
        *this << doc[kObjectKey];
      } else {
        *this << doc;
      }
    } while (0);
  }

public:
  inline IXSiumio();
  inline IXSiumio(const std::string &);
  inline ~IXSiumio();

  bool enable = false; //!@ Main switch for the browser, default is true
  int mode = 0;        //!@ Mode, default is 0
  unsigned int chromium_main_pid = 0;
  unsigned int chromium_server_port = 0;
  int notifyCode = -1;
  std::string notifyMsg = "Ok";
  tfIdentifyType identify =
      0; //!@ Unique identifier, default is 0, used for synchronization
  RequestType type = RequestType::kDefault;
  Server server;
  Policy policy;
  tfSeedType seed = 0;
  Cookies cookies;
  Storage storage;
  Jss jss;
  Startup startup;
  Extensions extensions;
  Fingerprint fps; //!@ Fingerprint settings, default is empty
  Bridge bridge;
  Proxy proxy; //!@ Proxy information, default is empty
  WebRTCIPHandling webrtcIPHandling;
  std::string dynFpsUrl;
  DynamicFpsInfo dynFpsInfo; //!@ Dynamic FPS information, default is empty
  std::string super;

private:
  const std::vector<std::string> hashKeys = {
      "dynFpsInfo", "webrtcIPHandling", "fps",     "proxy",   "bridge",
      "seed",       "identify",         "cookies", "storage", "jss"};
};
inline IXSiumio::IXSiumio() {
}
inline IXSiumio::~IXSiumio() {
}
inline IXSiumio::IXSiumio(const std::string &json) {
  *this << json;
}
inline void IXSiumio::operator<<(const rapidjson::Value &v) {
  if (!v.IsObject())
    return;

  enable = false; //!@ Main switch for the browser, default is true
  identify = 0;   //!@ Unique identifier, default is 0, used for synchronization
  seed = 0;
  mode = 0;
  type = RequestType::kDefault;
  cookies = Cookies();
  storage = Storage();
  jss = Jss();
  startup = Startup();
  extensions = Extensions();
  fps = Fingerprint();
  bridge = Bridge();
  proxy = Proxy();
  webrtcIPHandling = WebRTCIPHandling();
  dynFpsUrl = "";
  dynFpsInfo = DynamicFpsInfo();
  super = "";
  server = Server();
  notifyCode = -1;
  notifyMsg = "Ok";
  policy = Policy();

  if (v.HasMember("enable") && v["enable"].IsBool()) {
    enable = v["enable"].GetBool();
  }
  if (v.HasMember("type") && v["type"].IsUint()) {
    type = static_cast<decltype(type)>(v["type"].GetUint());
  }
  if (v.HasMember("notifyCode") && v["notifyCode"].IsInt()) {
    notifyCode = v["notifyCode"].GetInt();
  }
  if (v.HasMember("notifyMsg") && v["notifyMsg"].IsString()) {
    notifyMsg = v["notifyMsg"].GetString();
  }
  if (v.HasMember("chromium_main_pid") && v["chromium_main_pid"].IsUint()) {
    chromium_main_pid = v["chromium_main_pid"].GetUint();
  }
  if (v.HasMember("chromium_server_port") &&
      v["chromium_server_port"].IsUint()) {
    chromium_server_port = v["chromium_server_port"].GetUint();
  }
  if (v.HasMember("mode") && v["mode"].IsInt()) {
    mode = v["mode"].GetInt();
  }
  if (v.HasMember("server") && v["server"].IsObject()) {
    server << v["server"];
  }
  if (v.HasMember("super") && v["super"].IsString()) {
    super = v["super"].GetString();
  }
  if (v.HasMember("identify") && v["identify"].IsUint64()) {
    identify = static_cast<decltype(identify)>(v["identify"].GetUint64());
  }
  if (v.HasMember("seed") && v["seed"].IsUint64()) {
    seed = static_cast<decltype(seed)>(v["seed"].GetUint64());
  }
  if (v.HasMember("dynFpsInfo") && v["dynFpsInfo"].IsObject()) {
    dynFpsInfo << v["dynFpsInfo"];
  }
  if (v.HasMember("policy") && v["policy"].IsObject()) {
    policy << v["policy"];
  }
  if (v.HasMember("cookies") && v["cookies"].IsObject()) {
    cookies << v["cookies"];
  }
  if (v.HasMember("storage") && v["storage"].IsObject()) {
    storage << v["storage"];
  }
  if (v.HasMember("jss") && v["jss"].IsObject()) {
    jss << v["jss"];
  }
  if (v.HasMember("startup") && v["startup"].IsObject()) {
    startup << v["startup"];
  }
  if (v.HasMember("webrtcIPHandling") && v["webrtcIPHandling"].IsObject()) {
    webrtcIPHandling << v["webrtcIPHandling"];
  }
  if (v.HasMember("dynFpsUrl") && v["dynFpsUrl"].IsString()) {
    dynFpsUrl = v["dynFpsUrl"].GetString();
  }
  if (v.HasMember("proxy") && v["proxy"].IsObject()) {
    proxy << v["proxy"];
  }
  if (v.HasMember("bridge") && v["bridge"].IsObject()) {
    bridge << v["bridge"];
  }
  if (v.HasMember("fps") && v["fps"].IsObject()) {
    fps << v["fps"];
  }
  if (v.HasMember("extensions") && v["extensions"].IsObject()) {
    extensions << v["extensions"];
  }
}
inline void IXSiumio::operator>>(rapidjson::Document &doc) const {
  if (!doc.IsObject())
    return;
  auto &allocator = doc.GetAllocator();
  if (doc.HasMember(kObjectKey))
    doc.RemoveMember(kObjectKey);

  rapidjson::Document xsiumioDoc(rapidjson::kObjectType);
  xsiumioDoc.SetObject();
  auto &xsiumioAllocator = xsiumioDoc.GetAllocator();
  RAPIDJSON_ADDMEMBER_BOOL(xsiumioDoc, std::string("enable"), enable,
                           xsiumioAllocator);
  RAPIDJSON_ADDMEMBER_UINT(xsiumioDoc, std::string("type"),
                           static_cast<tfRequestType>(type), xsiumioAllocator);
  RAPIDJSON_ADDMEMBER_STRING(xsiumioDoc, std::string("super"), super,
                             xsiumioAllocator);
  RAPIDJSON_ADDMEMBER_UINT64(xsiumioDoc, std::string("identify"), identify,
                             xsiumioAllocator);
  RAPIDJSON_ADDMEMBER_UINT64(xsiumioDoc, std::string("seed"), seed,
                             xsiumioAllocator);
  RAPIDJSON_ADDMEMBER_INT(xsiumioDoc, std::string("mode"), mode,
                          xsiumioAllocator);
  RAPIDJSON_ADDMEMBER_UINT(xsiumioDoc, std::string("chromium_main_pid"),
                           chromium_main_pid, xsiumioAllocator);
  RAPIDJSON_ADDMEMBER_UINT(xsiumioDoc, std::string("chromium_server_port"),
                           chromium_server_port, xsiumioAllocator);
  RAPIDJSON_ADDMEMBER_INT(xsiumioDoc, std::string("notifyCode"), notifyCode,
                          xsiumioAllocator);
  RAPIDJSON_ADDMEMBER_STRING(xsiumioDoc, std::string("notifyMsg"), notifyMsg,
                             xsiumioAllocator);
  server >> xsiumioDoc;
  bridge >> xsiumioDoc;
  proxy >> xsiumioDoc;
  cookies >> xsiumioDoc;
  storage >> xsiumioDoc;
  jss >> xsiumioDoc;
  startup >> xsiumioDoc;
  fps >> xsiumioDoc;
  extensions >> xsiumioDoc;
  webrtcIPHandling >> xsiumioDoc;
  dynFpsInfo >> xsiumioDoc;
  policy >> xsiumioDoc;
  RAPIDJSON_ADDMEMBER_STRING(xsiumioDoc, std::string("dynFpsUrl"), dynFpsUrl,
                             xsiumioAllocator);
  rapidjson::Value xsiumioValue(xsiumioDoc, allocator);
  doc.AddMember(rapidjson::Value(kObjectKey, allocator).Move(), xsiumioValue,
                allocator);
}
inline IXSiumio::Policy::Policy() = default;
inline IXSiumio::Policy::~Policy() = default;
inline IXSiumio::Cookies::Cookies() = default;
inline IXSiumio::Cookies::~Cookies() = default;
inline IXSiumio::Storage::Storage() = default;
inline IXSiumio::Storage::~Storage() = default;
inline IXSiumio::Startup::Startup() = default;
inline IXSiumio::Startup::~Startup() = default;
inline IXSiumio::Server::Server() = default;
inline IXSiumio::Server::~Server() = default;
inline IXSiumio::Jss::Jss() = default;
inline IXSiumio::Jss::~Jss() = default;
inline IXSiumio::WebRTCIPHandling::WebRTCIPHandling() = default;
inline IXSiumio::WebRTCIPHandling::~WebRTCIPHandling() = default;
inline IXSiumio::Proxy::Proxy() = default;
inline IXSiumio::Proxy::~Proxy() = default;
inline IXSiumio::Bridge::Bridge() = default;
inline IXSiumio::Bridge::~Bridge() = default;
inline IXSiumio::Extensions::Extensions() = default;
inline IXSiumio::Extensions::~Extensions() = default;
inline IXSiumio::Fingerprint::Fingerprint() = default;
inline IXSiumio::Fingerprint::~Fingerprint() = default;
inline IXSiumio::Fingerprint::Hash::Hash() = default;
inline IXSiumio::Fingerprint::Hash::~Hash() = default;
inline IXSiumio::Fingerprint::Math::Math() = default;
inline IXSiumio::Fingerprint::Math::~Math() = default;
inline IXSiumio::Fingerprint::V8::V8() = default;
inline IXSiumio::Fingerprint::V8::~V8() = default;
inline IXSiumio::Fingerprint::Canvas::Canvas() = default;
inline IXSiumio::Fingerprint::Canvas::~Canvas() = default;
inline IXSiumio::Fingerprint::Audio::Audio() = default;
inline IXSiumio::Fingerprint::Audio::~Audio() = default;
inline IXSiumio::Fingerprint::Webgl::Webgl() = default;
inline IXSiumio::Fingerprint::Webgl::~Webgl() = default;
inline IXSiumio::Fingerprint::Tlspeet::Tlspeet() = default;
inline IXSiumio::Fingerprint::Tlspeet::~Tlspeet() = default;
inline IXSiumio::Fingerprint::Webgl::ContextAttributes::ContextAttributes() =
    default;
inline IXSiumio::Fingerprint::Webgl::ContextAttributes::~ContextAttributes() =
    default;
inline IXSiumio::Fingerprint::Webgl::ShaderPrecisionFormat::
    ShaderPrecisionFormat() = default;
inline IXSiumio::Fingerprint::Webgl::ShaderPrecisionFormat::
    ~ShaderPrecisionFormat() = default;
inline IXSiumio::Fingerprint::Webgl::ShaderPrecisionFormat::_8DF_::_8DF_() =
    default;
inline IXSiumio::Fingerprint::Webgl::ShaderPrecisionFormat::_8DF_::~_8DF_() =
    default;
inline IXSiumio::Fingerprint::Webgl::ShaderPrecisionFormat::_8B30_::_8B30_() =
    default;
inline IXSiumio::Fingerprint::Webgl::ShaderPrecisionFormat::_8B30_::~_8B30_() =
    default;
inline IXSiumio::Fingerprint::Webgl::ShaderPrecisionFormat::_8B31_::_8B31_() =
    default;
inline IXSiumio::Fingerprint::Webgl::ShaderPrecisionFormat::_8B31_::~_8B31_() =
    default;
inline IXSiumio::Fingerprint::Font::Font() = default;
inline IXSiumio::Fingerprint::Font::~Font() = default;
inline IXSiumio::Fingerprint::CdmRegistry::CdmRegistry() = default;
inline IXSiumio::Fingerprint::CdmRegistry::~CdmRegistry() = default;
inline IXSiumio::Fingerprint::CdmRegistry::CdmEntry::CdmEntry() = default;
inline IXSiumio::Fingerprint::CdmRegistry::CdmEntry::~CdmEntry() = default;
inline IXSiumio::Fingerprint::Screen::Screen() = default;
inline IXSiumio::Fingerprint::Screen::~Screen() = default;
inline IXSiumio::Fingerprint::Screen::Dpi::Dpi() = default;
inline IXSiumio::Fingerprint::Screen::Dpi::~Dpi() = default;
inline IXSiumio::Fingerprint::Disk::Disk() = default;
inline IXSiumio::Fingerprint::Disk::~Disk() = default;
inline IXSiumio::Fingerprint::UserAgentMetadata::UserAgentMetadata() = default;
inline IXSiumio::Fingerprint::UserAgentMetadata::~UserAgentMetadata() = default;
inline IXSiumio::Fingerprint::UserAgentMetadata::Brand_version::
    Brand_version() = default;
inline IXSiumio::Fingerprint::UserAgentMetadata::Brand_version::
    ~Brand_version() = default;
inline IXSiumio::Fingerprint::UserAgentMetadata::Brand_full_version::
    Brand_full_version() = default;
inline IXSiumio::Fingerprint::UserAgentMetadata::Brand_full_version::
    ~Brand_full_version() = default;
inline IXSiumio::Fingerprint::Voice::Voice() = default;
inline IXSiumio::Fingerprint::Voice::~Voice() = default;
inline IXSiumio::Fingerprint::Voice::Data::Data() = default;
inline IXSiumio::Fingerprint::Voice::Data::~Data() = default;
inline IXSiumio::Fingerprint::Product::Product() = default;
inline IXSiumio::Fingerprint::Product::~Product() = default;
inline IXSiumio::DynamicFpsInfo::DynamicFpsInfo() = default;
inline IXSiumio::DynamicFpsInfo::~DynamicFpsInfo() = default;
inline IXSiumio::DynamicFpsInfo::Ipinfo::Ipinfo() = default;
inline IXSiumio::DynamicFpsInfo::Ipinfo::~Ipinfo() = default;
} // namespace xsiumio
} // namespace chromium
/// /*_ Memade®（新生™） _**/
/// /*_ Sat, 19 Jul 2025 21:20:54 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif ///__5E6CBAF3_846B_4EF0_B587_0B9D7DC7F803__