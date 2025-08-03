#ifndef DISK_MONITOR_H
#define DISK_MONITOR_H

#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
struct DiskInfo {
    std::string mountPoint;
    ULARGE_INTEGER totalSpace;
    ULARGE_INTEGER freeSpace;
    DISK_PERFORMANCE diskPerformance;
};
#else
struct DiskInfo {
    std::string mountPoint;
    unsigned long long totalSpace;
    unsigned long long freeSpace;
    // IOPS data is more complex to get on Linux and will be omitted for now.
};
#endif

std::vector<DiskInfo> getDiskInfo();

#endif // DISK_MONITOR_H