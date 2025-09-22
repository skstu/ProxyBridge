#ifdef _WIN32
#include "windows_service.hpp"
#include <iostream>
#include <sstream>

WindowsService* WindowsService::s_instance = nullptr;

WindowsService::WindowsService(const std::string& serviceName, const std::string& displayName)
    : m_serviceName(serviceName)
    , m_displayName(displayName)
    , m_statusHandle(nullptr)
    , m_serviceStopEvent(nullptr)
{
    s_instance = this;
    
    // 初始化服务状态
    ZeroMemory(&m_serviceStatus, sizeof(m_serviceStatus));
    m_serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    m_serviceStatus.dwCurrentState = SERVICE_START_PENDING;
    m_serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
}

WindowsService::~WindowsService() {
    if (m_serviceStopEvent) {
        CloseHandle(m_serviceStopEvent);
    }
    s_instance = nullptr;
}

void WindowsService::SetServiceHandler(std::function<void()> startHandler, 
                                      std::function<void()> stopHandler) {
    m_startHandler = startHandler;
    m_stopHandler = stopHandler;
}

bool WindowsService::InstallService(const std::string& binaryPath) {
    SC_HANDLE schSCManager = OpenSCManager(
        nullptr,                    // local computer
        nullptr,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);     // full access rights 

    if (!schSCManager) {
        WriteToEventLog("OpenSCManager failed: " + ServiceUtils::GetLastErrorString(), 
                       EVENTLOG_ERROR_TYPE);
        return false;
    }

    SC_HANDLE schService = CreateServiceA(
        schSCManager,              // SCM database 
        m_serviceName.c_str(),     // name of service 
        m_displayName.c_str(),     // service name to display 
        SERVICE_ALL_ACCESS,        // desired access 
        SERVICE_WIN32_OWN_PROCESS, // service type 
        SERVICE_AUTO_START,        // start type 
        SERVICE_ERROR_NORMAL,      // error control type 
        binaryPath.c_str(),        // path to service's binary 
        nullptr,                   // no load ordering group 
        nullptr,                   // no tag identifier 
        nullptr,                   // no dependencies 
        nullptr,                   // LocalSystem account 
        nullptr);                  // no password 

    if (!schService) {
        DWORD error = GetLastError();
        CloseServiceHandle(schSCManager);
        
        if (error == ERROR_SERVICE_EXISTS) {
            WriteToEventLog("Service already exists", EVENTLOG_WARNING_TYPE);
            return true;
        }
        
        WriteToEventLog("CreateService failed: " + ServiceUtils::GetLastErrorString(), 
                       EVENTLOG_ERROR_TYPE);
        return false;
    }

    WriteToEventLog("Service installed successfully");
    
    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return true;
}

