#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <iostream.h>
#include <qdatastream.h>
#include <qstring.h>

class Translation {

public:

    Translation();
    Translation( const QString lang, const QString word = QString(), const QString alt = QString() );
    Translation( const Translation& trans );
    ~Translation();

    const QString getLanguage() const;
    void setLanguage( const QString& language );

    const QString getWord() const;
    void setWord( const QString& word );

    const QString getAlt() const;
    void setAlt( const QString& alt );

    friend QDataStream& operator<<( QDataStream& out, const Translation& translation );
    friend QDataStream& operator>>( QDataStream& in, Translation& translation );

private:

    QString lang;
    QString word;
    QString alt;

};

QDataStream& operator<<( QDataStream& out, const Translation& translation );
QDataStream& operator>>( QDataStream& in, Translation& translation );

#endif
