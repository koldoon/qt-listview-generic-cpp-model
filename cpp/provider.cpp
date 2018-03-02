#include "provider.h"
#include <QDebug>

namespace app {

    Provider::Provider( QObject* parent )
        : QObject( parent ) {
    }


    void Provider::addItem() {
        auto item = QSharedPointer<DataItem>( new DataItem( m_itemsModel.count() ) );
        m_itemsModel << item;
    }


    void Provider::addItems3() {
        QList<QSharedPointer<DataItem>> source;
        source << QSharedPointer<DataItem>( new DataItem( m_itemsModel.count() ) )
               << QSharedPointer<DataItem>( new DataItem( m_itemsModel.count() + 2 ) )
               << QSharedPointer<DataItem>( new DataItem( m_itemsModel.count() + 3 ) );
        m_itemsModel << source;
    }


    void Provider::removeItem() {
        if ( m_itemsModel.count() == 0 )
            return;

        int index = m_itemsModel.count() / 2;
        m_itemsModel.removeAt( index );
    }


    ListModel_DataItem* Provider::itemsModel() {
        return &m_itemsModel;
    }
}
