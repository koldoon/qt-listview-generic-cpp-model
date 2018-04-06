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
               << QSharedPointer<DataItem>( new DataItem( m_items.count() + 2 ) )
               << QSharedPointer<DataItem>( new DataItem( m_items.count() + 3 ) );
        m_items << source;
    }


    void Provider::removeItem() {
        if ( m_items.count() == 0 )
            return;

        for ( auto item : m_items ) {
            qDebug() << item->value();
        }

        int index = m_items.count() / 2;
        m_items.removeAt( index );
    }


    ListModel_DataItem* Provider::items() {
        return &m_items;
    }
}
