#include "Alarm.h"
#include <string>

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