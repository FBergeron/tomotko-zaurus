#include "TermKey.h"

TermKey::TermKey( const QUuid& termUid /* = QUuid() */, const QUuid& vocabUid /* = QUuid() */ ) : termUid( termUid ), vocabUid( vocabUid ) {
}

TermKey::TermKey( const TermKey& key ) : termUid( key.termUid ), vocabUid( key.vocabUid ) {
}

TermKey::~TermKey() {
}

QUuid TermKey::getTermUid() const {
    return( termUid );
}

QUuid TermKey::getVocabUid() const {
    return( vocabUid );
}

bool TermKey::isNull() const {
    return( termUid.isNull() && vocabUid.isNull() );
}

int TermKey::operator==( const TermKey& key ) const {
    return( termUid == key.getTermUid() && vocabUid == key.getVocabUid() );
}

QDataStream& operator<<( QDataStream& out, const TermKey& key ) {
    out << key.termUid.toString() << key.vocabUid.toString();

    return( out );
}

QDataStream& operator>>( QDataStream& in, TermKey& key ) {
    QString tempTermUidStr;
    QUuid tempTermUid;
    QString tempVocabUidStr;
    QUuid tempVocabUid;

    in >> tempTermUidStr;
    tempTermUid = QUuid( tempTermUidStr );

    in >> tempVocabUidStr;
    tempVocabUid = QUuid( tempVocabUidStr );

    key = TermKey( tempTermUid, tempVocabUid );

    return( in );
}
