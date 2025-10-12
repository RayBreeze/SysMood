// Hi the lone wonderer here! This is a header file for a memory monitor program.
//
// Does what ?
// - This code is a header file for a memory monitor program.
// - It includes necessary libraries and defines a function to get memory usage.
//
// Written by: Samman Das (github.com/RayBreeze)
// Written on: 2025-05-17
// License: MIT
// Version: 1.0
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the MIT License.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// MIT License for more details.
// You should have received a copy of the MIT License
// along with this program. If not, see <https://opensource.org/licenses/MIT>.
// notes:
//    - this file just access the memory which was never cleaned externally or internally maybe a little bit of dust and furs 🐶
//    - not responsible if your computer is already crying out of memory 🥲
//    - if you find a bug, please report it to me on github
//
#include <stdio.h>
#include <cstdint>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#define DIV 1024
#elif __linux__
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#define DIV 1024
#endif

class Memory {
public:
#ifdef _WIN32
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
        long long used = statusex.ullTotalPhys - statusex.ullAvailPhys;
        return used / DIV / DIV;
    }

#elif __linux__
private:
    struct MemInfo {
        long long total;
        long long free;
        long long available;
        long long buffers;
        long long cached;
    };
    
    MemInfo getMemInfo() {
        MemInfo info = {0};
        std::ifstream file("/proc/meminfo");
        std::string line;
        
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string key;
            long long value;
            std::string unit;
            
            if (iss >> key >> value >> unit) {
                if (key == "MemTotal:") info.total = value;
                else if (key == "MemFree:") info.free = value;
                else if (key == "MemAvailable:") info.available = value;
                else if (key == "Buffers:") info.buffers = value;
                else if (key == "Cached:") info.cached = value;
            }
        }
        return info;
    }

public:
    int memory_percent(){
        MemInfo info = getMemInfo();
        if (info.total == 0) return 0;
        long long used = info.total - info.available;
        return (used * 100) / info.total;
    }
    
    int memory_available(){
        MemInfo info = getMemInfo();
        return info.available / DIV; 
    }
    
    int memory_total(){
        MemInfo info = getMemInfo();
        return info.total / DIV; 
    }
    
    int memory_used(){
        MemInfo info = getMemInfo();
        return (info.total - info.available) / DIV; 
    }
#endif
};
