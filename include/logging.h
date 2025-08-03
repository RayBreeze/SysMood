#ifndef LOGGING_H
#define LOGGING_H

#include <string>
#include "cpu_monitor.h"
#include "memory_monitor.h"
// #include "disk_monitor.h"
// #include "network_monitor.h"

struct SystemSnapshot {
    long long timestamp;
    int cpu_percent;
    int mem_percent;
    // Add other metrics as needed
};

void init_logging();
void log_stats(const SystemSnapshot& snapshot);
void apply_retention_policy(int max_entries = 2000); // Default to 2000 lines
void display_historical_summary();
void log_system_data(); // Added declaration

#endif // LOGGING_H
