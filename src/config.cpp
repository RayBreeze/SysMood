#include "config.h"
#include <fstream>
#include <string>
#include <iostream>

const std::string CONFIG_FILE = "config.ini";

Config load_config() {
    Config config;
    std::ifstream infile(CONFIG_FILE);
    if (!infile.good()) {
        std::cout << "config.ini not found, creating with default settings." << std::endl;
        save_config(config);
        return config;
    }

    std::string line;
    while (std::getline(infile, line)) {
        size_t delimiter_pos = line.find('=');
        if (delimiter_pos != std::string::npos) {
            std::string key = line.substr(0, delimiter_pos);
            std::string value = line.substr(delimiter_pos + 1);
            if (key == "refresh_interval_ms") {
                config.refresh_interval_ms = std::stoi(value);
            } else if (key == "retention_policies_max_entries") {
                config.retention_policies_max_entries = std::stoi(value);
            }
        }
    }
    infile.close();
    return config;
}

void save_config(const Config& config) {
    std::ofstream outfile(CONFIG_FILE);
    outfile << "refresh_interval_ms=" << config.refresh_interval_ms << std::endl;
    outfile << "retention_policies_max_entries=" << config.retention_policies_max_entries << std::endl;
    outfile.close();
}
