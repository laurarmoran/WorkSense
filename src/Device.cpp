#include "Device.h"

Device::Device(const std::string &id, const std::string &name,
               double cpuUsage, double memoryUsage, bool connected)
    : id(id),
      name(name),
      cpuUsage(cpuUsage),
      memoryUsage(memoryUsage),
      connected(connected)
{
}