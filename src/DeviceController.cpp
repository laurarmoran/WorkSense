#include "DeviceController.h"
#include <QDateTime>
#include <algorithm>
#include <stdexcept>

DeviceController::DeviceController(QObject *parent) : QObject(parent), generator_(std::random_device{}())
{
    deviceManager_.setAlarmHandler(
        [this](const std::string &deviceId, AlarmSeverity severity)
        {
            addAlarm(QString::fromStdString(deviceId), severity);
        });

    initializedDevices();

    connect(&simulationTimer_, &QTimer::timeout, this, &DeviceController::updateSimulation);

    simulationTimer_.start(2000);
}

void DeviceController::initializedDevices()
{
    deviceManager_.addDevice(
        "SIM-TRAIN-01", "Railway Simulator", 42.5, 61.0, true);

    deviceManager_.addDevice(
        "RAILWAY-TEST-01", "Railway Test Bench", 82.0, 72.0, true);

    deviceManager_.addDevice(
        "MAINTENANCE-WS-01", "Maintenance Station", 94.0, 97.0, true);
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
    case AlarmSeverity::DeviceDisconnected:
        return "DEVICE_DISCONNECTED";
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
    case AlarmSeverity::DeviceDisconnected:
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

void DeviceController::updateSimulation()
{
    for (const auto &device : deviceManager_.getListOfDevices())
    {
        if (!device.isConnected())
            continue;

        double newCpuUsage = usageDistribution_(generator_);
        double newMemoryUsage = usageDistribution_(generator_);

        deviceManager_.updateUsage(device.getId(), newCpuUsage, newMemoryUsage);

        if (disconnectEvent_(generator_))
            deviceManager_.setConnected(device.getId(), false);
    }

    emit dataChanged();
}