#include "Device.h"

#include <vector>

class DeviceManager
{
public:
    void addDevice(const std::string &id, const std::string &name, double cpuUsage,
                   double memoryUsage, bool connected = true);
    void updateUsage(const std::string &id, double cpuUsage, double memoryUsage);
    const std::vector<Device> &getListOfDevices() const;
    int removeDevice(const std::string &id);
    bool isConnected(const std::string &id) const;
    void setConnected(const std::string &id, bool connected);

private:
    std::vector<Device> listOfDevices_;
};