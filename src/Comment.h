#ifndef COMMENT_H
#define COMMENT_H

#include <qpe/quuid.h>
#include <iostream.h>
#include <qdatastream.h>
#include <qstring.h>

class Comment {

public:

    Comment( const QUuid& uid = QUuid(), const QString& text = QString() );
    Comment( const QString& text );
    Comment( const Comment& comment );
    ~Comment();

    QUuid getUid() const;

    const QString getText() const;
    void setText( const QString& text );
    bool isEmpty() const;

    bool isMarkedForDeletion() const;
    void setMarkedForDeletion( bool isMarkedForDeletion );

    friend QDataStream& operator<<( QDataStream& out, const Comment& translation );
    friend QDataStream& operator>>( QDataStream& in, Comment& translation );

private:

    QUuid   uid;
    QString text;
    
    //bool    markedForDeletion;

};

QDataStream& operator<<( QDataStream& out, const Comment& translation );
QDataStream& operator>>( QDataStream& in, Comment& translation );

#endif

