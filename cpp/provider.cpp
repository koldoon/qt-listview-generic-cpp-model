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


    void Provider::removeItem() {
        if ( m_itemsModel.length() == 0 )
            return;

        int index = m_itemsModel.length() / 2;
        m_itemsModel.removeAt( index );
    }


    ListModel_DataItem* Provider::itemsModel() {
        return &m_itemsModel;
    }
}
