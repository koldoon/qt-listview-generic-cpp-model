#pragma once

#include "data_item.h"
#include <QAbstractListModel>

namespace __qobjectsqmllist {
    // role name to be used in delegates for list items
    static const char ITEM_ROLE_NAME[] = "modelData";

    // instantiating optimization.
    // since we don't need actual model index when we work with
    // flat list, we can instantiate it just once and use everywhere
    static const QModelIndex ROOT_MODEL_INDEX;

    class QObjectsQmlListBase : public QAbstractListModel {
        Q_OBJECT
        Q_PROPERTY( int length MEMBER m_length NOTIFY lengthChanged )

    Q_SIGNALS:
        void lengthChanged();

    protected:
        void setLength( int value ) {
            m_length = value;
            emit lengthChanged();
        }

    private:
        int m_length = 0;
    };
}


template <typename T>
class QObjectsQmlList : public QList<QSharedPointer<T>>, public __qobjectsqmllist::QObjectsQmlListBase {
    using ITEM = QSharedPointer<T>;
    using LIST = QList<ITEM>;

public:
    enum CollectionRole {
        ItemRole = Qt::UserRole
    };

    int rowCount( const QModelIndex& parent ) const override {
        Q_UNUSED( parent )
        return LIST::count();
    }

    QVariant data( const QModelIndex& index, int role ) const override {
        auto row = index.row();

        if ( row < 0 || row >= LIST::count() )
            return QVariant();

        if ( role == ItemRole )
            return QVariant::fromValue<QObject*>( LIST::at( row ).data() );

        return QVariant();
    }

    QHash<int, QByteArray> roleNames() const override {
        auto roles = QAbstractListModel::roleNames();
        roles[ItemRole] = __qobjectsqmllist::ITEM_ROLE_NAME;
        return roles;
    };


    void insert( int i, const ITEM& item ) {
        if ( i < 0 || i > LIST::count() )
            return;

        beginInsertRows( __qobjectsqmllist::ROOT_MODEL_INDEX, i, i );
        LIST::insert( i, item );
        endInsertRows();
        setLength( LIST::count() );
    }

    void append( const ITEM& item ) {
        auto i = LIST::count();
        beginInsertRows( __qobjectsqmllist::ROOT_MODEL_INDEX, i, i );
        LIST::append( item );
        endInsertRows();
        setLength( LIST::count() );
    }

    void append( const LIST& list ) {
        if ( list.count() == 0 )
            return;

        auto start = LIST::count();
        auto end = LIST::count() + list.count() - 1;
        beginInsertRows( __qobjectsqmllist::ROOT_MODEL_INDEX, start, end );
        LIST::append( list );
        endInsertRows();
        setLength( LIST::count() );
    }

    void removeAt( int i ) {
        if ( i < 0 || i > LIST::count() - 1 )
            return;

        beginRemoveRows( __qobjectsqmllist::ROOT_MODEL_INDEX, i, i );
        LIST::removeAt( i );
        endRemoveRows();
        setLength( LIST::count() );
    }

    void replace( int i, const ITEM& item ) {
        removeAt( i );
        insert( i, item );
    }

    // --- QList-style comfort ;) ---

    QObjectsQmlList& operator+=( const ITEM& t ) {
        append( t );
        return *this;
    }

    QObjectsQmlList& operator<<( const ITEM& t ) {
        append( t );
        return *this;
    }

    QObjectsQmlList& operator+=( const LIST& list ) {
        append( list );
        return *this;
    }

    QObjectsQmlList& operator<<( const LIST& list ) {
        append( list );
        return *this;
    }
};
