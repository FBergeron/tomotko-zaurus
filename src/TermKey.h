#ifndef TERM_KEY_H
#define TERM_KEY_H

#include <qpe/quuid.h>
#include <iostream.h>
#include <qdatastream.h>

class TermKey {

public:

    TermKey( const QUuid& termUid = QUuid(), const QUuid& vocabUid = QUuid() );
    TermKey( const TermKey& key );
    ~TermKey();

    QUuid getTermUid() const;
    QUuid getVocabUid() const;
    bool isNull() const;

    int operator==( const TermKey& key ) const;

    friend QDataStream& operator<<( QDataStream& out, const TermKey& key );
    friend QDataStream& operator>>( QDataStream& in, TermKey& key );

private:

    QUuid termUid;
    QUuid vocabUid;

};

#endif

