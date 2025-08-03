// Hi the lone wonderer here! This is main file of Sysmood program.
//
// Does what ?
// - This code is the main file of a Sysmood program.
// - It includes necessary libraries and defines the main function to get CPU, memory, disk, network, temperature, and process usage.
//
// Written by: Samman Das (github.com/RayBreeze)
// Written on: 2025-05-17
// License: MIT
// Version: 1.5
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
#include <chrono>
#include "cpu_monitor.h"
#include "memory_monitor.h"
#include "disk_monitor.h"
#include "network_monitor.h"
#include "process_monitor.h"
#include "temperature_monitor.h"
#include "logging.h"

// Helper function to convert wstring to string for output
std::string wstringToString(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

void printAllStats(const SystemSnapshot& snapshot) {
    std::cout << "========================System Stats======================= " << std::endl;
    std::cout << "CPU Usage: " << snapshot.cpu_percent << "%" << std::endl;
    std::cout << "Memory Usage: " << snapshot.mem_percent << "%" << std::endl;
    // ... add other stats from snapshot if needed
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
    init_logging();

    char choice = 'r'; // Start with a refresh
    std::vector<ProcessInfo> processes;
    std::string sortKey = "pid";

    do {
        if (choice == 'r') { // Refresh data
            // 1. Gather all data
            SystemSnapshot snapshot;
            snapshot.timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            snapshot.cpu_percent = cpu.now();
            snapshot.mem_percent = memory_percent();
            processes = getProcessList();

            // 2. Log the new data
            log_stats(snapshot);
            apply_retention_policy();

            // 3. Display all stats
            std::cout << "\n\n--- Refreshed at " << snapshot.timestamp << " ---" << std::endl;
            printAllStats(snapshot);
            // ... print other stats sections (disk, temp, etc.)
        }

        if (choice == 'p') {
            sortProcesses(processes, sortKey);
            printProcesses(processes);
            std::cout << "\n[s]ort, [k]ill, [b]ack to main menu: ";
            std::cin >> choice;
            if (choice == 's') {
                std::cout << "Sort by (pid, name, memory): ";
                std::cin >> sortKey;
                choice = 'p'; // Go back to process view
            } else if (choice == 'k') {
                DWORD pidToKill;
                std::cout << "Enter PID to kill: ";
                std::cin >> pidToKill;
                terminateProcessById(pidToKill);
                choice = 'p'; // Refresh process view
            } else {
                choice = 'm'; // Go back to main menu
            }
            continue; // Loop back to process menu or main menu
        }

        if (choice == 'h') {
            display_historical_summary();
        }

        // Main Menu
        std::cout << "\n=========================Main Menu========================= " << std::endl;
        std::cout << "[r]efresh stats, [p]rocesses, [h]istory, [q]uit: ";
        std::cin >> choice;

    } while (choice != 'q');

    std::cout << "Exiting SysMood." << std::endl;
    return 0;
}