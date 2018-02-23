#include "list_model.h"

namespace __listmodel {

    // instantiating optimization.
    // since we don't need actual model index when we work with
    // flat list, we can instantiate it just once and use everywhere
    static const QModelIndex ROOT_MODEL_INDEX;


    void IndicesListModelImpl::push( int count ) {
        insertAt( m_count, count );
    }


    void IndicesListModelImpl::pop( int count ) {
        return removeAt( m_count - count, count );
    }


    void IndicesListModelImpl::removeAt( int index, int count ) {
        if ( index < 0 || index + count > m_count || m_count == 0 || count < 1 )
            return;

        int start = index;
        int end = index + count - 1;
        beginRemoveRows( ROOT_MODEL_INDEX, start, end );
        m_count -= count;
        endRemoveRows();
        emit countChanged( m_count );
    }


    void IndicesListModelImpl::insertAt( int index, int count ) {
        if ( index < 0 || index + count > m_count + 1 || count < 1 )
            return;

        int start = index;
        int end = index + count - 1;
        beginInsertRows( ROOT_MODEL_INDEX, start, end );
        m_count += count;
        endInsertRows();
        emit countChanged( m_count );
    }


    void IndicesListModelImpl::reset( int count ) {
        beginResetModel();
        m_count = count < 0 ? 0 : count;
        endResetModel();
        emit countChanged( m_count );
    }


    int IndicesListModelImpl::count() const {
        return m_count;
    }


    int IndicesListModelImpl::rowCount( const QModelIndex& parent ) const {
        Q_UNUSED( parent )
        return m_count;
    }


    QVariant IndicesListModelImpl::data( const QModelIndex& index, int role ) const {
        Q_UNUSED( index )
        Q_UNUSED( role )
        return QVariant();
    }
}
