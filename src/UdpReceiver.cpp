#include "UdpReceiver.h"

UdpReceiver::UdpReceiver(QObject *parent) : QObject(parent)
{
    connect(&socket_, &QUdpSocket::readyRead, this, &UdpReceiver::readPendingDatagrams);
}

bool UdpReceiver::startListening(quint16 port)
{
    return socket_.bind(QHostAddress::LocalHost, port);
}

void UdpReceiver::readPendingDatagrams()
{
    while (socket_.hasPendingDatagrams())
    {
        QByteArray message;
        message.resize(socket_.pendingDatagramSize());
        socket_.readDatagram(message.data(), message.size());
        emit messageReceived(message);
    }
}