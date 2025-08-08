#include "cpu_monitor.h"

#ifdef _WIN32
#include <windows.h>
#include <random>

Usage::Usage() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    numProcessors = sysInfo.dwNumberOfProcessors;

    // Initialize previous times
    FILETIME idleTime, kernelTime, userTime;
    GetSystemTimes(&idleTime, &kernelTime, &userTime);
    prevIdleTime = idleTime;
    prevKernelTime = kernelTime;
    prevUserTime = userTime;
}

int Usage::now() {
    FILETIME idleTime, kernelTime, userTime;
    if (!GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        // If unable to get times, return -1 or a suitable error value
        return -1;
    }

    // Convert FILETIME to ULONGLONG
    ULONGLONG idle = ((ULONGLONG)idleTime.dwHighDateTime << 32) | idleTime.dwLowDateTime;
    ULONGLONG kernel = ((ULONGLONG)kernelTime.dwHighDateTime << 32) | kernelTime.dwLowDateTime;
    ULONGLONG user = ((ULONGLONG)userTime.dwHighDateTime << 32) | userTime.dwLowDateTime;

    ULONGLONG prevIdle = ((ULONGLONG)prevIdleTime.dwHighDateTime << 32) | prevIdleTime.dwLowDateTime;
    ULONGLONG prevKernel = ((ULONGLONG)prevKernelTime.dwHighDateTime << 32) | prevKernelTime.dwLowDateTime;
    ULONGLONG prevUser = ((ULONGLONG)prevUserTime.dwHighDateTime << 32) | prevUserTime.dwLowDateTime;

    ULONGLONG sysIdleDiff = idle - prevIdle;
    ULONGLONG sysKernelDiff = kernel - prevKernel;
    ULONGLONG sysUserDiff = user - prevUser;

    ULONGLONG sysTotal = sysKernelDiff + sysUserDiff;
    int cpuUsage = 0;
    if (sysTotal > 0) {
        cpuUsage = (int)(100.0 * (sysTotal - sysIdleDiff) / sysTotal);
    }

    // Update previous times
    prevIdleTime = idleTime;
    prevKernelTime = kernelTime;
    prevUserTime = userTime;

    return cpuUsage;
}

#else
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

Usage::Usage() {
    std::ifstream file("/proc/stat");
    std::string line;
    std::getline(file, line);
    std::stringstream ss(line);
    std::string cpu;
    ss >> cpu >> lastTotalUser >> lastTotalUserLow >> lastTotalSys >> lastTotalIdle;
}

int Usage::now() {
    unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

    std::ifstream file("/proc/stat");
    std::string line;
    std::getline(file, line);
    std::stringstream ss(line);
    std::string cpu;
    ss >> cpu >> totalUser >> totalUserLow >> totalSys >> totalIdle;

    double total_diff = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) + (totalSys - lastTotalSys);
    double idle_diff = totalIdle - lastTotalIdle;
    double total_usage = total_diff + idle_diff;

    lastTotalUser = totalUser;
    lastTotalUserLow = totalUserLow;
    lastTotalSys = totalSys;
    lastTotalIdle = totalIdle;

    return (int)((total_diff / total_usage) * 100.0);
}

#endif
