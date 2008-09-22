#include "StandbyTerm.h"

StandbyTerm::StandbyTerm() {
}

StandbyTerm::StandbyTerm( const TermKey& termKey, uint pool ) : key( termKey ), pool( pool ) {
}

StandbyTerm::StandbyTerm( const StandbyTerm& standbyTerm ) : key( standbyTerm.key ), pool( standbyTerm.pool ) {
}

StandbyTerm::~StandbyTerm() {
}

TermKey StandbyTerm::getKey() const {
    return( key );
}

uint StandbyTerm::getPool() const {
    return( pool );
}

int StandbyTerm::operator==( const StandbyTerm& term ) const {
    return( key == term.getKey() && pool == term.getPool() );
}

QDataStream& operator<<( QDataStream& out, const StandbyTerm& term ) {
    out << term.key << term.pool;

    return( out );
}

QDataStream& operator>>( QDataStream& in, StandbyTerm& term ) {
    TermKey tempTermKey;
    uint tempPool;

    in >> tempTermKey >> tempPool;

    term = StandbyTerm( tempTermKey, tempPool );

    return( in );
}
