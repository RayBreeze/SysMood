#include "web_server.h"
#include "httplib.h"
#include "cpu_monitor.h"
#include "memory_monitor.h"
#include "disk_monitor.h"
#include "network_monitor.h"
#include "process_monitor.h"
#include "temperature_monitor.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#endif

using std::string;

// Helper to convert wstring to string
std::string wstring_to_string(const std::wstring& wstr) {
    if (wstr.empty()) return "";
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

void start_web_server() {
    httplib::Server svr;

    // Serve the dashboard HTML
    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        std::ifstream file("dashboard.html");
        std::stringstream buffer;
        buffer << file.rdbuf();
        res.set_content(buffer.str(), "text/html");
    });

    // API endpoint to get system stats
    svr.Get("/api/stats", [](const httplib::Request&, httplib::Response& res) {
        Usage cpu;
        std::stringstream json_response;
        
        // Build JSON response manually
        json_response << "{";
        json_response << "\"cpu_usage\":" << cpu.now() << ",";
        json_response << "\"memory_usage\":" << memory_percent() << ",";
        json_response << "\"memory_available\":" << memory_available() << ",";
        json_response << "\"memory_total\":" << memory_total() << ",";
        json_response << "\"memory_used\":" << memory_used() << ",";

        // Disk Info
        json_response << "\"disk_info\":[";
        std::vector<DiskInfo> diskInfo = getDiskInfo();
        for (size_t i = 0; i < diskInfo.size(); ++i) {
            const auto& disk = diskInfo[i];
            json_response << "{";
            json_response << "\"mount_point\":\"" << disk.mountPoint << "\",";
            json_response << "\"total_space_gb\":" << (disk.totalSpace.QuadPart / (1024.0 * 1024.0 * 1024.0)) << ",";
            json_response << "\"free_space_gb\":" << (disk.freeSpace.QuadPart / (1024.0 * 1024.0 * 1024.0));
            json_response << "}";
            if (i < diskInfo.size() - 1) json_response << ",";
        }
        json_response << "],";

        // Temperature Info
        json_response << "\"temperature_info\":[";
        std::vector<TemperatureInfo> tempInfo = getTemperatureInfo();
        for (size_t i = 0; i < tempInfo.size(); ++i) {
            const auto& temp = tempInfo[i];
            json_response << "{";
            json_response << "\"sensor_name\":\"" << wstring_to_string(temp.sensorName) << "\",";
            json_response << "\"temperature\":" << temp.currentTemperature << ",";
            json_response << "\"unit\":\"" << wstring_to_string(temp.unit) << "\"";
            json_response << "}";
            if (i < tempInfo.size() - 1) json_response << ",";
        }
        json_response << "]";
        json_response << "}";

        res.set_content(json_response.str(), "application/json");
    });

    // Prometheus metrics endpoint
    svr.Get("/metrics", [](const httplib::Request&, httplib::Response& res) {
        Usage cpu;
        std::stringstream ss;
        ss << "# HELP sysmood_cpu_usage_percent CPU usage percentage.\n";
        ss << "# TYPE sysmood_cpu_usage_percent gauge\n";
        ss << "sysmood_cpu_usage_percent " << cpu.now() << "\n";

        ss << "# HELP sysmood_memory_usage_percent Memory usage percentage.\n";
        ss << "# TYPE sysmood_memory_usage_percent gauge\n";
        ss << "sysmood_memory_usage_percent " << memory_percent() << "\n";

        // Add other metrics here...

        res.set_content(ss.str(), "text/plain; version=0.0.4");
    });

    std::cout << "Starting web server at http://localhost:8080" << std::endl;
    svr.listen("localhost", 8080);
}