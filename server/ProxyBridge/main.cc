#include "bridge.hpp"

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline,
	int cmdshow) {
	//BridgeService* bridge = BridgeService::CreateOrGet();
	//bridge->Start();
	//std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	//bridge->Stop();
	//BridgeService::Destroy();
	//do {
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//} while (1);
	BridgeService::CreateOrGet()->Perform();
	return 0;
}
