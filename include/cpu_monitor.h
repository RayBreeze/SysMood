// Hi, the lone wonderer here! This is a header file for a CPU monitor program.
//
// Does what ?
// - This code is a header file for a CPU monitor program.
// - It includes necessary libraries and defines a function to get CPU usage.
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
//    - this file is mostly chill, unless your system's on fire 🔥
//    - not responsible if your computer gets feelings 🥲
//    - if you find a bug, please report it to me on github
//
#define _WIN32_WINNT 0x0602
#include <stdio.h>
#include <cstdint>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <unistd.h>
#include <sys/time.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#endif

#ifdef _WIN32
uint64_t FromFileTime(const FILETIME &ft)
{
    ULARGE_INTEGER uli = {0};
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    return uli.QuadPart;
}
#elif __linux__
struct CpuTimes
{
    uint64_t user, nice, system, idle, iowait, irq, softirq, steal;
    uint64_t getTotalTime() const
    {
        return user + nice + system + idle + iowait + irq + softirq + steal;
    }
    uint64_t getIdleTime() const
    {
        return idle + iowait;
    }
};

struct CPUStats
{
    uint64_t ctxt;
    uint64_t btime;
    uint64_t processes;
    uint64_t procs_running;
    uint64_t procs_blocked;
};

CpuTimes getCpuTimes()
{
    std::ifstream file("/proc/stat");
    if (!file.is_open())
    {
        std::cerr << "Could not open /proc/stat" << std::endl;
        return CpuTimes{0};
    }
    std::string line;
    std::getline(file, line);

    std::istringstream ss(line);
    std::string cpu_label;
    CpuTimes times = {0};

    ss >> cpu_label >> times.user >> times.nice >> times.system >> times.idle >> times.iowait >> times.irq >> times.softirq >> times.steal;

    return times;
}

CPUStats getCPUstats()
{
    std::ifstream file("/proc/stat");
    CPUStats stats = {0};
    if (!file.is_open())
    {
        std::cerr << "Could not open /proc/stat" << std::endl;
        return stats;
    }
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        std::string key;
        ss >> key;
        if (key == "ctxt")
        {
            ss >> stats.ctxt;
        }
        else if (key == "btime")
        {
            ss >> stats.btime;
        }
        else if (key == "processes")
        {
            ss >> stats.processes;
        }
        else if (key == "procs_running")
        {
            ss >> stats.procs_running;
        }
        else if (key == "procs_blocked")
        {
            ss >> stats.procs_blocked;
        }
    }
    return stats;
}
#endif

class Processor
{
};

class Usage : public Processor
{
public:
#ifdef _WIN32
    uint64_t FromFileTime(const FILETIME &ft)
    {
        ULARGE_INTEGER uli = {0};
        uli.LowPart = ft.dwLowDateTime;
        uli.HighPart = ft.dwHighDateTime;
        return uli.QuadPart;
    }

    int now()
    {
        FILETIME i0, i1, k0, k1, u0, u1;
        GetSystemTimes(&i0, &k0, &u0);
        SleepEx(1000, false);
        GetSystemTimes(&i1, &k1, &u1);

        uint64_t idle0 = FromFileTime(i0);
        uint64_t idle1 = FromFileTime(i1);
        uint64_t kernel0 = FromFileTime(k0);
        uint64_t kernel1 = FromFileTime(k1);
        uint64_t user0 = FromFileTime(u0);
        uint64_t user1 = FromFileTime(u1);

        uint64_t idle = idle1 - idle0;
        uint64_t kernel = kernel1 - kernel0;
        uint64_t user = user1 - user0;

        double cpu = (1.0 - (double)idle / (kernel + user)) * 100.0;
        return static_cast<int>(cpu);
    }

#elif __linux__
    int now()
    {
        CpuTimes times1 = getCpuTimes();
        sleep(1);
        CpuTimes times2 = getCpuTimes();

        uint64_t idle1 = times1.getIdleTime();
        uint64_t idle2 = times2.getIdleTime();
        uint64_t total1 = times1.getTotalTime();
        uint64_t total2 = times2.getTotalTime();

        uint64_t idle_diff = idle2 - idle1;
        uint64_t total_diff = total2 - total1;

        if (total_diff == 0)
            return 0; // Avoid division by zero

        double cpu = (double)(total_diff - idle_diff) / total_diff * 100.0;
        return static_cast<int>(cpu);
    }
    CPUStats getCPUstats()
    {
        return ::getCPUstats();
    }

#endif
};
