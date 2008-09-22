#ifndef TERM_KEY_H
#define TERM_KEY_H

#include <iostream.h>
#include <qdatastream.h>

class TermKey {

public:

    TermKey( int termId = -1, int vocabId = -1 );
    TermKey( const TermKey& key );
    ~TermKey();

    int getTermId() const;
    int getVocabId() const;
    bool isNull() const;

    int operator==( const TermKey& key ) const;

    friend QDataStream& operator<<( QDataStream& out, const TermKey& key );
    friend QDataStream& operator>>( QDataStream& in, TermKey& key );

private:

    int termId;
    int vocabId;

};

#endif

