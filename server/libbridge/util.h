#if !defined(__D9079DA5_DB2A_4DF8_8CD7_348CEBED0351__)
#define __D9079DA5_DB2A_4DF8_8CD7_348CEBED0351__

#ifdef __linux__
#  include <sys/resource.h>

# ifndef HAVE_UNAME
#  define HAVE_UNAME
# endif

#elif _WIN32
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <fcntl.h>
#  include <io.h>
#  include <windows.h>

#endif // _WIN32

#ifdef HAVE_UNAME
#  include <sys/utsname.h>
#endif

#if __has_include("openssl/opensslv.h")
#  include "openssl/opensslv.h"
#else
#  ifndef OPENSSL_VERSION_TEXT
#    define OPENSSL_VERSION_TEXT "NONE"
#  endif
#endif

#include <boost/version.hpp>
#include <boost/config.hpp>

#include <string>
#include <string_view>
#include <iterator>
#include <algorithm>
#include <optional>
#include <iostream>
#include <sstream>
#include <memory>
#include <ios>

class Util final {
public:
	Util() = default;
	~Util() = default;
public:
	static std::string version_info();
	static int platform_init();
	static bool parse_endpoint_string(std::string_view str,
		std::string& host, std::string& port, bool& ipv6only);
	static bool is_space(const char c);
	static std::string_view string_trim(std::string_view sv);
};

/// /*_ Memade®（新生™） _**/
/// /*_ Sun, 07 Sep 2025 01:28:53 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__D9079DA5_DB2A_4DF8_8CD7_348CEBED0351__