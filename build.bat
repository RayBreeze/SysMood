@echo off
echo Building SysMood...
echo.

REM Clean previous build
if exist SysMood.exe del SysMood.exe

REM Compile with all necessary libraries
echo Compiling with g++...
g++ -I include -std=c++17 -Wall -O2 src/*.cpp -o SysMood.exe -lws2_32 -lpsapi -lwbemuuid -lole32 -loleaut32

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ================================
    echo Build successful! 
    echo Executable: SysMood.exe
    echo ================================
    echo.
    echo To run the application: SysMood.exe
    echo To access web dashboard: http://localhost:8080
    echo.
) else (
    echo.
    echo ================================
    echo Build failed! Check the errors above.
    echo ================================
)

pause
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
