#ifndef CONFIG_H
#define CONFIG_H

#include <string>

struct Config {
    int refresh_interval_ms = 5000; // Default 5 seconds
    int retention_policies_max_entries = 2000;
};

Config load_config();
void save_config(const Config& config);

#endif // CONFIG_H
