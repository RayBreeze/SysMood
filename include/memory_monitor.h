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
#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

int memory_percent();
int memory_available();
int memory_total();
int memory_used();

#endif // MEMORY_MONITOR_H