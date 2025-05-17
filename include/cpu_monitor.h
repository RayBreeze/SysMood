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
#include <windows.h>
#include <stdlib.h>
uint64_t FromFileTime( const FILETIME& ft ) {
    ULARGE_INTEGER uli = { 0 };
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    return uli.QuadPart;
}

    class Processor{};

    class Usage: public Processor
    {
        public:

        int now(){
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
            uint64_t total = (kernel + user) - idle;

            double cpu = (1.0 - (double)idle / (kernel + user)) * 100.0;

            return static_cast<int>(cpu);

        }

    };
