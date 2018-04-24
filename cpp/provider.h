#pragma once

#include "data_item.h"
#include "data_item_2.h"
#include "qobject_list_model.h"
#include <QList>
#include <QObject>
#include <QSharedPointer>

// "app" namespace is defined on purpose to show the case of using
// list model within namespaces
namespace app {

    DECLARE_Q_OBJECT_LIST_MODEL( DataItem )
    DECLARE_Q_OBJECT_LIST_MODEL( DataItem2 )

    // Provider is an example of class with some business logic that
    // needs to expose some collection of items of class DataItem
    class Provider : public QObject {
        Q_OBJECT
        Q_PROPERTY( QObjectListModel_DataItem* items READ items CONSTANT )
        Q_PROPERTY( QObjectListModel_DataItem2* items2 READ items2 CONSTANT )

    public:
        explicit Provider( QObject* parent = Q_NULLPTR );

        Q_INVOKABLE void addItem();
        Q_INVOKABLE void addItems3();
        Q_INVOKABLE void changeItem();
        Q_INVOKABLE void removeItem();

    private:
        // Since this getter is not safe (ownership remains to c++)
        // and it is used for QML only it'd better to make it private.
        QObjectListModel_DataItem* items();
        QObjectListModel_DataItem  m_items;

        QObjectListModel_DataItem2* items2();
        QObjectListModel_DataItem2  m_items2;
    };
}