bool WindowsService::UninstallService() {
    SC_HANDLE schSCManager = OpenSCManager(
        nullptr,                   // local computer
        nullptr,                   // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);    // full access rights 

    if (!schSCManager) {
        WriteToEventLog("OpenSCManager failed: " + ServiceUtils::GetLastErrorString(), 
                       EVENTLOG_ERROR_TYPE);
        return false;
    }

    SC_HANDLE schService = OpenServiceA(
        schSCManager,              // SCM database 
        m_serviceName.c_str(),     // name of service 
        DELETE);                   // need delete access 

    if (!schService) {
        CloseServiceHandle(schSCManager);
        WriteToEventLog("OpenService failed: " + ServiceUtils::GetLastErrorString(), 
                       EVENTLOG_ERROR_TYPE);
        return false;
    }

    // 先停止服务
    SERVICE_STATUS serviceStatus;
    ControlService(schService, SERVICE_CONTROL_STOP, &serviceStatus);

    // 删除服务
    if (!DeleteService(schService)) {
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        WriteToEventLog("DeleteService failed: " + ServiceUtils::GetLastErrorString(), 
                       EVENTLOG_ERROR_TYPE);
        return false;
    }

    WriteToEventLog("Service uninstalled successfully");
    
    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return true;
}

bool WindowsService::RunAsService() {
    SERVICE_TABLE_ENTRY serviceTable[] = {
        { const_cast<LPSTR>(m_serviceName.c_str()), ServiceMain },
        { nullptr, nullptr }
    };

    if (!StartServiceCtrlDispatcher(serviceTable)) {
        WriteToEventLog("StartServiceCtrlDispatcher failed: " + 
                       ServiceUtils::GetLastErrorString(), EVENTLOG_ERROR_TYPE);
        return false;
    }

    return true;
}

void WINAPI WindowsService::ServiceMain(DWORD argc, LPTSTR* argv) {
    if (!s_instance) return;

    s_instance->m_statusHandle = RegisterServiceCtrlHandler(
        s_instance->m_serviceName.c_str(), ServiceCtrlHandler);

    if (!s_instance->m_statusHandle) {
        s_instance->WriteToEventLog("RegisterServiceCtrlHandler failed: " + 
                                   ServiceUtils::GetLastErrorString(), EVENTLOG_ERROR_TYPE);
        return;
    }

    // 报告服务正在启动
    s_instance->SetServiceStatus(SERVICE_START_PENDING);

    // 创建停止事件
    s_instance->m_serviceStopEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    if (!s_instance->m_serviceStopEvent) {
        s_instance->SetServiceStatus(SERVICE_STOPPED);
        return;
    }

    // 报告服务正在运行
    s_instance->SetServiceStatus(SERVICE_RUNNING);

    // 启动服务工作线程
    HANDLE hThread = CreateThread(nullptr, 0, ServiceWorkerThread, nullptr, 0, nullptr);
    if (hThread) {
        // 等待停止事件
        WaitForSingleObject(s_instance->m_serviceStopEvent, INFINITE);
        CloseHandle(hThread);
    }

    // 报告服务已停止
    s_instance->SetServiceStatus(SERVICE_STOPPED);
}

void WINAPI WindowsService::ServiceCtrlHandler(DWORD ctrlCode) {
    if (!s_instance) return;

    switch (ctrlCode) {
    case SERVICE_CONTROL_STOP:
        s_instance->SetServiceStatus(SERVICE_STOP_PENDING);
        
        // 调用停止处理器
        if (s_instance->m_stopHandler) {
            s_instance->m_stopHandler();
        }
        
        // 设置停止事件
        SetEvent(s_instance->m_serviceStopEvent);
        break;

    case SERVICE_CONTROL_INTERROGATE:
        break;

    default:
        break;
    }
}

DWORD WINAPI WindowsService::ServiceWorkerThread(LPVOID lpParam) {
    if (!s_instance || !s_instance->m_startHandler) {
        return ERROR_INVALID_PARAMETER;
    }

    try {
        // 调用启动处理器
        s_instance->m_startHandler();
    }
    catch (const std::exception& e) {
        s_instance->WriteToEventLog("Service worker thread exception: " + std::string(e.what()), 
                                   EVENTLOG_ERROR_TYPE);
        return ERROR_EXCEPTION_IN_SERVICE;
    }

    return ERROR_SUCCESS;
}

void WindowsService::SetServiceStatus(DWORD currentState, DWORD exitCode, DWORD waitHint) {
    static DWORD checkPoint = 1;

    m_serviceStatus.dwCurrentState = currentState;
    m_serviceStatus.dwWin32ExitCode = exitCode;
    m_serviceStatus.dwWaitHint = waitHint;

    if (currentState == SERVICE_START_PENDING) {
        m_serviceStatus.dwControlsAccepted = 0;
    } else {
        m_serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    }

    if ((currentState == SERVICE_RUNNING) || (currentState == SERVICE_STOPPED)) {
        m_serviceStatus.dwCheckPoint = 0;
    } else {
        m_serviceStatus.dwCheckPoint = checkPoint++;
    }

    SetServiceStatus(m_statusHandle, &m_serviceStatus);
}

void WindowsService::WriteToEventLog(const std::string& message, WORD type) {
    HANDLE hEventSource = RegisterEventSource(nullptr, m_serviceName.c_str());
    if (hEventSource) {
        const char* strings[1] = { message.c_str() };
        ReportEventA(hEventSource, type, 0, 0, nullptr, 1, 0, strings, nullptr);
        DeregisterEventSource(hEventSource);
    }
}

void WindowsService::RunAsConsole(std::function<void()> mainHandler) {
    if (mainHandler) {
        mainHandler();
    }
}

// ServiceUtils 实现
namespace ServiceUtils {
    bool IsRunningAsService() {
        return GetConsoleWindow() == nullptr;
    }

    bool IsElevated() {
        BOOL elevated = FALSE;
        HANDLE token = nullptr;
        
        if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
            TOKEN_ELEVATION elevation;
            DWORD size = sizeof(elevation);
            
            if (GetTokenInformation(token, TokenElevation, &elevation, sizeof(elevation), &size)) {
                elevated = elevation.TokenIsElevated;
            }
            CloseHandle(token);
        }
        
        return elevated == TRUE;
    }

    std::string GetLastErrorString() {
        DWORD error = GetLastError();
        if (error == 0) return "No error";

        LPSTR messageBuffer = nullptr;
        size_t size = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, error, 0, reinterpret_cast<LPSTR>(&messageBuffer), 0, nullptr);

        std::string message(messageBuffer, size);
        LocalFree(messageBuffer);
        
        return message;
    }

    void PrintUsage() {
        std::cout << "ProxyBridge Windows Service\n";
        std::cout << "Usage:\n";
        std::cout << "  proxy.exe [options]\n";
        std::cout << "  proxy.exe --install    Install as Windows service\n";
        std::cout << "  proxy.exe --uninstall  Uninstall Windows service\n";
        std::cout << "  proxy.exe --start      Start service\n";
        std::cout << "  proxy.exe --stop       Stop service\n";
        std::cout << "  proxy.exe --console    Run as console application\n";
        std::cout << "\nService Options:\n";
        std::cout << "  --config <file>        Configuration file path\n";
        std::cout << "  --log <file>           Log file path\n";
        std::cout << "\nExample:\n";
        std::cout << "  proxy.exe --install --config config.json\n";
        std::cout << "  proxy.exe --console --config config.json\n";
    }
}

#endif // _WIN32