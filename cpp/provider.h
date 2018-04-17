#pragma once

#include "data_item.h"
#include "qobject_list.h"
#include <QList>
#include <QObject>
#include <QSharedPointer>

// "app" namespace is defined on purpose to show the case of using
// list model within namespaces
namespace app {

    // Provider is an example of class with some buseness logic that
    // needs to expose some collection of items of class DataItem
    class Provider : public QObject {
        Q_OBJECT

        // QML Property of type of generated Model Class
        Q_PROPERTY( QObjectList<app::DataItem>* items READ items CONSTANT )

    public:
        explicit Provider( QObject* parent = Q_NULLPTR );

        Q_INVOKABLE void addItem();
        Q_INVOKABLE void addItems3();
        Q_INVOKABLE void changeItem();
        Q_INVOKABLE void removeItem();

    private:
        // Since this getter is not safe (ownership remains to c++)
        // and it is used for QML only it'd better to make it private.
        QObjectList<app::DataItem>* items();
        QObjectList<app::DataItem>  m_items;
    };
}
