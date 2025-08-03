# SysMood

Hey there, the lone wanderer here!
**SysMood** is a comprehensive system monitoring tool for Windows, designed to provide real-time insights into your system's performance and health. Beyond just raw data, SysMood offers playful, sometimes sassy feedback based on your system's "mood," complete with an ASCII art entrance.

---

## Features

SysMood offers a wide range of monitoring capabilities and features:

### 1. Core System Monitoring

*   **CPU Usage:** Measures how hard your CPU is working, providing real-time percentage utilization.
*   **Memory Usage:** Detailed breakdown of memory usage, including total, available, and used memory in MB, along with percentage utilization.
*   **Disk Information:** Provides insights into your disk drives, including mount points, total space, and free space. (Note: IOPS monitoring is implemented but may require specific compiler/environment setup for full functionality).
*   **Network Statistics:** Monitors network interface statistics (bytes sent/received, packet errors, dropped packets) and lists active network connections (TCP).
*   **Process Monitoring:** Lists running processes, including their IDs, names, and memory usage.
*   **Temperature Monitoring:** (Windows-specific) Attempts to retrieve system component temperatures, providing crucial thermal insights.

### 2. System Mood & Feedback

*   **System Mood:** Generates playful and descriptive "moods" for your system based on overall CPU and memory load.
*   **Memory Mood:** Provides specific feedback on memory utilization, ranging from "chilling" to "suffocating."
*   **ASCII Art Banner:** A cool ASCII art display greets you upon application startup.

### 3. Data Logging & Web Interface

*   **Historical Logging:** Logs system performance data (CPU and memory usage) to a CSV file (`sysmood_log.csv`) for historical analysis.
*   **Retention Policy:** Automatically manages the log file size, retaining a configurable number of recent entries.
*   **Historical Summary:** Displays average CPU and memory usage based on logged historical data.
*   **Web Server:** (Experimental) Includes a built-in web server that can potentially expose system metrics via a web interface.

---

## How to Use

### 1. Clone This Repo

```sh
git clone https://github.com/RayBreeze/SysMood.git
cd SysMood
```

### 2. Build It (Recommended: Visual Studio on Windows)

SysMood is primarily developed and tested with Microsoft Visual C++ (MSVC) on Windows due to its reliance on Windows-specific APIs.

**Using Visual Studio IDE:**

1.  **Open Visual Studio:** Launch Visual Studio (Community Edition is free).
2.  **Open Folder:** Go to `File` > `Open` > `Folder...` and select the `SysMood` project directory.
3.  **Build:** Go to `Build` > `Build Solution` (or `Build` > `Build All`). Visual Studio will handle the compilation and linking of necessary libraries.

**Using Developer Command Prompt for Visual Studio:**

1.  **Open Developer Command Prompt:** Search for "Developer Command Prompt for VS [Your Visual Studio Version]" in your Windows Start Menu and open it.
2.  **Navigate to Project:**
    ```cmd
    cd C:\Users\ABHINAV KUMAR\Desktop\SysMood
    ```
3.  **Compile:**
    ```cmd
    cl.exe /EHsc /Zi /FeSysMood.exe /Iinclude src\*.cpp ws2_32.lib Iphlpapi.lib ole32.lib Wbemuuid.lib oleaut32.lib advapi32.lib
    ```

### 3. Run It

After successful compilation, run the executable:

```sh
.\SysMood.exe
```

---

## What You’ll See

```
________       ___    ___ ________  _____ ______   ________  ________  ________     
|\\   ____\\     |\\  \\  /  /|\\   ____\\|\\   _ \\  _   \\|\\   __  \\|\\   __  \\|\\   ___ \\    
\\ \\  \___|_    \\ \\  \\/  / | \\  \___|\\ \\  \\\\\\__\\ \\  \\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\|\\  \\_|\\ \\   
 \\ \\_____  \\    \\ \\    / / \\ \\_____  \\ \\  \\\\|__| \\ \\  \\ \\  \\\\\\  \\ \\ \\  \\\\\\  \\ \\ \\  \\ \\  
  \\|____|\\  \\    \\/  /  /   \\|____|\\  \\ \\  \\    \\ \\  \\ \\  \\\\\\  \\ \\ \\  \\\\\\  \\ \\ \\  \\_\\ \\ 
    ____\\_\\  \\ __/  / /       ____\\_\\  \\ \\__\\    \\ \\__\\ \\_______\\ \\_______\\ \\_______\\
   |\\_________\\\\\\___/ /       |\\_________\\\\|__|     \\|__|\\|_______|\\|_______|\\|_______|\\
   \\|_________\\\\|___|/        \\|_________|                                            

Sysmood: Your system is feeling. It also has moods.
========================System Stats======================= 
CPU Usage: 12%
Memory Usage: 45%
Memory Available: 8000 MB
Memory Total: 16000 MB
Memory Used: 7200 MB
=========================================================== 
========================System Mood======================== 
Yeah I am chilling here, totally chillll ;)
So much free memory! I could host a party in here! 
=========================================================== 
```

---

## Wanna Tweak It?

*   **Change the ASCII Art:** Go wild in `main.cpp`—make it yours!
*   **Edit the Mood Messages:** The mood lines for CPU and memory are all in `main.cpp`. Make them as weird or wise as you want.
*   **Adjust Logging:** Modify `src/logging.cpp` to change logging behavior, retention policies, or add more metrics to the log.
*   **Web Server Customization:** Explore `src/web_server.cpp` to customize the web interface and exposed metrics.

---

## What You Need

*   Windows Operating System
*   C++17 or newer compiler (MSVC recommended for full functionality)

---

## License

MIT License. See [LICENSE](LICENSE) for the legal stuff.

---

## Who Made This?

Made with ☕ and curiosity by [Samman Das (RayBreeze)](https://github.com/RayBreeze)

---

## Found a Bug? Want to Help?

Open an issue or PR! We'd love to see what you come up with.