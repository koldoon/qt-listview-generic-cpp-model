#pragma once

#include "qobject_list_model.h"
#include <QDebug>
#include <QObject>
#include <QString>

namespace app {
    // Some sort of data structure as a collection element
    class DataItem : public QObject {
        Q_OBJECT
        Q_PROPERTY( int id READ id WRITE setId NOTIFY changed )
        Q_PROPERTY( QString value READ value WRITE setValue NOTIFY changed )

    public:
        explicit DataItem( int id = 42, QString value = "The main question" );

        int     id();
        QString value();

        void setId( const int& value );
        void setValue( const QString& value );

        // Example of some business logic that changes internal model state.
        Q_INVOKABLE void doubleId();

    signals:
        void changed();

    private:
        int     m_id;
        QString m_value;
    };
}
