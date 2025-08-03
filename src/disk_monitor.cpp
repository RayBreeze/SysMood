#include "disk_monitor.h"

#ifdef _WIN32
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <memory>
#include <winioctl.h> // For DISK_PERFORMANCE, STORAGE_DEVICE_NUMBER, etc.
#include <strsafe.h> // For sprintf_s

std::vector<DiskInfo> getDiskInfo() {
    std::vector<DiskInfo> allDisksInfo;
    char buffer[256];
    DWORD bytesRead = GetLogicalDriveStringsA(sizeof(buffer) - 1, buffer);

    if (bytesRead > 0) {
        char* drive = buffer;
        while (*drive) {
            DiskInfo currentDiskInfo;
            currentDiskInfo.mountPoint = drive;

            ULARGE_INTEGER totalSpace, freeSpace;
            if (GetDiskFreeSpaceExA(drive, nullptr, &totalSpace, &freeSpace)) {
                currentDiskInfo.totalSpace = totalSpace;
                currentDiskInfo.freeSpace = freeSpace;
            } else {
                currentDiskInfo.totalSpace.QuadPart = 0;
                currentDiskInfo.freeSpace.QuadPart = 0;
            }

            // To get IOPS, we need the physical drive path
            std::string volumePath = "\\.\" + std::string(drive);
            volumePath.pop_back(); // Remove trailing backslash
            HANDLE hVolume = CreateFileA(volumePath.c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

            if (hVolume != INVALID_HANDLE_VALUE) {
                STORAGE_DEVICE_NUMBER sdn;
                DWORD bytesReturned;
                if (DeviceIoControl(hVolume, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &sdn, sizeof(sdn), &bytesReturned, NULL)) {
                    char physicalDrivePath[100];
                    StringCbPrintfA(physicalDrivePath, sizeof(physicalDrivePath), "\\.\\PhysicalDrive%lu", sdn.DeviceNumber); // Using StringCbPrintfA instead of sprintf_s
                    HANDLE hDevice = CreateFileA(physicalDrivePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

                    if (hDevice != INVALID_HANDLE_VALUE) {
                        DeviceIoControl(hDevice, IOCTL_DISK_PERFORMANCE, NULL, 0, &currentDiskInfo.diskPerformance, sizeof(DISK_PERFORMANCE), &bytesReturned, NULL);
                        CloseHandle(hDevice);
                    }
                }
                CloseHandle(hVolume);
            }

            allDisksInfo.push_back(currentDiskInfo);
            drive += strlen(drive) + 1;
        }
    }

    return allDisksInfo;
}
#else
#include <sys/statvfs.h>
#include <mntent.h>
#include <vector>

std::vector<DiskInfo> getDiskInfo() {
    std::vector<DiskInfo> allDisksInfo;
    FILE *mount_table = setmntent("/etc/mtab", "r");
    if (mount_table) {
        struct mntent *mount_entry;
        while ((mount_entry = getmntent(mount_table))) {
            struct statvfs vfs;
            if (statvfs(mount_entry->mnt_dir, &vfs) == 0) {
                DiskInfo info;
                info.mountPoint = mount_entry->mnt_dir;
                info.totalSpace = vfs.f_bsize * vfs.f_blocks;
                info.freeSpace = vfs.f_bsize * vfs.f_bfree;
                allDisksInfo.push_back(info);
            }
        }
        endmntent(mount_table);
    }
    return allDisksInfo;
}
#endif
