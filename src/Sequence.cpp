#include "Sequence.h"

const int Sequence::MAX_ITEM = 5;
const QString Sequence::itemString[] = { "a", "b", "c", "d", "e" };

Sequence::Sequence( const QString& seqStr = QString::null ) : enabled( false ) {
    if( seqStr.isEmpty() )
        return;

    QString tempStr = seqStr;
    int indexOfColon = seqStr.find( ":" );
    if( indexOfColon != -1 ) {
        QString enabledStr = seqStr.mid( indexOfColon + 1 );
        enabled = ( enabledStr == "on" );
        tempStr = seqStr.mid( 0, indexOfColon );
    }

    QStringList groupStrings = QStringList::split( "-", tempStr );
    for( QStringList::ConstIterator it = groupStrings.begin(); it != groupStrings.end(); it++ ) {
        QStringList itemStrings = QStringList::split( "+", *it );
        ItemList itemList;
        for( QStringList::ConstIterator it2 = itemStrings.begin(); it2 != itemStrings.end(); it2++ ) {
            bool isOk;
            int item = (*it2).toInt( &isOk );
            if( isOk )
                itemList.append( (Item)item );
        }
        groups.append( itemList );
    }
}

Sequence::Sequence( const Sequence& seq ) : enabled( seq.enabled ) {
    for( QValueList<ItemList>::ConstIterator it = seq.groups.begin(); it != seq.groups.end(); it++ ) {
        ItemList itemList;

        ItemList seqItemList = (ItemList)(*it);
        for( ItemList::ConstIterator it2 = seqItemList.begin(); it2 != seqItemList.end(); it2++ ) {
            Item seqItem = (Item)(*it2);
            itemList.append( seqItem ); 
        }

        groups.append( itemList );
    }
}

Sequence::~Sequence() {
}

bool Sequence::isEnabled() const {
    return( enabled );
}

void Sequence::setEnabled( bool isEnabled ) {
    enabled = isEnabled;
}

bool Sequence::contains( const Item& item ) const {
    for( QValueList<ItemList>::ConstIterator it = groups.begin(); it != groups.end(); it++ ) {
        ItemList itemList = (ItemList)(*it);
        if( itemList.contains( item ) )
            return( true );
    }
    return( false ); 
}

bool Sequence::isEmpty() const {
    return( groups.isEmpty() );
}

void Sequence::addGroup( ItemList group ) {
    groups.append( group );
}

void Sequence::removeLastGroup() {
    QValueList<ItemList>::Iterator it = groups.fromLast();
    groups.remove( it );
}

Sequence::ItemList Sequence::getGroupAt( int index ) const {
    return( groups[ index ] );
}

int Sequence::getGroupCount() const {
    return( groups.count() );
}

Sequence::Item Sequence::stringToItem( const QString& itemStr ) {
    for( int i = 0; i < Sequence::MAX_ITEM; i++ ) {
        if( itemStr == Sequence::itemString[ i ] ) 
            return( (Item)i );
    }
    return( INVALID ); 
}

QString Sequence::toString() const {
    QString str;
    QString groupDelimiter;
    for( QValueList<ItemList>::ConstIterator it = groups.begin(); it != groups.end(); it++ ) {
        ItemList itemList = (ItemList)(*it);
        qHeapSort( itemList );
        str += groupDelimiter;
        QString itemDelimiter;
        for( ItemList::ConstIterator it2 = itemList.begin(); it2 != itemList.end(); it2++ ) {
            Item item = (Item)(*it2);
            str += itemDelimiter + QString::number( item );
            itemDelimiter = "+";
        }
        groupDelimiter = "-";
    }
    str += QString( ":" ) + ( enabled ? "on" : "off" );
    return( str );
}

QString Sequence::toHumanReadableString() const {
    QString str;
    QString groupDelimiter;
    for( QValueList<ItemList>::ConstIterator it = groups.begin(); it != groups.end(); it++ ) {
        ItemList itemList = (ItemList)(*it);
        qHeapSort( itemList );
        str += groupDelimiter;
        QString itemDelimiter;
        for( ItemList::ConstIterator it2 = itemList.begin(); it2 != itemList.end(); it2++ ) {
            Item item = (Item)(*it2);
            str += itemDelimiter + itemString[ item ];
            itemDelimiter = "+";
        }
        groupDelimiter = " > ";
    }
    return( str );
}

QDataStream& operator<<( QDataStream& out, const Sequence& sequence ) {
    int enabledAsInt = ( sequence.enabled ? 1 : 0 );
    out << enabledAsInt;
  
    out << sequence.groups.count();
    for( QValueList<Sequence::ItemList>::ConstIterator it = sequence.groups.begin(); it != sequence.groups.end(); it++ ) {
        const Sequence::ItemList itemList = (const Sequence::ItemList)(*it);
        out << itemList.count();
        for( Sequence::ItemList::ConstIterator it2 = itemList.begin(); it2 != itemList.end(); it2++ ) {
            const Sequence::Item item = (const Sequence::Item)(*it2);
            out << (int)item;
        }
    }

    return( out );
}

QDataStream& operator>>( QDataStream& in, Sequence& sequence ) {
    int tempEnabledAsInt;
    QValueList<Sequence::ItemList> tempGroups;

    in >> tempEnabledAsInt;
    int tempItemListCount;
    in >> tempItemListCount;
    for( int i = 0; i < tempItemListCount; i++ ) {
        int tempItemCount;
        in >> tempItemCount;
        Sequence::ItemList itemList;
        for( int j = 0; j < tempItemCount; j++ ) {
            int tempItemAsInt;
            in >> tempItemAsInt;
            itemList.append( (Sequence::Item)tempItemAsInt );
        }
        tempGroups.append( itemList );
    }

    sequence = Sequence();
    sequence.enabled = ( tempEnabledAsInt == 1 );
    sequence.groups = tempGroups;

    return( in );
}
