#include "Term.h"

Term::Term( int id = 0, int vocabId = 0 ) : id( id ), vocabId( vocabId ), markedForStudy( FALSE ), imagePath( QString::null ) {
}

Term::Term( const Term& term ) : id( term.id ), vocabId( term.vocabId ), markedForStudy( term.markedForStudy ), imagePath( term.imagePath ) {
    for( TranslationMap::ConstIterator it = term.translations.begin(); it != term.translations.end(); it++ ) {
        const Translation& trans = it.data();
        addTranslation( trans );
    }
    for( CommentMap::ConstIterator it = term.comments.begin(); it != term.comments.end(); it++ ) {
        const BilingualKey& key = it.key();
        const QString& comment = it.data();
        addComment( key, comment );
    }
}

Term::~Term() {
}

const int Term::getId() const {
    return( id );
}

const int Term::getVocabId() const {
    return( vocabId );
}

bool Term::isMarkedForStudy() const {
    return( markedForStudy );
}

void Term::setMarkedForStudy( bool isMarkedForStudy ) {
    markedForStudy = isMarkedForStudy;
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

void Term::addComment( const BilingualKey& key, const QString& comment ) {
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

QString& Term::getComment( const BilingualKey& key )  {
    return( comments[ key ] );
}

QString Term::getComment( const BilingualKey& key ) const {
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
    out << term.id << term.vocabId << term.translations << term.comments << term.imagePath;
    return( out );
}

QDataStream& operator>>( QDataStream& in, Term& term ) {
    int tempId;
    int tempVocabId;
    Term::TranslationMap tempTranslations;
    Term::CommentMap tempComments;
    QString tempImagePath;

    in >> tempId >> tempVocabId >> tempTranslations >> tempComments;
    in >> tempImagePath;

    term = Term( tempId, tempVocabId );
    for( Term::TranslationMap::ConstIterator it = tempTranslations.begin(); it != tempTranslations.end(); it++ ) {
        const Translation& trans = it.data();
        term.addTranslation( trans );
    }
    for( Term::CommentMap::ConstIterator it = tempComments.begin(); it != tempComments.end(); it++ ) {
        const BilingualKey& key = it.key();
        const QString& comment = it.data();
        term.addComment( key, comment );
    }
    term.setImagePath( tempImagePath );

    return( in );
}

