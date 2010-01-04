#include "TermKey.h"

TermKey::TermKey( const QUuid& termUid /* = QUuid() */, const QUuid& vocabUid /* = QUuid() */, const QUuid& firstLangTransUid /* = QUuid() */, const QUuid& testLangTransUid /* = QUuid() */ ) : termUid( termUid ), vocabUid( vocabUid ), firstLangTransUid( firstLangTransUid ), testLangTransUid( testLangTransUid ) {
}

TermKey::TermKey( const TermKey& key ) : termUid( key.termUid ), vocabUid( key.vocabUid ), firstLangTransUid( key.firstLangTransUid ), testLangTransUid( key.testLangTransUid ) {
}

TermKey::~TermKey() {
}

QUuid TermKey::getTermUid() const {
    return( termUid );
}

QUuid TermKey::getVocabUid() const {
    return( vocabUid );
}

QUuid TermKey::getFirstLanguageTranslationUid() const {
    return( firstLangTransUid );
}

QUuid TermKey::getTestLanguageTranslationUid() const {
    return( testLangTransUid );
}

bool TermKey::isNull() const {
    return( termUid.isNull() && vocabUid.isNull() && firstLangTransUid.isNull() && testLangTransUid.isNull() );
}

int TermKey::operator==( const TermKey& key ) const {
    return( termUid == key.getTermUid() && vocabUid == key.getVocabUid() && 
        firstLangTransUid == key.getFirstLanguageTranslationUid() && testLangTransUid == key.getTestLanguageTranslationUid() );
}

QDataStream& operator<<( QDataStream& out, const TermKey& key ) {
    out << key.termUid.toString() << key.vocabUid.toString() << key.firstLangTransUid << key.testLangTransUid;

    return( out );
}

QDataStream& operator>>( QDataStream& in, TermKey& key ) {
    QString tempTermUidStr;
    QUuid tempTermUid;
    QString tempVocabUidStr;
    QUuid tempVocabUid;
    QString tempFirstLangTransUidStr;
    QUuid tempFirstLangTransUid;
    QString tempTestLangTransUidStr;
    QUuid tempTestLangTransUid;

    in >> tempTermUidStr;
    tempTermUid = QUuid( tempTermUidStr );

    in >> tempVocabUidStr;
    tempVocabUid = QUuid( tempVocabUidStr );

    in >> tempFirstLangTransUidStr;
    tempFirstLangTransUid = QUuid( tempFirstLangTransUidStr );

    in >> tempTestLangTransUidStr;
    tempTestLangTransUid = QUuid( tempTestLangTransUidStr );

    key = TermKey( tempTermUid, tempVocabUid, tempFirstLangTransUid, tempTestLangTransUid );

    return( in );
}
