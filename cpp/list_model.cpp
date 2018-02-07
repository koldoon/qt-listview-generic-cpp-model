#include "list_model.h"

namespace __listmodel {

    // instantiating optimization.
    // since we don't need actual model index when we work with
    // flat list, we can instantiate it just once and use everywhere
    static const QModelIndex ROOT_MODEL_INDEX;


    void IndicesListModelImpl::push( int count ) {
        insertAt( length, count );
    }


    void IndicesListModelImpl::pop( int count ) {
        return removeAt( length - count, count );
    }


    void IndicesListModelImpl::removeAt( int index, int count ) {
        if ( index < 0 || index + count > length || length == 0 || count < 1 )
            return;

        int start = index;
        int end = index + count - 1;
        beginRemoveRows( ROOT_MODEL_INDEX, start, end );
        length -= count;
        endRemoveRows();
        emit countChanged( length );
    }


    void IndicesListModelImpl::insertAt( int index, int count ) {
        if ( index < 0 || index + count > length + 1 || count < 1 )
            return;

        int start = index;
        int end = index + count - 1;
        beginInsertRows( ROOT_MODEL_INDEX, start, end );
        length += count;
        endInsertRows();
        emit countChanged( length );
    }


    void IndicesListModelImpl::reset( int count ) {
        beginResetModel();
        length = count < 0 ? 0 : count;
        endResetModel();
        emit countChanged( length );
    }


    int IndicesListModelImpl::count() const {
        return length;
    }


    int IndicesListModelImpl::rowCount( const QModelIndex& parent ) const {
        Q_UNUSED( parent )
        return length;
    }


    QVariant IndicesListModelImpl::data( const QModelIndex& index, int role ) const {
        Q_UNUSED( index )
        Q_UNUSED( role )
        return QVariant();
    }
}
