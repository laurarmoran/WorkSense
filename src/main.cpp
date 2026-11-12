#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>
#include <thread>
#include <random>

#include "Alarm.h"
#include "DeviceManager.h"

int main()
{
    DeviceManager deviceManager;
    try
    {
        deviceManager.addDevice("SIM-TRAIN-01", "Railway Simulator", 42.5, 61.0, true);
        deviceManager.addDevice("RAILWAY-TEST-01", "Railway Test Bench", 82.0, 72.0, true);
        deviceManager.addDevice("MAINTENANCE-WS-01", "Maintentance Station", 94.0, 97.0, true);
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Error creating device: " << e.what() << '\n';
        return 1;
    }

    std::cout << "====================================\n";
    std::cout << "       WorkSense Monitoring         \n";
    std::cout << "====================================\n\n";

    // Random number generators for simulating CPU, memory usage, and disconnection events
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<int> usageDistribution(45, 99);
    std::bernoulli_distribution disconnectEvent(0.05); // 5% chance of disconnection event

    while (true)
    {
        for (const auto &device : deviceManager.getListOfDevices())
        {
            AlarmSeverity severity = evaluateAlarm(device);
            if (severity != AlarmSeverity::Normal)
                std::cout << "ALARM: " << alarmToString(severity)
                          << " for device " << device.getId() << "\n\n";

            if(!device.isConnected())
                std::cout << "ALARM: DEVICE_DISCONNECTED for device " << device.getId() << "\n\n";
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
        for (const auto &device : deviceManager.getListOfDevices())
        {
            if (!device.isConnected())
                continue;

            deviceManager.updateUsage(
                device.getId(),
                usageDistribution(generator),
                usageDistribution(generator));

            if (device.isConnected() && disconnectEvent(generator))
                deviceManager.setConnected(device.getId(), false);
        }
    }
}