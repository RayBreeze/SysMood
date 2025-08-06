# SysMood - System Monitoring Tool

## Compilation Fixes Applied

### Issues Fixed:
1. **Type mismatch errors**: Fixed inconsistent return types between headers and implementations
2. **CPU Monitor**: Simplified CPU monitoring with placeholder values (working memory monitoring)
3. **Function signatures**: Updated function parameters to match usage
4. **Missing includes**: Added necessary Windows API includes
5. **Logging system**: Updated to collect real system data
6. **Web server**: Added proper Windows header includes

### How to Build:
```bash
# Option 1: Use the build script
build.bat

# Option 2: Manual compilation
g++ -I include -std=c++17 -Wall -O2 src/*.cpp -o SysMood.exe -lws2_32 -lpsapi -lwbemuuid -lole32 -loleaut32
```

### How to Run:
```bash
SysMood.exe
```

### Features Working:
- ✅ Memory monitoring (usage, total, available, used)
- ✅ System mood messages based on resource usage
- ✅ Web server on http://localhost:8080
- ✅ Data logging to CSV
- ✅ Console output with system stats
- ⚠️ CPU monitoring (placeholder values for now)
- ✅ Disk monitoring
- ✅ Temperature monitoring (Windows WMI)

### Next Steps:
- Implement proper CPU monitoring using Performance Counters
- Add network monitoring functionality
- Enhance process monitoring
- Improve error handling

### Dependencies:
- MinGW/GCC with C++17 support
- Windows SDK libraries (included in compilation)
- httplib.h (included)
- nlohmann/json.hpp (included)

The application now compiles and runs successfully with working memory monitoring and system mood detection!
