#include "Comment.h"

Comment::Comment( const QUuid& uid /* = QUuid() */, const QString& text /* = QString() */ ) 
    : uid( uid ), text( text ), markedForDeletion( false ) {
}

Comment::Comment( const QString& text )
    : uid( QUuid() ), text( text ), markedForDeletion( false ) {
}

Comment::~Comment() {
}

Comment::Comment( const Comment& comment )
    : uid( comment.uid), text( comment.text ), markedForDeletion( comment.markedForDeletion ) {
}

QUuid Comment::getUid() const {
    return( uid );
}

const QString Comment::getText() const {
    return( text );
}

void Comment::setText( const QString& text ) {
    this->text = text;
}

bool Comment::isMarkedForDeletion() const {
    return( markedForDeletion );
}

void Comment::setMarkedForDeletion( bool isMarkedForDeletion ) {
    markedForDeletion = isMarkedForDeletion;
}

QDataStream& operator<<( QDataStream& out, const Comment& comment ) {
    out << comment.uid << comment.text;
    return( out );
}

QDataStream& operator>>( QDataStream& in, Comment& comment ) {
    QString tempUid;
    QString tempText;

    in >> tempUid >> tempText;
    comment = Comment( tempUid, tempText );
    
    return( in );
}
