#pragma once

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <string>
#include <functional>

// Windows 服务包装器类
class WindowsService {
public:
    WindowsService(const std::string& serviceName, const std::string& displayName);
    ~WindowsService();

    // 设置服务处理函数
    void SetServiceHandler(std::function<void()> startHandler, 
                          std::function<void()> stopHandler);

    // 安装服务
    bool InstallService(const std::string& binaryPath);

    // 卸载服务
    bool UninstallService();

    // 启动服务
    bool StartService();

    // 停止服务
    bool StopService();

    // 作为服务运行
    bool RunAsService();

    // 作为控制台应用运行
    void RunAsConsole(std::function<void()> mainHandler);

private:
    static void WINAPI ServiceMain(DWORD argc, LPTSTR* argv);
    static void WINAPI ServiceCtrlHandler(DWORD ctrlCode);
    static DWORD WINAPI ServiceWorkerThread(LPVOID lpParam);

    void SetServiceStatus(DWORD currentState, DWORD exitCode = 0, DWORD waitHint = 0);
    void WriteToEventLog(const std::string& message, WORD type = EVENTLOG_INFORMATION_TYPE);

    std::string m_serviceName;
    std::string m_displayName;
    SERVICE_STATUS_HANDLE m_statusHandle;
    SERVICE_STATUS m_serviceStatus;
    HANDLE m_serviceStopEvent;
    
    std::function<void()> m_startHandler;
    std::function<void()> m_stopHandler;

    static WindowsService* s_instance;
};

// 服务管理辅助函数
namespace ServiceUtils {
    bool IsRunningAsService();
    bool IsElevated();
    std::string GetLastErrorString();
    void PrintUsage();
}

#endif // _WIN32