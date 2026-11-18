#pragma once

#include <QObject>
#include <QVariantList>
#include <QVariantMap>
#include <QTimer>
#include <random>
#include "DeviceManager.h"


class DeviceController : public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(QVariantList devices READ devices NOTIFY dataChanged)
    Q_PROPERTY(QVariantList alarms READ alarms NOTIFY dataChanged)
    Q_PROPERTY(int activeCount READ activeCount NOTIFY dataChanged)
    Q_PROPERTY(int warningCount READ warningCount NOTIFY dataChanged)
    Q_PROPERTY(int criticalCount READ criticalCount NOTIFY dataChanged)

    public:
    explicit DeviceController(QObject *parent = nullptr);

    QVariantList devices() const;
    QVariantList alarms() const;

    int activeCount() const;
    int warningCount() const;  
    int criticalCount() const;

    signals:
    void dataChanged();

    private:
    void initializedDevices();
    void updateSimulation();
    void addAlarm(const QString &deviceId, AlarmSeverity severity);
    QVariantMap deviceToMap(const Device &device) const;
    QString severityToString(AlarmSeverity severity) const;

    DeviceManager deviceManager_;
    QTimer simulationTimer_;

    std::mt19937 generator_;
    std::uniform_int_distribution<int> usageDistribution_{5, 100};
    std::bernoulli_distribution disconnectEvent_{0.02};

    QVariantList alarms_;
};