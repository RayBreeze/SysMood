// Hi the lone wonderer here! This is main file of Sysmood program.
//
// Does what ?
// - This code is the main file of a Sysmood program.
// - It includes necessary libraries and defines the main function to get CPU and memory usage.
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
//    - this file is the main cpp file yeah the backbone or the spine you can say 🦴
//    - It can predict your system's mood by checking the CPU and memory usage 🥰.
//    - Might give you insight how to care of your system.
//    - if you find a bug, please report it to me on github
//
#include <iostream>
#include "cpu_monitor.h"
#include "memory_monitor.h"

int main() {
    Usage cpu;

    int cpu_percent = cpu.now();
    int mem_percent = memory_percent();
    int mem_available = memory_available();
    int mem_total = memory_total();
    int mem_used = memory_used();
    std::cout << 
R"(________       ___    ___ ________  _____ ______   ________  ________  ________     
|\   ____\     |\  \  /  /|\   ____\|\   _ \  _   \|\   __  \|\   __  \|\   ___ \    
\ \  \___|_    \ \  \/  / | \  \___|\ \  \\\__\ \  \ \  \|\  \ \  \|\  \ \  \_|\ \   
 \ \_____  \    \ \    / / \ \_____  \ \  \\|__| \  \ \  \\\  \ \  \\\  \ \  \ \\ \  
  \|____|\  \    \/  /  /   \|____|\  \ \  \    \ \  \ \  \\\  \ \  \\\  \ \  \_\\ \ 
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