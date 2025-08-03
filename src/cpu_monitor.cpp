#include "cpu_monitor.h"

#ifdef _WIN32
#include <windows.h>

Usage::Usage() {
    SYSTEM_INFO sysInfo;
    FILETIME ftime, fsys, fuser;

    GetSystemInfo(&sysInfo);
    numProcessors = sysInfo.dwNumberOfProcessors;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&lastCPU, &ftime, sizeof(FILETIME));

    self = GetCurrentProcess();
    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
    memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
}

int Usage::now() {
    FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;
    double percent;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));

    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));

    percent = (double)(sys.QuadPart - lastSysCPU.QuadPart) +
              (double)(user.QuadPart - lastUserCPU.QuadPart);
    percent /= (now.QuadPart - lastCPU.QuadPart);
    percent /= numProcessors;
    lastCPU = now;
    lastUserCPU = user;
    lastSysCPU = sys;

    return (int)(percent * 100);
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
