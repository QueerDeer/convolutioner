#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "convolutioner.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QObject* root = engine.rootObjects()[0];
    Convolutioner* convolutioner = new Convolutioner(root);

    engine.rootContext()->setContextProperty("convolutioner", convolutioner);

    QObject::connect(root, SIGNAL(qmlGetFactor(QString, QString)),
                     convolutioner, SLOT(getFactor(QString, QString)));

    QObject::connect(root, SIGNAL(qmlComputeAprioryLine(QString, QString)),
                     convolutioner, SLOT(computeAprioryLine(QString, QString)));

    QObject::connect(root, SIGNAL(qmlComputeAprioryCircle(QString, QString)),
                     convolutioner, SLOT(computeAprioryCircle(QString, QString)));

    QObject::connect(root, SIGNAL(qmlComputeOverlapAddLine(QString, QString)),
                     convolutioner, SLOT(computeOverlapAddLine(QString, QString)));

    QObject::connect(root, SIGNAL(qmlComputeOverlapAddCircle(QString, QString)),
                     convolutioner, SLOT(computeOverlapAddCircle(QString, QString)));

    QObject::connect(root, SIGNAL(qmlComputeOverlapSaveLine(QString, QString)),
                     convolutioner, SLOT(computeOverlapSaveLine(QString, QString)));

    QObject::connect(root, SIGNAL(qmlComputeOverlapSaveCircle(QString, QString)),
                     convolutioner, SLOT(computeOverlapSaveCircle(QString, QString)));

    return app.exec();
}
