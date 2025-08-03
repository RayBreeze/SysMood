#ifndef PROCESS_MONITOR_H
#define PROCESS_MONITOR_H

#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
struct ProcessInfo {
    DWORD pid;
    std::wstring name;
    double cpuUsage; // Note: CPU usage calculation for a single snapshot is non-trivial.
    SIZE_T memoryUsage; // in bytes
    std::wstring user;
};
#else
#include <string>
struct ProcessInfo {
    int pid;
    std::string name;
    double cpuUsage; // Placeholder
    long memoryUsage; // in KB
    std::string user;
};
#endif

std::vector<ProcessInfo> getProcessList();
void sortProcesses(std::vector<ProcessInfo>& processes, const std::string& sortBy);
bool terminateProcessById(int pid);

#endif // PROCESS_MONITOR_H