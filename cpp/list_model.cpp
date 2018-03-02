#include "list_model.h"

namespace __listmodel {

    // instantiating optimization.
    // since we don't need actual model index when we work with
    // flat list, we can instantiate it just once and use everywhere
    static const QModelIndex ROOT_MODEL_INDEX;


    void IndicesListModelImpl::push( int count ) {
        insertAt( m_length, count );
    }


    void IndicesListModelImpl::pop( int count ) {
        return removeAt( m_length - count, count );
    }


    void IndicesListModelImpl::removeAt( int index, int count ) {
        if ( index < 0 || index + count > m_length || m_length == 0 || count < 1 )
            return;

        int start = index;
        int end = index + count - 1;
        beginRemoveRows( ROOT_MODEL_INDEX, start, end );
        m_length -= count;
        endRemoveRows();
        emit lengthChanged( m_length );
    }


    void IndicesListModelImpl::insertAt( int index, int count ) {
        if ( index < 0 || index > m_length + 1 || count < 1 )
            return;

        int start = index;
        int end = index + count - 1;
        beginInsertRows( ROOT_MODEL_INDEX, start, end );
        m_length += count;
        endInsertRows();
        emit lengthChanged( m_length );
    }


    void IndicesListModelImpl::reset( int count ) {
        beginResetModel();
        m_length = count < 0 ? 0 : count;
        endResetModel();
        emit lengthChanged( m_length );
    }


    int IndicesListModelImpl::length() const {
        return m_length;
    }


    int IndicesListModelImpl::rowCount( const QModelIndex& parent ) const {
        Q_UNUSED( parent )
        return m_length;
    }


    QVariant IndicesListModelImpl::data( const QModelIndex& index, int role ) const {
        Q_UNUSED( index )
        Q_UNUSED( role )
        return QVariant();
    }
}
