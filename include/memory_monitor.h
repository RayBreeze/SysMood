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
#include <windows.h>
#include <stdlib.h>
#define DIV 1024
    int memory_percent(){
        MEMORYSTATUSEX statusex;

        statusex.dwLength = sizeof(statusex);

        GlobalMemoryStatusEx(&statusex);
        long long percent = statusex.dwMemoryLoad;

        return percent;
    }
    int memory_available(){
        MEMORYSTATUSEX statusex;

        statusex.dwLength = sizeof(statusex);

        GlobalMemoryStatusEx(&statusex);
        long long available = statusex.ullAvailPhys / DIV / DIV;

        return available;
    }
    int memory_total(){
        MEMORYSTATUSEX statusex;
        statusex.dwLength = sizeof(statusex);
        GlobalMemoryStatusEx(&statusex);
        long long total = statusex.ullTotalPhys / DIV / DIV;
        return total;
    }
    int memory_used(){
        MEMORYSTATUSEX statusex;
        statusex.dwLength = sizeof(statusex);
        GlobalMemoryStatusEx(&statusex);
        long long used = statusex.ullTotalPhys - statusex.ullAvailPhys;
        return used / DIV / DIV;
    }
