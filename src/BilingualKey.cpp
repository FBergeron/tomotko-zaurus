#include "BilingualKey.h"

BilingualKey::BilingualKey() {
}

BilingualKey::BilingualKey( const QString& stringKey ) 
    : lang1( stringKey.left( 2 ) ), lang2( stringKey.right( 2 ) ) {
}

BilingualKey::BilingualKey( const QString& firstLanguage, const QString& secondLanguage ) {
    // Sort the languages.
    if( firstLanguage.compare( secondLanguage ) < 0 ) {
        lang1 = firstLanguage;
        lang2 = secondLanguage;
    }
    else {
        lang1 = secondLanguage;
        lang2 = firstLanguage;
    }
}

BilingualKey::BilingualKey( const BilingualKey& key )
    : lang1( key.lang1 ), lang2( key.lang2 ) {
}

BilingualKey::~BilingualKey() {
}

const QString& BilingualKey::getFirstLanguage() const {
    return( lang1 );
}

const QString& BilingualKey::getSecondLanguage() const {
    return( lang2 );
}

bool BilingualKey::contains( const QString& language ) const {
    return( lang1 == language || lang2 == language );
}

QString BilingualKey::toString() const {
    return( QString( lang1 + QString( "-" ) + lang2 ) );
}

BilingualKey& BilingualKey::operator=( const BilingualKey& key ) {
    if( this != &key ) {
        lang1 = key.getFirstLanguage();
        lang2 = key.getSecondLanguage();
    }
    return( *this );
}

bool operator==( const BilingualKey& left, const BilingualKey& right ) {
    return( left.toString().compare( right.toString() ) == 0 );
}

bool operator<( const BilingualKey& left, const BilingualKey& right ) {
    return( left.toString().compare( right.toString() ) < 0 );
}

QDataStream& operator<<( QDataStream& out, const BilingualKey& key ) {
    out << key.lang1 << key.lang2;
    return( out );
}

QDataStream& operator>>( QDataStream& in, BilingualKey& key ) {
    QString tempLang1;
    QString tempLang2;

    in >> tempLang1 >> tempLang2;
    key = BilingualKey( tempLang1, tempLang2 );
    
    return( in );
}
