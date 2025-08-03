#ifndef DISK_MONITOR_H
#define DISK_MONITOR_H

#include <string>
#include <vector>
#include <windows.h> // For ULARGE_INTEGER
#include <winioctl.h> // For DISK_PERFORMANCE, STORAGE_DEVICE_NUMBER, etc.

struct DiskInfo {
    std::string mountPoint;
    ULARGE_INTEGER totalSpace;
    ULARGE_INTEGER freeSpace;
#ifdef _WIN32
    DISK_PERFORMANCE diskPerformance;
#endif
};

std::vector<DiskInfo> getDiskInfo();

#endif // DISK_MONITOR_H