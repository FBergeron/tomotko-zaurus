#include "BiUidKey.h"

BiUidKey::BiUidKey() {
}

BiUidKey::BiUidKey( const QString& stringKey ) 
    : firstKey( stringKey.left( 38 ) ), secondKey( stringKey.right( 38 ) ) {
}

BiUidKey::BiUidKey( const QString& firstUid, const QString& secondUid ) {
    // Sort the uids.
    if( firstUid.compare( secondUid ) < 0 ) {
        firstKey = firstUid;
        secondKey = secondUid;
    }
    else {
        firstKey = secondUid;
        secondKey = firstUid;
    }
}

BiUidKey::BiUidKey( const BiUidKey& key )
    : firstKey( key.firstKey ), secondKey( key.secondKey ) {
}

BiUidKey::~BiUidKey() {
}

const QString& BiUidKey::getFirstUid() const {
    return( firstKey );
}

const QString& BiUidKey::getSecondUid() const {
    return( secondKey );
}

bool BiUidKey::isNull() const {
    return( firstKey.isNull() && secondKey.isNull() );
}

bool BiUidKey::contains( const QString& uid ) const {
    return( firstKey == uid || secondKey == uid );
}

QString BiUidKey::toString() const {
    return( QString( firstKey + QString( "|" ) + secondKey ) );
}

BiUidKey& BiUidKey::operator=( const BiUidKey& key ) {
    if( this != &key ) {
        firstKey = key.getFirstUid();
        secondKey = key.getSecondUid();
    }
    return( *this );
}

bool operator==( const BiUidKey& left, const BiUidKey& right ) {
    return( left.toString().compare( right.toString() ) == 0 );
}

bool operator<( const BiUidKey& left, const BiUidKey& right ) {
    return( left.toString().compare( right.toString() ) < 0 );
}

QDataStream& operator<<( QDataStream& out, const BiUidKey& key ) {
    out << key.firstKey << key.secondKey;
    return( out );
}

QDataStream& operator>>( QDataStream& in, BiUidKey& key ) {
    QString tempFirstKey;
    QString tempSecondKey;

    in >> tempFirstKey >> tempSecondKey;
    key = BiUidKey( tempFirstKey, tempSecondKey );
    
    return( in );
}

