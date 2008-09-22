#ifndef BILINGUAL_KEY_H
#define BILINGUAL_KEY_H

#include <qdatastream.h>
#include <qstring.h>

class BilingualKey {

public:

    BilingualKey();
    BilingualKey( const QString& stringKey );
    BilingualKey( const QString& firstLanguage, const QString& secondLanguage );
    BilingualKey( const BilingualKey& key );
    ~BilingualKey();
   
    const QString& getFirstLanguage() const;
    const QString& getSecondLanguage() const;
    bool contains( const QString& language ) const;
    QString toString() const;

    BilingualKey& operator=( const BilingualKey& key );
    friend bool operator==( const BilingualKey& left, const BilingualKey& right );
    friend bool operator<( const BilingualKey& left, const BilingualKey& right );

    friend QDataStream& operator<<( QDataStream& out, const BilingualKey& key );
    friend QDataStream& operator>>( QDataStream& in, BilingualKey& key );

private:
   
    QString lang1;
    QString lang2;

};

QDataStream& operator<<( QDataStream& out, const BilingualKey& key );
QDataStream& operator>>( QDataStream& in, BilingualKey& key );

#endif

