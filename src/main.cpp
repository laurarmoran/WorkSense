#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUrl>
#include <iostream>
#include "DeviceController.h"
#include "UdpReceiver.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QGuiApplication::setApplicationName("WorkSense");
    QGuiApplication::setOrganizationName("WorkSense");

    UdpReceiver udpReceiver;
    DeviceController deviceController(udpReceiver);
    auto port = 50001;
    udpReceiver.startListening(port) ? std::cout << "Listening on port " << port << std::endl
                                     : std::cerr << "Failed to bind to port " << port << std::endl;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty(
        "deviceController", &deviceController);

    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}