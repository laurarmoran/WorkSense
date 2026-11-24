#pragma once

#include <QObject>
#include <QVariantList>
#include <QVariantMap>
#include "DeviceManager.h"
#include "UdpReceiver.h"

class DeviceController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantList devices READ devices NOTIFY dataChanged)
    Q_PROPERTY(QVariantList alarms READ alarms NOTIFY dataChanged)
    Q_PROPERTY(int activeCount READ activeCount NOTIFY dataChanged)
    Q_PROPERTY(int warningCount READ warningCount NOTIFY dataChanged)
    Q_PROPERTY(int criticalCount READ criticalCount NOTIFY dataChanged)

public:
    explicit DeviceController(UdpReceiver &udpReceiver, QObject *parent = nullptr);

    QVariantList devices() const;
    QVariantList alarms() const;

    int activeCount() const;
    int warningCount() const;
    int criticalCount() const;

signals:
    void dataChanged();

private:
    void addAlarm(const QString &deviceId, AlarmSeverity severity);
    QVariantMap deviceToMap(const Device &device) const;
    QString severityToString(AlarmSeverity severity) const;

    DeviceManager deviceManager_;
    QVariantList alarms_;

private slots:
    void handleMessage(const QByteArray &message);
};