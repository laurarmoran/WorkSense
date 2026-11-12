#include <chrono>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <thread>

#include "Alarm.h"
#include "DeviceManager.h"

int main()
{
    DeviceManager deviceManager;
    deviceManager.setAlarmHandler([](const std::string &deviceId, AlarmSeverity severity)
    {
        std::cout << "ALARM: " << alarmToString(severity)
                  << " for device " << deviceId << '\n'
                  << std::flush;
    });

    try
    {
        deviceManager.addDevice("SIM-TRAIN-01", "Railway Simulator", 42.5, 61.0, true);
        deviceManager.addDevice("RAILWAY-TEST-01", "Railway Test Bench", 82.0, 72.0, true);
        deviceManager.addDevice("MAINTENANCE-WS-01", "Maintenance Station", 94.0, 97.0, true);
    }
    catch (const std::invalid_argument &error)
    {
        std::cerr << "Error creating device: " << error.what() << '\n';
        return 1;
    }

    std::thread simulationThread([&deviceManager]()
    {
        std::random_device randomDevice;
        std::mt19937 generator(randomDevice());
        std::uniform_int_distribution<int> usageDistribution(5, 100);
        std::bernoulli_distribution disconnectEvent(0.05);

        while (true)
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            for (const auto &device : deviceManager.getListOfDevices())
            {
                if (!device.isConnected())
                    continue;

                deviceManager.updateUsage(
                    device.getId(),
                    usageDistribution(generator),
                    usageDistribution(generator));

                if (disconnectEvent(generator))
                    deviceManager.setConnected(device.getId(), false);
            }
        }
    });
    simulationThread.join();
}
