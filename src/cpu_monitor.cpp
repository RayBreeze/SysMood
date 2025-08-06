#include "cpu_monitor.h"

#ifdef _WIN32
#include <windows.h>
#include <random>

Usage::Usage() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    numProcessors = sysInfo.dwNumberOfProcessors;
}

int Usage::now() {
    // For now, return a random CPU usage between 5-25% as a placeholder
    // This is a temporary solution until we can implement proper CPU monitoring
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(5, 25);
    
    return dis(gen);
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
