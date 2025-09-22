#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <Windows.h>
#include "../include/ibridge.h"
namespace{
	static std::string ReadFile(
		const std::string& file_,
		const int& mode_ = std::ios::in | std::ios::binary) {
		std::string result;
		try {
			std::fstream of(std::filesystem::path(file_), static_cast<std::ios_base::openmode>(mode_));
			do {
				if (!of.is_open())
					break;
				of.seekg(0, of.end);
				size_t size = static_cast<size_t>(of.tellg());
				if (size <= 0)
					break;
				result.resize(size, 0x00);
				of.seekg(0, of.beg);
				of.read(&result[0], size);
			} while (0);
			if (of.is_open())
				of.close();
		}
		catch (const std::filesystem::filesystem_error& e) {
			std::cout << e.what() << std::endl;
		}
		return result;
	}
}///namespace
//const std::string test_cfg = R"JSON({"xsiumio":{"enable":true,"type":1,"super":"6ccd0699437e0203ab0d38a4cb461711f0d19d073125f7a3f386e2100c88fce1","identify":5588668,"seed":13402498682075557,"mode":0,"chromium_main_pid":0,"chromium_server_port":0,"notifyCode":1,"notifyMsg":"Done","server":{"enable":false,"host":"127.0.0.1","port":65535},"bridge":{"enable":true,"config":"config.conf","single_process":true,"server_listen":"127.0.0.1:55668","reuse_port":false,"happyeyeballs":false,"v6only":false,"v4only":false,"local_ip":"","transparent":false,"so_mark":-1,"udp_timeout":60,"tcp_timeout":-1,"rate_limit":-1,"proxy_pass":"socks5h://windows-zone-windows-session-687116-sessTime-10:111111@pr-na.ipidea.io:2333","proxy_pass_ssl":false,"ssl_certificate_dir":"","ssl_cacert_dir":"","ssl_sni":"","proxy_ssl_name":"","ssl_ciphers":"","ssl_prefer_server_ciphers":false,"ipip_db":"17monipdb.datx","http_doc":"/tmp/","htpasswd":false,"autoindex":true,"logs_path":"","disable_logs":true,"disable_http":false,"disable_socks":false,"disable_udp":false,"disable_insecure":false,"scramble":false,"noise_length":0,"deny_region":[],"allow_region":[],"auth_users":[],"users_rate_limit":[]},"proxy":{"enable":true,"url":"http://127.0.0.1:55668","credentials":true},"cookies":{"enable":true,"blocklist":["c.adsco.re"]},"storage":{"enable":true,"blocklist":["c.adsco.re"]},"jss":{"enable":true,"frameImplDidClearWindowObject":""},"startup":{"enable":true,"enable_cleanup_udd":false,"homepage":"","search_engine":"","search_engine_id":0,"startup_pid":11616,"urls":[],"args":["--window-size1=640,480","--window-position1=10,10","--no-sandbox","--disable-web-security","--no-first-run","--no-default-browser-check","--disable-session-crashed-bubble","--disable-restore-session-state","--disable-background-mode","--disable-account-consistency","--disable-gaia-services","--disable-top-sites","--disable-sync","--disable-popup-blocking","--disable-accelerated-2d-canvas","--disable-features=DnsOverHttps,ChromeSignin,AccountConsistency,SameSiteByDefaultCookies,CookiesWithoutSameSiteMustBeSecure,ThirdPartyCookieDeprecationTrial,TrackingProtection3PCD","--host-resolver-rules=MAP * ~NOTFOUND , EXCLUDE 127.0.0.1","--proxy-bypass-list=<-loopback>"]},"fps":{"enable":true,"real_fingerprint_output":false,"navigatorWebdriver":false,"do_not_track":false,"platform":"Win32","hardwareConcurrency":8,"deviceMemory":4,"userAgent":"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/138.0.0.0 Safari/537.36","rtt":196,"netDnsAllowFallbackToSystemtask":false,"languages":["zh-HK","zh","en"],"product":{"name":"Google Chrome","version":"138.0.7204.158","chromium":"138.0.7204.158"},"userAgentMetadata":{"enable":true,"brand_version_list":[],"brand_full_version_list":[],"full_version":"138.0.7204.158","platform":"Windows","platform_version":"19.0.0","architecture":"x86","model":"","mobile":false,"bitness":"64","wow64":false,"form_factors":["Desktop"]},"voice":{"enable":true,"datas":[{"lang":"de-DE","name":"Google Deutsch"},{"lang":"en-US","name":"Google US English"},{"lang":"en-GB","name":"Google UK English Female"},{"lang":"en-GB","name":"Google UK English Male"},{"lang":"es-ES","name":"Google español"},{"lang":"es-US","name":"Google español de Estados Unidos"},{"lang":"fr-FR","name":"Google français"},{"lang":"hi-IN","name":"Google हिन्दी"},{"lang":"id-ID","name":"Google Bahasa Indonesia"},{"lang":"it-IT","name":"Google italiano"},{"lang":"ja-JP","name":"Google 日本語"},{"lang":"ko-KR","name":"Google 한국의"},{"lang":"nl-NL","name":"Google Nederlands"},{"lang":"pl-PL","name":"Google polski"},{"lang":"pt-BR","name":"Google português do Brasil"},{"lang":"ru-RU","name":"Google русский"},{"lang":"zh-CN","name":"Google 普通话（中国大陆）"},{"lang":"zh-HK","name":"Google 粤語（香港）"},{"lang":"zh-TW","name":"Google 國語（臺灣）"}]},"canvas":{"enable":true,"hash":{"random":false,"base":503625.6258244903,"from":0.000009999999747378752,"to":100.00000762939453}},"webgl":{"enable":true,"hash":{"random":false,"base":57.67282812936959,"from":0.000009999999747378752,"to":100.00000762939453},"getParameter":{"3379":"64","3386":"16384,16384","3410":"8","3411":"8","3412":"0","3413":"0","3414":"16","3415":"1","7936":"WebKit","7937":"WebKit WebGL","7938":"WebGL 2.0 (OpenGL ES 3.0 Chromium)", "33901":"1024", "33902" : "1,1", "34024" : "2048", "34047" : "8", "34076" : "64", "34921" : "16", "34930" : "16", "35660" : "16", "35661" : "128", "35724" : "WebGL GLSL ES 3.00 (OpenGL ES GLSL ES 3.0 Chromium)", "36347" : "256", "36348" : "128", "36349" : "16384", "37445" : "Google Inc. (NVIDIA)", "37446" : "ANGLE (NVIDIA, NVIDIA GeForce GTX 1060 (0x00001C03) Direct3D11 vs_5_0 ps_5_0, D3D11)"}, "contextAttributes":{"alpha":true, "antialias" : true, "depth" : true, "desynchronized" : false, "failIfMajorPerformanceCaveat" : false, "powerPreference" : "high-performance", "premultipliedAlpha" : false, "preserveDrawingBuffer" : false, "stencil" : true, "xrCompatible" : false, "enable" : false}, "shaderPrecisionFormat" : {"8B30":{"8DF0":{"rangeMin":127, "rangeMax" : 127, "precision" : 255}, "8DF1" : {"rangeMin":125, "rangeMax" : 129, "precision" : 22}, "8DF2" : {"rangeMin":125, "rangeMax" : 127, "precision" : 23}, "8DF3" : {"rangeMin":120, "rangeMax" : 127, "precision" : 10}, "8DF4" : {"rangeMin":120, "rangeMax" : 127, "precision" : 10}, "8DF5" : {"rangeMin":120, "rangeMax" : 255, "precision" : 10}}, "8B31" : {"8DF0":{"rangeMin":126, "rangeMax" : 128, "precision" : 8}, "8DF1" : {"rangeMin":125, "rangeMax" : 127, "precision" : 30}, "8DF2" : {"rangeMin":127, "rangeMax" : 30, "precision" : 8}, "8DF3" : {"rangeMin":120, "rangeMax" : 30, "precision" : 31}, "8DF4" : {"rangeMin":120, "rangeMax" : 127, "precision" : 0}, "8DF5" : {"rangeMin":120, "rangeMax" : 127, "precision" : 2}}, "enable" : false}}, "audio":{"enable":true, "hash" : {"random":false, "base" : 56.54744858386513, "from" : 0.000009999999747378752, "to" : 100.00000762939453}}, "math" : {"enable":true, "hash" : {"random":false, "base" : 4.396053334915892e-7, "from" : 0.000009999999747378752, "to" : 100.00000762939453}}, "v8" : {"enable":true, "hash" : {"random":false, "base" : 13402498682075556.0, "from" : 0.0, "to" : 0.0}}, "font" : {"enable":true, "hash" : {"random":false, "base" : 3.141592502593994, "from" : 0.000009999999747378752, "to" : 100.00000762939453}, "allowlist" : ["Arial", "Bahnschrift", "Calibri", "Cambria", "Cambria Math", "Candara", "Comic Sans MS", "Consolas", "Constantia", "Corbel", "Courier New", "Franklin Gothic", "Gabriola", "Gadugi", "Georgia", "Impact", "Javanese Text", "Lucida Console", "Lucida Sans Unicode", "MS Gothic", "MV Boli", "Malgun Gothic", "Microsoft Himalaya", "Microsoft JhengHei", "Microsoft New Tai Lue", "Microsoft PhagsPa", "Microsoft Sans Serif", "Microsoft Tai Le", "Microsoft YaHei", "Microsoft Yi Baiti", "MingLiU-ExtB", "MingLiU_HKSCS-ExtB", "Mongolian Baiti", "Myanmar Text", "PMingLiU-ExtB", "Palatino Linotype", "Segoe Script", "Segoe UI", "Segoe UI Emoji", "Segoe UI Symbol", "SimSun", "SimSun-ExtB", "Sitka Small", "Sylfaen", "Symbol", "Trebuchet MS", "Verdana", "Webdings"] }, "screen" : {"enable":true, "height" : 1600, "width" : 2560, "colorDepth" : 32, "pixelDepth" : 32, "availLeft" : 0, "availTop" : 0, "availHeight" : 1560, "availWidth" : 2560, "devicePixelRatio" : 1.5, "dpi" : {"x":144, "y" : 144}}, "tlspeet" : {"enable":true, "grease_seed" : 177, "enable_http2_settings_grease" : false, "permute_extensions" : false}, "disk" : {"enable":true, "hash" : {"random":false, "base" : 0.0, "from" : 0.0, "to" : 0.0}, "quota" : 788849126576, "usage" : 743849126576}, "cdmRegistry" : {"enable":true, "hash" : {"random":false, "base" : 0.0, "from" : 0.0, "to" : 0.0}, "allowlist" : [{"key_system":"org.w3.clearkey", "mime_type" : "video/mp4; codecs=\"avc1.42E01E\"", "robustness" : ""}, { "key_system":"com.widevine.alpha","mime_type" : "video/mp4; codecs=\"avc1.42E01E\"","robustness" : "SW_SECURE_CRYPTO" }, { "key_system":"com.widevine.alpha","mime_type" : "video/mp4; codecs=\"avc1.42E01E\"","robustness" : "SW_SECURE_DECODE" }, { "key_system":"org.w3.clearkey","mime_type" : "video/mp4; codecs=\"avc1.42c00d\"","robustness" : "" }, { "key_system":"com.widevine.alpha","mime_type" : "video/mp4; codecs=\"avc1.42c00d\"","robustness" : "SW_SECURE_CRYPTO" }, { "key_system":"com.widevine.alpha","mime_type" : "video/mp4; codecs=\"avc1.42c00d\"","robustness" : "SW_SECURE_DECODE" }] }}, "extensions":{"enable":true, "allowlist" : ["afgbmmdnakcefnkchckgelobigkbboci", "dkmbljdmlgohodhbbddncmbadcoepobl", "ebglcogbaklfalmoeccdjbmgfcacengf", "facgnnelgcipeopfbjcajpaibhhdjgcp", "fjongodbpmhjekocjdkdclopbjcgnjlj", "gdjlkcdpddmfnnafnbjkbamdhjjlpkga", "hgcaibhfajimiadchhkppepbecmommad", "iehijoelpeicgdpmemibdkbmpmealfhf", "ijjmpjalkodlophonbmjoeabifkepgke", "iopcliemaddhijhmjbecffinafojoofk", "mdmeamacpckfoenjdchgibipmkkakaea", "mmacanpinhhkmgdoebejenmkngocobej", "modppjhbgaggghdagdlmapifkfiffjem"] }, "webrtcIPHandling" : {"policy":"disable_non_proxied_udp", "port" : 0, "public_ip" : "61.244.112.229", "internal_ip" : ""}, "dynFpsInfo" : {"enable":true, "ipinfo" : {"ip":"61.244.112.229", "country" : "China", "region" : "Hong Kong", "city" : "Hong Kong", "zip" : "", "lat" : "", "lon" : "", "timezone" : "Asia/Hong_Kong"}}, "policy" : {"enable":true, "reuse" : true, "dynamic_proxy" : true, "reuse_ip" : 0, "reuse_failed_max" : 3, "reuse_success_max" : 10, "dynamic_proxy_type" : 1, "dynamic_proxy_session_timeout" : 10}, "dynFpsUrl" : "https://myip.ipipv.com"}})JSON";
int main(int argc, char** argv) {

	typedef void* (*interface_init)(const void*, unsigned long);
	typedef void(*interface_uninit)(void);
	interface_init init = nullptr;
	interface_uninit uninit = nullptr;
	HMODULE	hModule = LoadLibraryA("libbridge.dll");
	IBridge* bridge = nullptr;
	do {
		if (!hModule)
			break;
		init = (interface_init)GetProcAddress(hModule, "interface_init");
		uninit = (interface_uninit)GetProcAddress(hModule, "interface_uninit");
		if (!init || !uninit)
			break;
		bridge = reinterpret_cast<IBridge*>(init(nullptr, 0));
		if (!bridge)
			break;
		std::string test_cfg = ReadFile(R"(C:\Users\Administrator\AppData\Roaming\ChromiumDev\configures\xsiumio.json)");
		bridge->Start(test_cfg.data(),static_cast<unsigned long>(test_cfg.size()));
		std::string input;
		do {
			input.clear();
			std::getline(std::cin, input);
			if (input == "q" || std::cin.eof()) {
				bridge->Stop();
				break;
			}
			else {
			}
		} while (1);


	} while (0);

	if (uninit)
		uninit();
	if (hModule) {
		FreeLibrary(hModule);
	}
	return 0;
}