#pragma once

//
// DECLARE_LIST_MODEL( NAME, ITEM_TYPE )
// --------------------------
// Since it is impossible to create generic template class using Q_OBJECT,
// because MOC doesn't support such option, this macro generates
// ItemModel class suitable for using as data model for any QML ListView component
// with the following structure:
//
//  class NAME
//      + void push( const QSharedPointer<ITEM_TYPE>& item )
//      + void removeAt( int i )
//      + void insert( int i, const QSharedPointer<ITEM_TYPE>& item )
//      + int length() const
//      + const QList<QSharedPointer<ITEM_TYPE>>& list() const  // internal storage accessor
//      # Q_INVOKABLE ITEM_TYPE* item( int i, bool keepOwnership ) const   // QML Invokable
//
// Basically, it provides indices and changes signals to view,
// plus QList-like interface to feed and change internal storage (QList<QSharedPointer>).
// Typed item element can be get by "item()" method inside the delegate by index.
// To be able to use this model as QML Property Type, you MUST register
// this types as follows:
//
// TYPE must be QObject: class TYPE : public QObject { }
// Immediately after the TYPE class declaration:
//
//      Q_DECLARE_METATYPE( TYPE* )
//
// Inside void main() OR void QQmlExtensionPlugin::registerTypes():
//
//      qmlRegisterUncreatableType<ListModel_TYPE>( ... );
//      qmlRegisterUncreatableType<TYPE>( ... );
//

#include <QAbstractListModel>
#include <QList>
#include <QObject>
#include <QQmlEngine>
#include <QSharedPointer>

// private namespace with internal implementation
namespace __listmodel {
    class IndicesListModelImpl : public QAbstractListModel {
        Q_OBJECT
        Q_PROPERTY( int length READ length NOTIFY lengthChanged )

    public:
        // Since this class is intendent to be used in JS, we use conventional
        // to JS lists count/size method and property - length
        int length() const;

        // --- QAbstractListModel ---
        int      rowCount( const QModelIndex& parent ) const override;
        QVariant data( const QModelIndex& index, int role ) const override;

    protected:
        // Create "count" indices and push them to end
        void push( int count = 1 );

        // Remove "count" indices from the end.
        void pop( int count = 1 );

        // Remove indices at particular place.
        void removeAt( int index, int count = 1 );

        // Insert indices at particular place.
        void insertAt( int index, int count = 1 );

        // Reset model with new indices count
        void reset( int length = 0 );

    Q_SIGNALS:
        void lengthChanged( const int& length );

    private:
        int m_length = 0;
        int a; // alignment
    };
}

Q_DECLARE_METATYPE( __listmodel::IndicesListModelImpl* )


namespace __listmodel {
    template <class ItemType>
    class ListModelImplTemplate : public IndicesListModelImpl {
    public:
        void removeOne( const QSharedPointer<ItemType>& item ) {
            const auto index = storage.indexOf( item );
            if ( index != -1 )
                removeAt( index );
        }

        void clear() {
            storage.clear();
            IndicesListModelImpl::reset();
        }

        void append( const QSharedPointer<ItemType>& item ) {
            storage.append( item );
            IndicesListModelImpl::push();
        }

        void append( const QList<QSharedPointer<ItemType>>& list ) {
            storage.append( list );
            IndicesListModelImpl::push( list.count() );
        }

        void pop( int count ) {
            if ( count <= 0 )
                return;
            if ( count > IndicesListModelImpl::length() )
                count = IndicesListModelImpl::length();
            int countCpy = count;
            for ( ; count > 0; count-- ) {
                storage.removeAt( storage.count() );
            }
            IndicesListModelImpl::pop( countCpy );
        }

        void removeAt( int i ) {
            if ( i > IndicesListModelImpl::length() )
                return;
            storage.removeAt( i );
            IndicesListModelImpl::removeAt( i );
        }

        void insert( int i, const QSharedPointer<ItemType>& item ) {
            storage.insert( i, item );
            IndicesListModelImpl::insertAt( i );
        }

        // --- QList-style comfort ;) ---

        ListModelImplTemplate& operator+=( const QSharedPointer<ItemType>& t ) {
            append( t );
            return *this;
        }

        ListModelImplTemplate& operator<<( const QSharedPointer<ItemType>& t ) {
            append( t );
            return *this;
        }

        ListModelImplTemplate& operator+=( const QList<QSharedPointer<ItemType>>& list ) {
            append( list );
            return *this;
        }

        ListModelImplTemplate& operator<<( const QList<QSharedPointer<ItemType>>& list ) {
            append( list );
            return *this;
        }

        // Internal QList storage accessor. It is restricted to change it directly,
        // since we have to proxy all this calls, but it is possible to use it's
        // iterators and other useful public interfaces and const methods.
        const QList<QSharedPointer<ItemType>>& list() const {
            return storage;
        }

        // Qt conventional container size method
        int count() {
            return IndicesListModelImpl::length();
        }

    protected:
        QList<QSharedPointer<ItemType>> storage;
    };
}


#define DECLARE_LIST_MODEL( NAME, ITEM_TYPE )                                        \
    class NAME : public __listmodel::ListModelImplTemplate<ITEM_TYPE> {              \
        Q_OBJECT                                                                     \
                                                                                     \
    protected:                                                                       \
        Q_INVOKABLE ITEM_TYPE* item( int i, bool keepOwnership = true ) const {      \
            if ( i >= 0 && i < storage.length() && storage.length() > 0 ) {          \
                auto obj = storage[i].data();                                        \
                if ( keepOwnership )                                                 \
                    QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership ); \
                return obj;                                                          \
            }                                                                        \
            else {                                                                   \
                return Q_NULLPTR;                                                    \
            }                                                                        \
        }                                                                            \
    };                                                                               \
                                                                                     \
    Q_DECLARE_METATYPE( NAME* )
