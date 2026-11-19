#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QObject>
#include <QUdpSocket>
#include <QByteArray>

class UdpReceiver : public QObject
{
    Q_OBJECT

public:
    explicit UdpReceiver(QObject *parent = nullptr);
    bool startListening(quint16 port);

signals:
    void messageReceived(const QByteArray &msg);

private slots:
    void readPendingDatagrams();

private:
    QUdpSocket socket_;
};

#endif // UDPRECEIVER_H