#include "memory_monitor.h"

#ifdef _WIN32
#include <windows.h>
#define DIV 1024

int memory_percent() {
    MEMORYSTATUSEX statusex;
    statusex.dwLength = sizeof(statusex);
    GlobalMemoryStatusEx(&statusex);
    return statusex.dwMemoryLoad;
}

int memory_available() {
    MEMORYSTATUSEX statusex;
    statusex.dwLength = sizeof(statusex);
    GlobalMemoryStatusEx(&statusex);
    return statusex.ullAvailPhys / DIV / DIV;
}

int memory_total() {
    MEMORYSTATUSEX statusex;
    statusex.dwLength = sizeof(statusex);
    GlobalMemoryStatusEx(&statusex);
    return statusex.ullTotalPhys / DIV / DIV;
}

int memory_used() {
    MEMORYSTATUSEX statusex;
    statusex.dwLength = sizeof(statusex);
    GlobalMemoryStatusEx(&statusex);
    return (statusex.ullTotalPhys - statusex.ullAvailPhys) / DIV / DIV;
}

#else
#include <fstream>
#include <string>
#include <map>

std::map<std::string, long> get_mem_info() {
    std::map<std::string, long> mem_info;
    std::ifstream file("/proc/meminfo");
    std::string line;
    while (std::getline(file, line)) {
        size_t sep = line.find(':');
        std::string key = line.substr(0, sep);
        long value = std::stol(line.substr(sep + 1));
        mem_info[key] = value;
    }
    return mem_info;
}

int memory_total() {
    return get_mem_info()["MemTotal"] / 1024;
}

int memory_available() {
    return get_mem_info()["MemAvailable"] / 1024;
}

int memory_used() {
    return memory_total() - memory_available();
}

int memory_percent() {
    return (int)(((double)memory_used() / memory_total()) * 100.0);
}

#endif
