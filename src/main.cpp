// Hi the lone wonderer here! This is main file of Sysmood program.
//
// Does what ?
// - This code is the main file of a Sysmood program.
// - It includes necessary libraries and defines the main function to get CPU, memory, disk, and network usage.
//
// Written by: Samman Das (github.com/RayBreeze)
// Written on: 2025-05-17
// License: MIT
// Version: 1.2
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
//    - this file is the main cpp file yeah the backbone or the spine you can say 💻
//    - It can predict your system's mood by checking the CPU and memory usage 🥰.
//    - Might give you insight how to care of your system.
//    - if you find a bug, please report it to me on github
//
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include "cpu_monitor.h"
#include "memory_monitor.h"
#include "disk_monitor.h"
#include "network_monitor.h"

// Helper function to convert wstring to string for output
std::string wstringToString(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

int main() {
    Usage cpu;

    int cpu_percent = cpu.now();
    int mem_percent = memory_percent();
    int mem_available = memory_available();
    int mem_total = memory_total();
    int mem_used = memory_used();
    std::cout <<
        R"(________       ___    ___ ________  _____ ______   ________  ________  ________     
|\   ____\     |\  \  /  /|\   ____\|\   _ \  _   |\|\   __  \|\   __  \|\   ___ \    
\ \  \___|_    \ \  \/  / | \  \___|\ \  \\\__\ \  \ \  |\  \ \  |\  \ \  \_\|\ \   
 \ \_____  \    \ \    / / \ \_____  \ \  \\|__| \  \ \  \\\  \ \  \\\  \ \  \ \ \  
  \|____|\  \    \/  /  /   \|____|\  \ \  \    \ \  \ \  \\\  \ \  \\\  \ \  _\\ \ 
    ____\_\  \ __/  / /       ____\_\  \ \__\    \ \__\ \_______\ \_______\ \_______\
   |\_________\\___/ /       |\_________\|__|     \|__|\|_______|\|_______|\|_______|
   \|_________\|___|/        \|_________|                                            

)" << std::endl;

    std::cout << "Sysmood: Your system is feeling. It also has moods." << std::endl;
    std::cout << "========================System Stats======================= " << std::endl;
    std::cout << "CPU Usage: " << cpu_percent << "%" << std::endl;
    std::cout << "Memory Usage: " << mem_percent << "%" << std::endl;
    std::cout << "Memory Available: " << mem_available << " MB" << std::endl;
    std::cout << "Memory Total: " << mem_total << " MB" << std::endl;
    std::cout << "Memory Used: " << mem_used << " MB" << std::endl;

    std::cout << "========================Disk Usage========================= " << std::endl;
    std::vector<DiskInfo> disks = getDiskInfo();
    for (const auto& disk : disks) {
        double totalSpaceGB = static_cast<double>(disk.totalSpace.QuadPart) / (1024 * 1024 * 1024);
        double freeSpaceGB = static_cast<double>(disk.freeSpace.QuadPart) / (1024 * 1024 * 1024);
        std::cout << "Drive: " << disk.mountPoint << std::endl;
        std::cout << "  - Total Space: " << std::fixed << std::setprecision(2) << totalSpaceGB << " GB" << std::endl;
        std::cout << "  - Free Space: " << std::fixed << std::setprecision(2) << freeSpaceGB << " GB" << std::endl;
        std::cout << "  - Read Ops: " << disk.diskPerformance.ReadCount << std::endl;
        std::cout << "  - Write Ops: " << disk.diskPerformance.WriteCount << std::endl;
    }
    
    std::cout << "======================Network Activity======================= " << std::endl;
    std::vector<NetworkInterfaceStats> netStats = getNetworkInterfaceStats();
    for (const auto& stat : netStats) {
        std::cout << "Interface: " << wstringToString(stat.description) << std::endl;
        std::cout << "  - Bytes Sent: " << stat.bytesSent << std::endl;
        std::cout << "  - Bytes Received: " << stat.bytesReceived << std::endl;
        std::cout << "  - Packet Errors: " << stat.packetErrors << std::endl;
        std::cout << "  - Dropped Packets: " << stat.droppedPackets << std::endl;
    }

    std::cout << "----------------------Active Connections--------------------- " << std::endl;
    std::vector<NetworkConnection> connections = getActiveConnections();
    for (const auto& conn : connections) {
        std::cout << conn.protocol << " "
                  << conn.localAddress << ":" << conn.localPort << " -> "
                  << conn.remoteAddress << ":" << conn.remotePort << " "
                  << conn.state << std::endl;
    }
    std::cout << "=========================================================== " << std::endl;

    std::cout << "========================System Mood======================== " << std::endl;
    if (cpu_percent > 80) {
        std::cout << "I am getting fried here helpppppp :o" << std::endl;
    } else if (cpu_percent > 50) {
        std::cout << "Well I am working here, cool :)" << std::endl;
    } else {
        std::cout << "Yeah I am chilling here, toally chillll ;)" << std::endl;
    }
    if (mem_percent > 80) {
        std::cout << "Whoa! My brain is almost full... I'm about to forget something! " << std::endl;
    } else if (mem_percent > 50) {
        std::cout << "Memory's getting a little crowded, but I can still juggle things! " << std::endl;
    } else {
        std::cout << "So much free memory! I could host a party in here! " << std::endl;
    }
    std::cout << "=========================================================== " << std::endl;


    return 0;
}