#ifndef PROCESS_MONITOR_H
#define PROCESS_MONITOR_H

#include <string>
#include <vector>
#include <windows.h>

struct ProcessInfo {
    DWORD pid;
    std::wstring name;
    double cpuUsage; // Note: CPU usage calculation for a single snapshot is non-trivial.
    SIZE_T memoryUsage; // in bytes
    std::wstring user;
};

std::vector<ProcessInfo> getProcessList();
void sortProcesses(std::vector<ProcessInfo>& processes, const std::string& sortBy);
bool terminateProcessById(DWORD pid);

#endif // PROCESS_MONITOR_H
