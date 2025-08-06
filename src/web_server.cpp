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
#include <nlohmann/json.hpp> // For JSON serialization

using json = nlohmann::json;

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
        json stats;
        stats["cpu_usage"] = cpu.now();
        stats["memory_usage"] = memory_percent();
        stats["memory_available"] = memory_available();
        stats["memory_total"] = memory_total();
        stats["memory_used"] = memory_used();

        // Disk Info
        json disk_info_json = json::array();
        for (const auto& disk : getDiskInfo()) {
            json d;
            d["mount_point"] = disk.mountPoint;
            d["total_space_gb"] = disk.totalSpace.QuadPart / (1024.0 * 1024.0 * 1024.0);
            d["free_space_gb"] = disk.freeSpace.QuadPart / (1024.0 * 1024.0 * 1024.0);
            disk_info_json.push_back(d);
        }
        stats["disk_info"] = disk_info_json;

        // Temperature Info
        json temp_info_json = json::array();
        for (const auto& temp : getTemperatureInfo()) {
            json t;
            t["sensor_name"] = wstring_to_string(temp.sensorName);
            t["temperature"] = temp.currentTemperature;
            t["unit"] = wstring_to_string(temp.unit);
            temp_info_json.push_back(t);
        }
        stats["temperature_info"] = temp_info_json;

        res.set_content(stats.dump(), "application/json");
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