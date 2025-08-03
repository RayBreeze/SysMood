#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H

#ifdef _WIN32
#include <windows.h>
#endif

class Usage {
public:
    Usage();
    int now();

private:
#ifdef _WIN32
    ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
    int numProcessors;
    HANDLE self;
#else
    unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;
#endif
};

#endif // CPU_MONITOR_H