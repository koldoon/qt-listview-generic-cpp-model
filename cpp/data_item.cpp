#include "data_item.h"
#include <QDebug>

namespace app {
    DataItem::DataItem( int id, QString value )
        : QObject( NULL )
        , m_id( id )
        , m_value( value ) {
    }


    int DataItem::id() {
        return m_id;
    }


    void DataItem::setId( const int& v ) {
        if ( v != m_id ) {
            m_id = v;
            emit changed();
        }
    }


    QString DataItem::value() {
        return m_value;
    }


    void DataItem::setValue( const QString& v ) {
        if ( v != m_value ) {
            m_value = v;
            emit changed();
            qDebug() << "Value for" << id() << "is changed to:" << v;
        }
    }


    void DataItem::doubleId() {
        m_id *= 2;
        emit changed();
    }
}
