#include "DeviceManager.h"
#include <stdexcept>
#include <utility>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <iostream>

void DeviceManager::processMessage(const QByteArray &message)
{
    // Receives json from UdpReceiver and processes it
    std::cout << "Message received: " << message.toStdString() << std::endl;
    QJsonDocument doc = QJsonDocument::fromJson(message);
    if (!doc.isObject())
        throw std::invalid_argument("Invalid JSON message");

    QJsonObject obj = doc.object();

    std::string action = obj.value("action").toString().toStdString();

    if (action != "add" && action != "update" && action != "remove" && action != "setConnected")
        throw std::invalid_argument("Unknown action: " + action);
    else if (action == "add")
        addDevice(
            obj.value("deviceId").toString().toStdString(),
            obj.value("deviceName").toString().toStdString(),
            obj.value("cpuUsage").toString().toDouble(),
            obj.value("memoryUsage").toString().toDouble(),
            obj.value("status").toString() == "CONNECTED");
    else if (action == "update")
        updateUsage(
            obj.value("deviceId").toString().toStdString(),
            obj.value("cpuUsage").toString().toDouble(),
            obj.value("memoryUsage").toString().toDouble());
    else if (action == "remove")
        removeDevice(obj.value("deviceId").toString().toStdString());
    else if (action == "setConnected")
        setConnected(
            obj.value("deviceId").toString().toStdString(),
            obj.value("status").toString() == "CONNECTED");
}

void DeviceManager::setAlarmHandler(AlarmHandler handler)
{
    alarmHandler_ = std::move(handler);
}

void DeviceManager::addDevice(const std::string &id, const std::string &name, double cpuUsage,
                              double memoryUsage, bool connected)
{
    if (id.empty())
        throw std::invalid_argument("Device ID cannot be empty");
    if (name.empty())
        throw std::invalid_argument("Device name cannot be empty");
    if (cpuUsage < 0.0 || cpuUsage > 100.0)
        throw std::invalid_argument("CPU usage must be between 0 and 100");
    if (memoryUsage < 0.0 || memoryUsage > 100.0)
        throw std::invalid_argument("Memory usage must be between 0 and 100");

    listOfDevices_.emplace_back(id, name, cpuUsage, memoryUsage, connected);
    const auto &device = listOfDevices_.back();
    const AlarmSeverity severity = evaluateAlarm(device);
    if (severity != AlarmSeverity::Normal && alarmHandler_)
        alarmHandler_(device.getId(), severity);
}

void DeviceManager::updateUsage(const std::string &id, double cpuUsage, double memoryUsage)
{
    if (cpuUsage < 5.0 || cpuUsage > 100.0)
        throw std::invalid_argument("CPU usage must be between 5 and 100");
    if (memoryUsage < 5.0 || memoryUsage > 100.0)
        throw std::invalid_argument("Memory usage must be between 5 and 100");

    for (auto &device : listOfDevices_)
    {
        if (device.getId() == id)
        {
            const AlarmSeverity previousSeverity = evaluateAlarm(device);
            device.cpuUsage_ = cpuUsage;
            device.memoryUsage_ = memoryUsage;
            const AlarmSeverity currentSeverity = evaluateAlarm(device);
            if (currentSeverity != AlarmSeverity::Normal &&
                currentSeverity != previousSeverity &&
                alarmHandler_)
            {
                alarmHandler_(device.getId(), currentSeverity);
            }
            return;
        }
    }

    throw std::out_of_range("Device ID not found: " + id);
}

int DeviceManager::removeDevice(const std::string &id)
{
    for (auto it = listOfDevices_.begin(); it != listOfDevices_.end(); ++it)
    {
        if (it->getId() == id)
        {
            listOfDevices_.erase(it);
            return 1;
        }
    }
    return 0;
}

const std::vector<Device> &DeviceManager::getListOfDevices() const
{
    return listOfDevices_;
}

bool DeviceManager::isConnected(const std::string &id) const
{
    for (const auto &device : listOfDevices_)
    {
        if (device.getId() == id)
            return device.isConnected();
    }
    return false;
}

void DeviceManager::setConnected(const std::string &id, bool connected)
{
    for (auto &device : listOfDevices_)
    {
        if (device.getId() == id)
        {
            const bool wasConnected = device.isConnected();
            device.setConnected(connected);
            if (wasConnected && !connected && alarmHandler_)
                alarmHandler_(device.getId(), AlarmSeverity::Disconnected);
            return;
        }
    }
    throw std::out_of_range("Device ID not found: " + id);
}