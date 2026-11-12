#pragma once

#include "Device.h"

enum class AlarmSeverity
{
    Normal,
    Warning,
    Critical, 
    DeviceDisconnected
};

AlarmSeverity evaluateAlarm(const Device &device);
std::string alarmToString(AlarmSeverity severity);