#ifndef DISK_MONITOR_H
#define DISK_MONITOR_H

#include <windows.h>
#include <string>
#include <vector>

struct DiskInfo {
    std::string mountPoint;
    ULARGE_INTEGER totalSpace;
    ULARGE_INTEGER freeSpace;
    DISK_PERFORMANCE diskPerformance;
};

std::vector<DiskInfo> getDiskInfo();

#endif // DISK_MONITOR_H
