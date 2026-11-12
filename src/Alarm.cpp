#include "Alarm.h"
#include <string>

AlarmSeverity evaluateAlarm(const Device &device)
{
    if (!device.isConnected())
        return AlarmSeverity::DeviceDisconnected;

    if (device.cpuUsage_ >= 90.0 || device.memoryUsage_ >= 95.0)
        return AlarmSeverity::Critical;

    if (device.cpuUsage_ >= 75.0 || device.memoryUsage_ >= 80.0)
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
    case AlarmSeverity::DeviceDisconnected:
        return "DEVICE_DISCONNECTED";
    }
    return "UNKNOWN";
}