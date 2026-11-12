#include "Alarm.h"
#include <functional>
#include <vector>

class DeviceManager
{
public:
    using AlarmHandler = std::function<void(const std::string &, AlarmSeverity)>;

    void setAlarmHandler(AlarmHandler handler);
    void addDevice(const std::string &id, const std::string &name, double cpuUsage,
                   double memoryUsage, bool connected = true);
    void updateUsage(const std::string &id, double cpuUsage, double memoryUsage);
    const std::vector<Device> &getListOfDevices() const;
    int removeDevice(const std::string &id);
    bool isConnected(const std::string &id) const;
    void setConnected(const std::string &id, bool connected);

private:
    std::vector<Device> listOfDevices_;
    AlarmHandler alarmHandler_;
};