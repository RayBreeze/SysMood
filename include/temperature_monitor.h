#ifndef TEMPERATURE_MONITOR_H
#define TEMPERATURE_MONITOR_H

#include <string>
#include <vector>

struct TemperatureInfo {
    std::wstring sensorName;
    float currentTemperature;
    std::wstring unit;
};

std::vector<TemperatureInfo> getTemperatureInfo();

#endif // TEMPERATURE_MONITOR_H
