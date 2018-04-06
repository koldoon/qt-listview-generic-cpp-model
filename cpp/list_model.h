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
        // Since this class is intended to be used in JS, we use conventional
        // to JS lists count/size method and property - length
        int length() const;

        // --- QAbstractListModel ---
        int      rowCount( const QModelIndex& parent ) const override;
        QVariant data( const QModelIndex& index, int role ) const override;

        // Create "count" indices and push them to end
        //        void prePush( int count = 1 );
        void push( int count = 1 );

        // Remove "count" indices from the end.
        void pop( int count = 1 );

        // Remove indices at particular place.
        void removeAt( int index, int count = 1 );

        // Insert indices at particular place.
//        void preInsertAt( int index, int count = 1 );
        void insertAt( int index, int count = 1 );

        // Reset model with new indices count
        void reset( int length = 0 );

    Q_SIGNALS:
        void lengthChanged( int length );

    private:
        int m_length = 0;
        int a; // alignment
    };
}

Q_DECLARE_METATYPE( __listmodel::IndicesListModelImpl* )


namespace __listmodel {
    template <class ItemType>
    class ListModelImplTemplate : public QObject, public QList<QSharedPointer<ItemType>> {
    public:
        void removeOne( const QSharedPointer<ItemType>& item ) {
            auto index = QList<ItemType>::indexOf( item );
            if ( index != -1 )
                removeAt( index );
        }

        void clear() {
            QList<ItemType>::clear();
            m_index.reset();
        }

        void append( const QSharedPointer<ItemType>& item ) {
            QList<QSharedPointer<ItemType>>::append( item );
            m_index.push();
        }

        void append( const QList<QSharedPointer<ItemType>>& list ) {
            QList<QSharedPointer<ItemType>>::append( list );
            m_index.push( list.count() );
        }

        void pop( int count = 1 ) {
            if ( count <= 0 )
                return;
            if ( count > m_index.length() )
                count = m_index.length();
            int countCpy = count;
            for ( ; count > 0; count-- ) {
                QList<ItemType>::removeAt( QList<ItemType>::count() - 1 );
            }
            m_index.pop( countCpy );
        }

        void removeAt( int i ) {
            if ( i > m_index.length() )
                return;
            QList<QSharedPointer<ItemType>>::removeAt( i );
            m_index.removeAt( i );
        }

        void insert( int i, const QSharedPointer<ItemType>& item ) {
            QList<ItemType>::insert( i, item );
            m_index.insertAt( i );
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

    protected:
        IndicesListModelImpl m_index;
    };
}


#define DECLARE_LIST_MODEL( NAME, ITEM_TYPE )                                                          \
    class NAME : public __listmodel::ListModelImplTemplate<ITEM_TYPE> {                                \
        Q_OBJECT                                                                                       \
        Q_PROPERTY( __listmodel::IndicesListModelImpl* model READ model CONSTANT )                     \
        Q_PROPERTY( int length READ length NOTIFY lengthChanged )                                      \
                                                                                                       \
    public:                                                                                            \
        NAME() {                                                                                       \
            connect( &m_index, SIGNAL( lengthChanged( int ) ), this, SIGNAL( lengthChanged( int ) ) ); \
        }                                                                                              \
                                                                                                       \
    protected:                                                                                         \
        __listmodel::IndicesListModelImpl* model() {                                                   \
            return &m_index;                                                                           \
        }                                                                                              \
                                                                                                       \
        int length() const {                                                                           \
            return m_index.length();                                                                   \
        }                                                                                              \
                                                                                                       \
        Q_INVOKABLE ITEM_TYPE* item( int i, bool keepOwnership = true ) const {                        \
            if ( i >= 0 && i < length() && length() > 0 ) {                                            \
                auto obj = at( i ).data();                                                             \
                if ( keepOwnership )                                                                   \
                    QQmlEngine::setObjectOwnership( obj, QQmlEngine::CppOwnership );                   \
                return obj;                                                                            \
            }                                                                                          \
            else {                                                                                     \
                return Q_NULLPTR;                                                                      \
            }                                                                                          \
        }                                                                                              \
                                                                                                       \
    Q_SIGNALS:                                                                                         \
        void lengthChanged( int length );                                                              \
    };                                                                                                 \
                                                                                                       \
    Q_DECLARE_METATYPE( NAME* )
