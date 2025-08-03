#include "logging.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <numeric>
#include <sstream> // Added for std::stringstream

const std::string LOG_FILE = "sysmood_log.csv";

void init_logging() {
    std::ifstream file(LOG_FILE);
    if (!file.good()) {
        std::ofstream outfile(LOG_FILE);
        outfile << "Timestamp,CPU_Usage_Percent,Memory_Usage_Percent" << std::endl;
        outfile.close();
    }
}

void log_stats(const SystemSnapshot& snapshot) {
    std::ofstream outfile(LOG_FILE, std::ios_base::app);
    outfile << snapshot.timestamp << ","
            << snapshot.cpu_percent << ","
            << snapshot.mem_percent << std::endl;
    outfile.close();
}

void apply_retention_policy(int max_entries) {
    std::vector<std::string> lines;
    std::string line;
    std::ifstream infile(LOG_FILE);

    while (std::getline(infile, line)) {
        lines.push_back(line);
    }
    infile.close();

    if (lines.size() > max_entries + 1) { // +1 for the header
        std::ofstream outfile(LOG_FILE, std::ios_base::trunc);
        outfile << lines[0] << std::endl; // Write header
        for (size_t i = lines.size() - max_entries; i < lines.size(); ++i) {
            outfile << lines[i] << std::endl;
        }
        outfile.close();
    }
}

void display_historical_summary() {
    std::vector<int> cpu_history;
    std::vector<int> mem_history;
    std::string line;
    std::ifstream infile(LOG_FILE);

    std::getline(infile, line); // Skip header

    while (std::getline(infile, line)) {
        std::stringstream ss(line);
        std::string item;
        std::getline(ss, item, ','); // Skip timestamp
        std::getline(ss, item, ',');
        cpu_history.push_back(std::stoi(item));
        std::getline(ss, item, ',');
        mem_history.push_back(std::stoi(item));
    }
    infile.close();

    if (cpu_history.empty()) {
        std::cout << "No historical data to display." << std::endl;
        return;
    }

    double avg_cpu = std::accumulate(cpu_history.begin(), cpu_history.end(), 0.0) / cpu_history.size();
    double avg_mem = std::accumulate(mem_history.begin(), mem_history.end(), 0.0) / mem_history.size();

    std::cout << "====================Historical Averages==================== " << std::endl;
    std::cout << "Average CPU Usage: " << std::fixed << std::setprecision(2) << avg_cpu << "%" << std::endl;
    std::cout << "Average Memory Usage: " << std::fixed << std::setprecision(2) << avg_mem << "%" << std::endl;
    std::cout << "(Based on the last " << cpu_history.size() << " entries)" << std::endl;
    std::cout << "For detailed graphs, please open sysmood_log.csv in a spreadsheet program." << std::endl;
}

// Definition for log_system_data
void log_system_data() {
    // This function needs to collect current system data and log it.
    // For now, we'll just log dummy data or integrate with existing monitors.
    // This will be properly implemented once other monitors are working.
    SystemSnapshot snapshot;
    snapshot.timestamp = std::chrono::duration_cast<std::chrono::seconds>(
                             std::chrono::system_clock::now().time_since_epoch()).count();
    // Placeholder values
    snapshot.cpu_percent = 0; 
    snapshot.mem_percent = 0;

    log_stats(snapshot);
    apply_retention_policy();
}
