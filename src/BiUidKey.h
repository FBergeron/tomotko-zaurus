#ifndef BI_UID_KEY_H
#define BI_UID_KEY_H

#include <qdatastream.h>
#include <qstring.h>

class BiUidKey {

public:

    BiUidKey();
    BiUidKey( const QString& stringKey );
    BiUidKey( const QString& firstUid, const QString& secondUid );
    BiUidKey( const BiUidKey& key );
    ~BiUidKey();
   
    const QString& getFirstUid() const;
    const QString& getSecondUid() const;
    bool isNull() const;
    bool contains( const QString& uid ) const;
    QString toString() const;

    BiUidKey& operator=( const BiUidKey& key );
    friend bool operator==( const BiUidKey& left, const BiUidKey& right );
    friend bool operator<( const BiUidKey& left, const BiUidKey& right );

    friend QDataStream& operator<<( QDataStream& out, const BiUidKey& key );
    friend QDataStream& operator>>( QDataStream& in, BiUidKey& key );

private:
   
    QString firstKey;
    QString secondKey;

};

QDataStream& operator<<( QDataStream& out, const BiUidKey& key );
QDataStream& operator>>( QDataStream& in, BiUidKey& key );

#endif


