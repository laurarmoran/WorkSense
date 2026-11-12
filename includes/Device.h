#pragma once

#include <string>

class DeviceManager;
class Device
{
public:
    std::string name_;
    double cpuUsage_;
    double memoryUsage_;

    Device(const std::string &id, const std::string &name, double cpuUsage,
           double memoryUsage, bool connected = true);

    const std::string &getId() const { return id_; }
    bool isConnected() const { return connected_; }
    
    private:
    friend class DeviceManager; // Allow DeviceManager to access private members of Device
    std::string id_;
    bool connected_;
    void setConnected(bool connected) { connected_ = connected; }
};
