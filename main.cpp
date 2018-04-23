#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <cpp/data_item_2.h>
#include <cpp/provider.h>

int main( int argc, char* argv[] ) {
    QCoreApplication::setAttribute( Qt::AA_EnableHighDpiScaling );
    QGuiApplication app( argc, argv );

    // These types are NOT supposed to be created in QML
    // for example, as ListDataModel { } or DataItem { }

    qmlRegisterUncreatableType<app::DataItem>( "App", 1, 0, "DataItem", "interface" );
    qmlRegisterUncreatableType<app::DataItem2>( "App", 1, 0, "DataItem2", "interface" );
    qmlRegisterUncreatableType<app::QObjectListModel_DataItem>( "App", 1, 0, "ListModel_DataItem", "interface" );
    qmlRegisterUncreatableType<app::QObjectListModel_DataItem2>( "App", 1, 0, "ListModel_DataItem2", "interface" );

    qmlRegisterType<app::Provider>( "App", 1, 0, "Provider" );

    QQmlApplicationEngine engine;
    engine.load( QUrl( QStringLiteral( "qrc:/qml/Main.qml" ) ) );

    if ( engine.rootObjects().isEmpty() )
        return -1;

    return app.exec();
}
