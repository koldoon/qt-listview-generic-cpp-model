#pragma once

#include "data_item.h"
#include "list_model.h"
#include <QList>
#include <QObject>
#include <QSharedPointer>

// Generate class implementation using MACRO defined in "list_model.h"
DECLARE_LIST_MODEL( ListModel_DataItem, app::DataItem )

// "app" namespace is defined on purpose to show the case of using
// list model within namespaces
namespace app {

    // Provider is an example of class with some buseness logic that
    // needs to expose some collection of items of class DataItem
    class Provider : public QObject {
        Q_OBJECT

        // QML Property of type of generated Model Class
        Q_PROPERTY( ListModel_DataItem* itemsModel READ itemsModel NOTIFY changed )

    public:
        explicit Provider( QObject* parent = Q_NULLPTR );

        Q_INVOKABLE void addItem();
        Q_INVOKABLE void removeItem();

    Q_SIGNALS:
        void changed();

    private:
        // Since this getter is not safe (ownership remains to c++)
        // and it is used for QML only it'd better to make it private.
        ListModel_DataItem* itemsModel();
        ListModel_DataItem  m_itemsModel;
    };
}
