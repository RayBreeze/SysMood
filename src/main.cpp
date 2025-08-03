#include <iostream>
#include "config.h"
#include "web_server.h"

#ifdef _WIN32
#include <windows.h>

SERVICE_STATUS        g_ServiceStatus = {0};
SERVICE_STATUS_HANDLE g_StatusHandle = NULL;
HANDLE                g_ServiceStopEvent = INVALID_HANDLE_VALUE;

VOID WINAPI ServiceMain(DWORD argc, LPTSTR* argv);
VOID WINAPI ServiceCtrlHandler(DWORD CtrlCode);

#else
#include <unistd.h>
#include <sys/stat.h>
#endif

void run_daemon() {
#ifdef _WIN32
    SERVICE_TABLE_ENTRY ServiceTable[] = {
        {(LPSTR)"SysMood", (LPSERVICE_MAIN_FUNCTION)ServiceMain},
        {NULL, NULL}
    };

    if (StartServiceCtrlDispatcher(ServiceTable) == FALSE) {
        // Handle error
    }
#else
    pid_t pid, sid;
    pid = fork();
    if (pid < 0) { exit(EXIT_FAILURE); }
    if (pid > 0) { exit(EXIT_SUCCESS); }
    umask(0);
    sid = setsid();
    if (sid < 0) { exit(EXIT_FAILURE); }
    if ((chdir("/")) < 0) { exit(EXIT_FAILURE); }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
#endif
    start_web_server();
}

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "--daemonize") {
        run_daemon();
    } else {
        Config config = load_config();
        start_web_server();
    }

    return 0;
}

#ifdef _WIN32
VOID WINAPI ServiceMain(DWORD argc, LPTSTR* argv) {
    g_StatusHandle = RegisterServiceCtrlHandler("SysMood", ServiceCtrlHandler);
    if (g_StatusHandle == NULL) {
        return;
    }

    ZeroMemory(&g_ServiceStatus, sizeof(g_ServiceStatus));
    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwControlsAccepted = 0;
    g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwServiceSpecificExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 0;

    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

    g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (g_ServiceStopEvent == NULL) {
        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
        return;
    }

    g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(g_SvcStatusHandle, &g_ServiceStatus);

    // Start the web server in a separate thread
    std::thread web_server_thread(start_web_server);

    WaitForSingleObject(g_ServiceStopEvent, INFINITE);

    // Stop the web server (this is a bit tricky with httplib, so we'll just exit)
    web_server_thread.detach();

    CloseHandle(g_ServiceStopEvent);

    g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
}

VOID WINAPI ServiceCtrlHandler(DWORD CtrlCode) {
    switch (CtrlCode) {
    case SERVICE_CONTROL_STOP:
        if (g_ServiceStatus.dwCurrentState == SERVICE_RUNNING) {
            SetEvent(g_ServiceStopEvent);
        }
        break;
    default:
        break;
    }
}
#endif