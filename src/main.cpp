#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <exception>
#include <windows.h>
#include <Lmcons.h>
#include <tlhelp32.h>
#include <Psapi.h>

#include <random>
#include <algorithm>
#include <map>
#include <iomanip>
#include <sstream>
#include <limits>

#include "cpu_monitor.h"
#include "memory_monitor.h"
#include "disk_monitor.h"
#include "network_monitor.h"
#include "process_monitor.h"
#include "temperature_monitor.h"
#include "logging.h"
#include "web_server.h"

// Define MemoryInfo struct for main.cpp
struct MemoryInfo {
    double usage_percent;
    long long available_mb;
    long long total_mb;
    long long used_mb;
};

// Function to display ASCII art banner
void display_banner() {
    std::cout << "\n";
    std::cout << "  ____            __  __                 _ \n";
    std::cout << " / ___| _   _ ___|  \\/  | ___   ___   __| |\n";
    std::cout << " \\___ \\| | | / __| |\\/| |/ _ \\ / _ \\ / _` |\n";
    std::cout << "  ___) | |_| \\__ \\ |  | | (_) | (_) | (_| |\n";
    std::cout << " |____/ \\__, |___/_|  |_|\\___/ \\___/ \\__,_|\n";
    std::cout << "        |___/                              \n";
    std::cout << "\nSysmood: Your system is feeling. It also has moods.\n" << std::endl;
}

// Function to get system mood based on CPU and memory usage
std::string get_system_mood(int cpu_usage, int memory_usage) {
    std::string mood;
    if (cpu_usage > 80 || memory_usage > 80) {
        mood = "I'm feeling the heat! My CPU and memory are working overtime!";
    } else if (cpu_usage > 50 || memory_usage > 50) {
        mood = "I'm a bit busy, but I can handle it. Just don't open too many tabs!";
    } else {
        mood = "Yeah I am chilling here, totally chillll ;)";
    }
    return mood;
}

// Function to get memory mood
std::string get_memory_mood(int memory_usage) {
    std::string mood;
    if (memory_usage > 90) {
        mood = "I'm suffocating! Close some applications, please!";
    } else if (memory_usage > 70) {
        mood = "My memory is getting full. I might slow down soon.";
    } else if (memory_usage > 40) {
        mood = "I'm comfortable with my current memory usage.";
    } else {
        mood = "So much free memory! I could host a party in here!";
    }
    return mood;
}

int main() {
    try {
        display_banner();
        std::cout << "========================System Stats=======================" << std::endl;

        // Initialize logging
        init_logging();

        // Initial data collection with error handling
        Usage cpu_usage_monitor;
        int cpu_usage = 0;
        MemoryInfo mem_info = {0};
        
        try {
            cpu_usage = cpu_usage_monitor.now();
        } catch (const std::exception& e) {
            std::cout << "Warning: Could not get CPU usage: " << e.what() << std::endl;
        }
        
        try {
            mem_info.usage_percent = memory_percent();
            mem_info.available_mb = memory_available();
            mem_info.total_mb = memory_total();
            mem_info.used_mb = memory_used();
        } catch (const std::exception& e) {
            std::cout << "Warning: Could not get memory info: " << e.what() << std::endl;
        }

        std::cout << "CPU Usage: " << cpu_usage << "%" << std::endl;
        std::cout << "Memory Usage: " << (int)mem_info.usage_percent << "%" << std::endl;
        std::cout << "Memory Available: " << mem_info.available_mb << " MB" << std::endl;
        std::cout << "Memory Total: " << mem_info.total_mb << " MB" << std::endl;
        std::cout << "Memory Used: " << mem_info.used_mb << " MB" << std::endl;
        std::cout << "===========================================================" << std::endl;

        std::cout << "========================System Mood=======================" << std::endl;
        std::cout << get_system_mood(cpu_usage, (int)mem_info.usage_percent) << std::endl;
        std::cout << get_memory_mood((int)mem_info.usage_percent) << std::endl;
        std::cout << "===========================================================" << std::endl;

        // Start web server in a separate thread with error handling
        try {
            std::thread web_server_thread(start_web_server);
            web_server_thread.detach(); // Detach the thread so it runs independently
            std::cout << "Web server started at http://localhost:8080" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Warning: Could not start web server: " << e.what() << std::endl;
        }

        // Main loop for console application
        while (true) {
            try {
                // Collect and log data every 5 seconds
                log_system_data();
            } catch (const std::exception& e) {
                std::cout << "Warning: Could not log system data: " << e.what() << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}