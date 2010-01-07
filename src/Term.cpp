#include "Term.h"

bool Term::isOldFormat = false; // Temporary flag for data conversion from 0.11.x to 0.12.x.

Term::Term( int id /* = 0 */, const QUuid& vocabUid /* = QUuid() */, const QUuid& uid /*= QUuid()*/ ) : uid( uid ), id( id ), vocabUid( vocabUid ), markedForStudy( false ), markedForDeletion( false ), imagePath( QString::null ) {
}

Term::Term( const Term& term ) : uid( term.uid ), id( term.id ), vocabUid( term.vocabUid ), markedForStudy( term.markedForStudy ), markedForDeletion( term.markedForDeletion ), imagePath( term.imagePath ) {
    for( TranslationMap::ConstIterator it = term.translations.begin(); it != term.translations.end(); it++ ) {
        const Translation& trans = it.data();
        addTranslation( trans );
    }
    for( CommentMap::ConstIterator it = term.comments.begin(); it != term.comments.end(); it++ ) {
        const BilingualKey& key = it.key();
        const Comment& comment = it.data();
        addComment( key, comment );
    }
}

Term::~Term() {
}

const QUuid Term::getUid() const {
    return( uid );
}

void Term::setUid( const QUuid& uid ) {
    this->uid = uid;
}

const int Term::getId() const {
    return( id );
}

const QUuid Term::getVocabUid() const {
    return( vocabUid );
}

void Term::setVocabUid( const QUuid& vocabUid ) {
    this->vocabUid = vocabUid;
}

//const int Term::getVocabId() const {
//    return( vocabId );
//}

bool Term::isMarkedForStudy() const {
    return( markedForStudy );
}

void Term::setMarkedForStudy( bool isMarkedForStudy ) {
    markedForStudy = isMarkedForStudy;
}

bool Term::isMarkedForDeletion() const {
    return( markedForDeletion );
}

void Term::setMarkedForDeletion( bool isMarkedForDeletion ) {
    markedForDeletion = isMarkedForDeletion;
    for( TranslationMap::Iterator it = translations.begin(); it != translations.end(); it++ ) {
        Translation& trans = it.data();
        trans.setMarkedForDeletion( isMarkedForDeletion );
    }
}

void Term::addTranslation( const Translation& translation ) {
    translations.insert( translation.getLanguage(), translation );
}

void Term::removeTranslation( const QString& language ) {
    translations.remove( language );
}

bool Term::isTranslationExists( const QString& language ) const {
    return( translations.contains( language ) );
}

Translation& Term::getTranslation( const QString& language )  {
    return( translations[ language ] );
}

Translation Term::getTranslation( const QString& language ) const {
    return( translations[ language ] );
}

Term::TranslationMap::ConstIterator Term::translationsBegin() const {
    return( translations.begin() );
}

Term::TranslationMap::ConstIterator Term::translationsEnd() const {
    return( translations.end() );
}

int Term::getTranslationCount() const {
    return( translations.count() );
}

void Term::addComment( const BilingualKey& key, const Comment& comment ) {
    comments.insert( key, comment );
}

void Term::removeComment( const BilingualKey& key ) {
    comments.remove( key );
}

void Term::removeComments( const QString& language ) {
    for( CommentMap::ConstIterator it = commentsBegin(); it != commentsEnd(); it++ ) {
        const BilingualKey& key = it.key();
        if( key.contains( language ) )
            removeComment( key );
    }
}

bool Term::isCommentExists( const BilingualKey& key ) const {
    return( comments.contains( key ) );
}

Comment& Term::getComment( const BilingualKey& key )  {
    return( comments[ key ] );
}

Comment Term::getComment( const BilingualKey& key ) const {
    return( comments[ key ] );
}

Term::CommentMap::ConstIterator Term::commentsBegin() const {
    return( comments.begin() );
}

Term::CommentMap::ConstIterator Term::commentsEnd() const {
    return( comments.end() );
}

const QString Term::getImagePath() const {
    return( imagePath );
}

void Term::setImagePath( const QString& imagePath ) {
    this->imagePath = imagePath;
}

QDataStream& operator<<( QDataStream& out, const Term& term ) {
    out << term.uid.toString();
    out << /*term.vocabUid.toString() <<*/ term.translations << term.comments << term.imagePath;
    
    return( out );
}

QDataStream& operator>>( QDataStream& in, Term& term ) {
    QString tempUidStr;
    QUuid tempUid;
    int tempId = -1;
    QString tempVocabUidStr;
    QUuid tempVocabUid;
    Term::TranslationMap tempTranslations;

    Term::CommentMap tempComments;
    Term::OldCommentMap tempOldComments;
    QString tempImagePath;
    if( Term::isOldFormat ) {
        int tempVocabId; // This value is dismissed.  The parent vocabulary will affect the vocabUid and the uid after reading all the terms. 
        in >> tempId;
        in >> tempVocabId;
        in >> tempTranslations;
        in >> tempOldComments;
        in >> tempImagePath;
    }
    else {
        in >> tempUidStr;
        tempUid = QUuid( tempUidStr );
        in >> tempTranslations;
        in >> tempComments;
        in >> tempImagePath;
    }
    term = Term( tempId, tempVocabUid, tempUid );
    for( Term::TranslationMap::ConstIterator it = tempTranslations.begin(); it != tempTranslations.end(); it++ ) {
        const Translation& trans = it.data();
        // Prevent adding empty translations.
        if( trans.getLanguage() ) 
            term.addTranslation( trans );
    }
    // Temporary for conversion from 0.11.x to 0.12.x.
    for( Term::OldCommentMap::ConstIterator it = tempOldComments.begin(); it != tempOldComments.end(); it++ ) {
        const BilingualKey& key = it.key();
        const QString& commentText = it.data();
        Comment comment( commentText ); // Uid will be assigned by the parent vocabulary.
        term.addComment( key, comment );
    }
    for( Term::CommentMap::ConstIterator it = tempComments.begin(); it != tempComments.end(); it++ ) {
        const BilingualKey& key = it.key();
        const Comment& comment = it.data();
        term.addComment( key, comment );
    }
    term.setImagePath( tempImagePath );

    return( in );
}
