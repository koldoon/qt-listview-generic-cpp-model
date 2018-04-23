#include "provider.h"
#include <QDebug>

namespace app {

    Provider::Provider( QObject* parent )
        : QObject( parent ) {
    }


    void Provider::addItem() {
        auto item = QSharedPointer<DataItem>( new DataItem( m_items.count() ) );
        m_items << item;
    }


    void Provider::addItems3() {
        QList<QSharedPointer<DataItem>> source;
        source << QSharedPointer<DataItem>( new DataItem( m_items.count() ) )
               << QSharedPointer<DataItem>( new DataItem( m_items.count() + 1 ) )
               << QSharedPointer<DataItem>( new DataItem( m_items.count() + 2 ) );
        m_items << source;
    }

    void Provider::changeItem() {
        if ( m_items.count() == 0 )
            return;

        int index = m_items.count() / 2;
        m_items.replace( index, QSharedPointer<DataItem>( new DataItem( 111, "Changed Item" ) ) );
    }


    void Provider::removeItem() {
        if ( m_items.count() == 0 )
            return;

        int index = m_items.count() / 2;
        m_items.removeAt( index );
    }


    QObjectListModel_DataItem* Provider::items() {
        return &m_items;
    }


    QObjectListModel_DataItem2* Provider::items2() {
        return &m_items2;
    }
}
