#include "bridge.hpp"

//int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline,
//	int cmdshow) {
//	//BridgeService* bridge = BridgeService::CreateOrGet();
//	//bridge->Start();
//	//std::this_thread::sleep_for(std::chrono::milliseconds(10000));
//	//bridge->Stop();
//	//BridgeService::Destroy();
//	//do {
//	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//	//} while (1);
//	BridgeService::CreateOrGet()->Perform();
//	return 0;
//}


//////////////////////////////////////////////////////////////////////////////////////////

#if defined(SHARED_IMPLEMENTATION)
#define SHARED_API __declspec(dllexport)
#define SHARED_API_PRIVATE __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	SHARED_API void* interface_init(void* data, unsigned long len) {
		BridgeService* bridge = BridgeService::CreateOrGet();
		return reinterpret_cast<void*>(dynamic_cast<IBridge*>(bridge));
	}
	SHARED_API void interface_uninit() {
		BridgeService::Destroy();
	}

#ifdef __cplusplus
}
#endif
