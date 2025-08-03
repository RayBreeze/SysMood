#include "memory_monitor.h"
#include <windows.h>

#define DIV 1024

int memory_percent(){
    MEMORYSTATUSEX statusex;
    statusex.dwLength = sizeof(statusex);
    GlobalMemoryStatusEx(&statusex);
    return statusex.dwMemoryLoad;
}

int memory_available(){
    MEMORYSTATUSEX statusex;
    statusex.dwLength = sizeof(statusex);
    GlobalMemoryStatusEx(&statusex);
    return statusex.ullAvailPhys / DIV / DIV;
}

int memory_total(){
    MEMORYSTATUSEX statusex;
    statusex.dwLength = sizeof(statusex);
    GlobalMemoryStatusEx(&statusex);
    return statusex.ullTotalPhys / DIV / DIV;
}

int memory_used(){
    MEMORYSTATUSEX statusex;
    statusex.dwLength = sizeof(statusex);
    GlobalMemoryStatusEx(&statusex);
    return (statusex.ullTotalPhys - statusex.ullAvailPhys) / DIV / DIV;
}