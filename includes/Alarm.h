#include "Device.h"

enum class AlarmSeverity
{
    Normal,
    Warning,
    Critical
};

AlarmSeverity evaluateAlarm(const Device &device);
std::string alarmToString(AlarmSeverity severity);