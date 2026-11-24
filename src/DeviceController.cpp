#include "DeviceController.h"
#include <QDateTime>
#include <algorithm>
#include <stdexcept>
#include <iostream>

DeviceController::DeviceController(UdpReceiver &udpReceiver, QObject *parent)
    : QObject(parent)
{
    deviceManager_.setAlarmHandler(
        [this](const std::string &deviceId, AlarmSeverity severity)
        {
            addAlarm(QString::fromStdString(deviceId), severity);
        });

    connect(&udpReceiver, &UdpReceiver::messageReceived, this, &DeviceController::handleMessage);
}

void DeviceController::handleMessage(const QByteArray &message)
{
    try
    {
        deviceManager_.processMessage(message);
        emit dataChanged();
    }
    catch (const std::exception &error)
    {
        std::cerr << "Failed to process UDP message: " << error.what() << std::endl;
    }
}

QVariantList DeviceController::devices() const
{
    QVariantList result;

    for (const auto &device : deviceManager_.getListOfDevices())
        result.append(deviceToMap(device));

    return result;
}

QVariantMap DeviceController::deviceToMap(const Device &device) const
{
    const AlarmSeverity severity = evaluateAlarm(device);
    return {
        {"id", QString::fromStdString(device.getId())},
        {"name", QString::fromStdString(device.name_)},
        {"cpu", device.cpuUsage_},
        {"memory", device.memoryUsage_},
        {"connected", device.isConnected()},
        {"severity", severityToString(severity)}};
}

QVariantList DeviceController::alarms() const
{
    return alarms_;
}

int DeviceController::activeCount() const
{
    return std::count_if(deviceManager_.getListOfDevices().begin(),
                         deviceManager_.getListOfDevices().end(),
                         [](const Device &device)
                         { return device.isConnected(); });
}

int DeviceController::warningCount() const
{
    return std::count_if(deviceManager_.getListOfDevices().begin(),
                         deviceManager_.getListOfDevices().end(),
                         [](const Device &device)
                         { return evaluateAlarm(device) == AlarmSeverity::Warning; });
}

int DeviceController::criticalCount() const
{
    return std::count_if(deviceManager_.getListOfDevices().begin(),
                         deviceManager_.getListOfDevices().end(),
                         [](const Device &device)
                         { return evaluateAlarm(device) == AlarmSeverity::Critical; });
}

QString DeviceController::severityToString(AlarmSeverity severity) const
{
    switch (severity)
    {
    case AlarmSeverity::Normal:
        return "NORMAL";
    case AlarmSeverity::Warning:
        return "WARNING";
    case AlarmSeverity::Critical:
        return "CRITICAL";
    case AlarmSeverity::Disconnected:
        return "DISCONNECTED";
    }
    return "UNKNOWN";
}

void DeviceController::addAlarm(const QString &deviceId, AlarmSeverity severity)
{
    QVariantMap alarm;
    alarm["deviceId"] = deviceId;
    alarm["severity"] = severityToString(severity);
    alarm["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    switch (severity)
    {
    case AlarmSeverity::Warning:
        alarm["message"] = "Resource usage warning";
        break;
    case AlarmSeverity::Critical:
        alarm["message"] = "Critical resource usage";
        break;
    case AlarmSeverity::Disconnected:
        alarm["message"] = "Device disconnected";
        break;
    default:
        alarm["message"] = "Device status changed";
        break;
    }

    alarms_.prepend(alarm);

    while (alarms_.size() > 10)
        alarms_.removeLast();

    emit dataChanged();
}
