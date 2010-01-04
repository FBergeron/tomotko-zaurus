#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <qpe/quuid.h>
#include <iostream.h>
#include <qdatastream.h>
#include <qstring.h>

class Translation {

public:

    static bool isOldFormat; // Temporary flag for data conversion from 0.11.x to 0.12.x.

    Translation( const QUuid& uid = QUuid(), const QString& lang = QString(), const QString& word = QString(), const QString& alt = QString() );
    Translation( const QString lang, const QString word = QString(), const QString alt = QString() );
    Translation( const Translation& trans );
    ~Translation();

    QUuid getUid() const;

    const QString getLanguage() const;
    void setLanguage( const QString& language );

    const QString getWord() const;
    void setWord( const QString& word );

    const QString getAlt() const;
    void setAlt( const QString& alt );

    bool isMarkedForDeletion() const;
    void setMarkedForDeletion( bool isMarkedForDeletion );

    friend QDataStream& operator<<( QDataStream& out, const Translation& translation );
    friend QDataStream& operator>>( QDataStream& in, Translation& translation );

private:

    QUuid   uid;
    QString lang;
    QString word;
    QString alt;
    
    bool markedForDeletion;

};

QDataStream& operator<<( QDataStream& out, const Translation& translation );
QDataStream& operator>>( QDataStream& in, Translation& translation );

#endif
