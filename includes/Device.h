#pragma once

#include <string>

class Device
{
public:
    const std::string id;
    const std::string name;
    double cpuUsage;
    double memoryUsage;
    bool connected;

    Device(const std::string &id, const std::string &name, double cpuUsage, 
        double memoryUsage, bool connected);
};
