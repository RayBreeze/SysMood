#include <iostream>
#include "cpu_monitor.h"
#include "memory_monitor.h"

int main() {
    std::cout << "Testing SysMood components..." << std::endl;
    
    try {
        Usage cpu;
        int cpu_usage = cpu.now();
        std::cout << "CPU Usage: " << cpu_usage << "%" << std::endl;
        
        int mem_usage = memory_percent();
        int mem_total = memory_total();
        int mem_avail = memory_available();
        int mem_used = memory_used();
        
        std::cout << "Memory Usage: " << mem_usage << "%" << std::endl;
        std::cout << "Memory Total: " << mem_total << " MB" << std::endl;
        std::cout << "Memory Available: " << mem_avail << " MB" << std::endl;
        std::cout << "Memory Used: " << mem_used << " MB" << std::endl;
        
        std::cout << "All components working correctly!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
