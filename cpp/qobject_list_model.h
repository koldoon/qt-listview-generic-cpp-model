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

    class QObjectListModelBase : public QAbstractListModel {
        Q_OBJECT
        Q_PROPERTY( int length MEMBER m_length NOTIFY lengthChanged )

    signals:
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


/*
 * QML ListView generic c++ model for collection of QObject-s.
 * Usage:
 * 1) Declare pointers to the contained class and to instantiated container itself:
 *    For some app::DataType it looks like
 *      Q_DECLARE_METATYPE( app::DataItem* )
 *      Q_DECLARE_METATYPE( QObjectList<app::DataItem>* )
 * 2) Register types in Qt Meta-system:
 *      qmlRegisterUncreatableType<app::DataItem>( "App", 1, 0, "DataItem", "interface" );
 *      qmlRegisterUncreatableType<QObjectList<app::DataItem>>( "App", 1, 0, "DataItemList", "interface" );
 * 3) Define and implement Q_PROPERTY:
 *      Q_PROPERTY( QObjectList<app::DataItem>* items READ items CONSTANT )
 * 4) Use this property as simple QList<QSharedObject<app::DataItem>>
 *
 * In QML ListView delegate the ROLE "modelData" will be available to get access to list item.
 * Since this role is conventional when using ListView with JS arrays, it is very handy
 * to use JS arrays as mock data providers during view prototyping.
 *
 * NOTE: Not all QList methods are usable to get this thing work. Please refer to this class source
 * and feel free to implement the rest methods.
 * Also, since QList doesn't have that methods marked as "virtual"
 * it is impossible to use this class with pointer to QList<>*.
 *
 * @author Vadim Usoltsev
 */
template <typename T>
class QObjectListModel : public QList<QSharedPointer<T>>, public __qobjectsqmllist::QObjectListModelBase {
    using ITEM = QSharedPointer<T>;
    using LIST = QList<ITEM>;

public:
    enum CollectionRole {
        ItemDataRole = Qt::UserRole
    };

    int rowCount( const QModelIndex& parent ) const override {
        Q_UNUSED( parent )
        return LIST::count();
    }

    QVariant data( const QModelIndex& index, int role ) const override {
        auto row = index.row();

        if ( row < 0 || row >= LIST::count() )
            return QVariant();

        if ( role == ItemDataRole )
            return QVariant::fromValue<QObject*>( LIST::at( row ).data() );

        return QVariant();
    }

    QHash<int, QByteArray> roleNames() const override {
        auto roles = QAbstractListModel::roleNames();
        roles[ItemDataRole] = __qobjectsqmllist::ITEM_ROLE_NAME;
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

    QObjectListModel& operator+=( const ITEM& t ) {
        append( t );
        return *this;
    }

    QObjectListModel& operator<<( const ITEM& t ) {
        append( t );
        return *this;
    }

    QObjectListModel& operator+=( const LIST& list ) {
        append( list );
        return *this;
    }

    QObjectListModel& operator<<( const LIST& list ) {
        append( list );
        return *this;
    }
};
