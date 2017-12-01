#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "convolutioner.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QObject* root = engine.rootObjects()[0];
    Convolutioner* convolutioner = new Convolutioner(root);

    engine.rootContext()->setContextProperty("convolutioner", convolutioner);

    QObject::connect(root, SIGNAL(qmlComputeApriory(QString, QString)),
                     convolutioner, SLOT(computeApriory(QString, QString)));

    QObject::connect(root, SIGNAL(qmlComputeOverlapAdd(QString, QString)),
                     convolutioner, SLOT(computeOverlapAdd(QString, QString)));

    QObject::connect(root, SIGNAL(qmlComputeOverlapSave(QString, QString)),
                     convolutioner, SLOT(computeOverlapSave(QString, QString)));

    return app.exec();
}
