#ifndef STANDBY_TERM_H
#define STANDBY_TERM_H

#include <iostream.h>
#include <stdlib.h>
#include <qdatastream.h>
#include "TermKey.h"


class StandbyTerm {

public:

    StandbyTerm();
    StandbyTerm( const TermKey& termKey, uint pool );
    StandbyTerm( const StandbyTerm& standbyTerm );
    ~StandbyTerm();

    TermKey getKey() const;
    uint getPool() const;
    
    int operator==( const StandbyTerm& term ) const;

private:

    TermKey key;
    uint pool;

    friend QDataStream& operator<<( QDataStream& out, const StandbyTerm& term );
    friend QDataStream& operator>>( QDataStream& in, StandbyTerm& term );

};

#endif

