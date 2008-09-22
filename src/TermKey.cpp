#include "TermKey.h"

TermKey::TermKey( int termId, int vocabId ) : termId( termId ), vocabId( vocabId ) {
}

TermKey::TermKey( const TermKey& key ) : termId( key.termId ), vocabId( key.vocabId ) {
}

TermKey::~TermKey() {
}

int TermKey::getTermId() const {
    return( termId );
}

int TermKey::getVocabId() const {
    return( vocabId );
}

bool TermKey::isNull() const {
    return( termId == -1 && vocabId == -1 );
}

int TermKey::operator==( const TermKey& key ) const {
    return( termId == key.getTermId() && vocabId == key.getVocabId() );
}

QDataStream& operator<<( QDataStream& out, const TermKey& key ) {
    out << key.termId << key.vocabId;

    return( out );
}

QDataStream& operator>>( QDataStream& in, TermKey& key ) {
    int tempTermId;
    int tempVocabId;

    in >> tempTermId >> tempVocabId;

    key = TermKey( tempTermId, tempVocabId );

    return( in );
}
