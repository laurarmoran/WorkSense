#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

enum class AlarmSeverity
{
    Normal,
    Warning,
    Critical
};

struct Device
{
    std::string id;
    std::string name;
    double cpuUsage;
    double memoryUsage;
    bool connected;
};

AlarmSeverity evaluateAlarm(const Device &device)
{
    if (device.cpuUsage >= 90.0 || device.memoryUsage >= 95.0)
        return AlarmSeverity::Critical;

    if (device.cpuUsage >= 75.0 || device.memoryUsage >= 80.0)
        return AlarmSeverity::Warning;

    return AlarmSeverity::Normal;
}

std::string alarmToString(AlarmSeverity severity)
{
    switch (severity)
    {
    case AlarmSeverity::Normal:
        return "NORMAL";
    case AlarmSeverity::Warning:
        return "WARNING";
    case AlarmSeverity::Critical:
        return "CRITICAL";
    }
    return "UNKNOWN";
}

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