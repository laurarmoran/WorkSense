#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#include "Alarm.h"

int main()
{
    std::vector<Device> devices = {
        {"SIM-TRAIN-01", "Railway Simulator", 42.5, 61.0, true},
        {"RAILWAY-TEST-01", "Railway Test Bench", 82.0, 72.0, true},
        {"MAINTENANCE-WS-01", "Maintentance Station", 94.0, 97.0, true}};

    std::cout << "====================================\n";
    std::cout << "       WorkSense Monitoring         \n";
    std::cout << "====================================\n\n";

    for (const auto &device : devices)
    {
        std::cout << "Device: " << device.id << '\n';
        std::cout << "Name: " << device.name << '\n';
        std::cout << "Connection: "
                  << (device.connected ? "ONLINE" : "OFFLINE")
                  << '\n';

        std::cout << std::fixed << std::setprecision(1);
        std::cout << "CPU: " << device.cpuUsage << "%\n";
        std::cout << "Memory: " << device.memoryUsage << "%\n";

        std::cout << "Alarm: "
                  << alarmToString(evaluateAlarm(device))
                  << "\n\n";
    }
    return 0;
}