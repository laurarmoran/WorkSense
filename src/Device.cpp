#include "Device.h"
#include <stdexcept>

Device::Device(const std::string &id, const std::string &name,
               double cpuUsage, double memoryUsage, bool connected)
    : id_(id),
      name_(name),
      cpuUsage_(cpuUsage),
      memoryUsage_(memoryUsage),
      connected_(connected)
{
}

