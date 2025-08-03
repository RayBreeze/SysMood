// Hi the lone wonderer here! This is main file of Sysmood program.
//
// Does what ?
// - This code is the main file of a Sysmood program.
// - It includes necessary libraries and defines the main function to get CPU, memory, disk, network, and process usage.
//
// Written by: Samman Das (github.com/RayBreeze)
// Written on: 2025-05-17
// License: MIT
// Version: 1.3
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
#include <algorithm>
#include "cpu_monitor.h"
#include "memory_monitor.h"
#include "disk_monitor.h"
#include "network_monitor.h"
#include "process_monitor.h"

// Helper function to convert wstring to string for output
std::string wstringToString(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

void printProcesses(const std::vector<ProcessInfo>& processes) {
    std::cout << std::left << std::setw(10) << "PID" 
              << std::setw(40) << "Name" 
              << std::setw(20) << "Memory (MB)" 
              << "User" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    for (const auto& p : processes) {
        std::cout << std::left << std::setw(10) << p.pid
                  << std::setw(40) << wstringToString(p.name)
                  << std::setw(20) << (p.memoryUsage / 1024 / 1024)
                  << wstringToString(p.user) << std::endl;
    }
}

int main() {
    Usage cpu;

    // Initial system stats display
    int cpu_percent = cpu.now();
    int mem_percent = memory_percent();
    int mem_available = memory_available();
    int mem_total = memory_total();
    int mem_used = memory_used();
    std::cout <<
        R"(________       ___    ___ ________  _____ ______   ________  ________  ________     
|\   ____\     |\  \  /  /|\   ____\|\   _ \  _   \|\   __  \|\   __  \|\   ___ \    
\ \  \___|_    \ \  \/  / | \  \___|\ \  \\\__\ \  \ \  |\  \ \  |\  \ \  \_\ \   
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

    // ... (rest of the initial stats display)

    // Interactive Process Management
    std::vector<ProcessInfo> processes = getProcessList();
    std::string sortKey = "pid"; // Default sort

    char choice;
    do {
        std::cout << "\n======================Process Management======================= " << std::endl;
        sortProcesses(processes, sortKey);
        printProcesses(processes);
        std::cout << "\n[l]ist/refresh, [s]ort, [k]ill, [q]uit: ";
        std::cin >> choice;

        switch (choice) {
            case 'l':
                processes = getProcessList();
                break;
            case 's':
                std::cout << "Sort by (pid, name, memory): ";
                std::cin >> sortKey;
                break;
            case 'k': {
                DWORD pidToKill;
                std::cout << "Enter PID to kill: ";
                std::cin >> pidToKill;
                if (terminateProcessById(pidToKill)) {
                    std::cout << "Process " << pidToKill << " terminated." << std::endl;
                } else {
                    std::cout << "Failed to terminate process " << pidToKill << "." << std::endl;
                }
                processes = getProcessList(); // Refresh list
                break;
            }
            case 'q':
                std::cout << "Exiting SysMood." << std::endl;
                break;
            default:
                std::cout << "Invalid choice." << std::endl;
                break;
        }
    } while (choice != 'q');

    return 0;
}