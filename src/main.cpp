#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUrl>

#include "DeviceController.h"
#include "UdpReceiver.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    QGuiApplication::setApplicationName("WorkSense");
    QGuiApplication::setOrganizationName("WorkSense");
    
    DeviceController deviceController;
    
    UdpReceiver udpReceiver;
    udpReceiver.startListening(50001);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty(
        "deviceController", &deviceController);

    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}