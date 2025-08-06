@echo off
echo Building SysMood...

REM Clean previous builds
if exist SysMood_fixed.exe del SysMood_fixed.exe

REM Compile with MinGW
echo Compiling with MinGW...
gcc -std=c++11 -O2 -Wall -Iinclude ^
    src/main.cpp ^
    src/cpu_monitor.cpp ^
    src/memory_monitor.cpp ^
    src/disk_monitor.cpp ^
    src/network_monitor.cpp ^
    src/process_monitor.cpp ^
    src/temperature_monitor.cpp ^
    src/logging.cpp ^
    src/config.cpp ^
    src/web_server.cpp ^
    -o SysMood_fixed.exe ^
    -lws2_32 -lIphlpapi -lole32 -lWbemuuid -loleaut32 -ladvapi32

if %ERRORLEVEL% EQU 0 (
    echo Build successful! Executable: SysMood_fixed.exe
) else (
    echo Build failed with error code %ERRORLEVEL%
)

pause
