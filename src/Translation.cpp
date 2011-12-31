#include "Translation.h"

//bool Translation::isOldFormat = false; // Temporary flag for data conversion from 0.11.x to 0.12.x.
bool Translation::isOldFormat = true; // Temporary flag for data conversion from 0.11.x to 0.12.x.

Translation::Translation( const QUuid& uid /* = QUuid() */, const QString& lang /* = QString() */, const QString& word /* = QString() */, const QString& alt /* = QString() */ ) 
    : uid( uid ), lang( lang ), word( word ), alt( alt ), markedForDeletion( false ) {
}

Translation::Translation( const QString lang, const QString word /* = QString() */, const QString alt /* = QString() */ ) 
    : uid( QUuid() ), lang( lang ), word( word ), alt( alt ), markedForDeletion( false ) {
}

Translation::~Translation() {
}

Translation::Translation( const Translation& trans )
    : uid( trans.uid), lang( trans.lang ), word( trans.word ), alt( trans.alt ), markedForDeletion( trans.markedForDeletion ) {
}

QUuid Translation::getUid() const {
    return( uid );
}

const QString Translation::getLanguage() const {
    return( lang );
}

void Translation::setLanguage( const QString& language ) {
    this->lang = language;
}

const QString Translation::getWord() const {
    return( word );
}

void Translation::setWord( const QString& word ) {
    this->word = word;
}

const QString Translation::getAlt() const {
    return( alt );
}

void Translation::setAlt( const QString& alt ) {
    this->alt = alt;
}

bool Translation::isMarkedForDeletion() const {
    return( markedForDeletion );
}

void Translation::setMarkedForDeletion( bool isMarkedForDeletion ) {
    markedForDeletion = isMarkedForDeletion;
}

QDataStream& operator<<( QDataStream& out, const Translation& translation ) {
    out << translation.uid.toString() << translation.lang << translation.word << translation.alt;
    return( out );
}

QDataStream& operator>>( QDataStream& in, Translation& translation ) {
#ifdef DEBUG
    cout << "Translation.operator>> Translation::isOldFormat=" << Translation::isOldFormat << endl;
#endif
    QString tempUid;
    QString tempLang;
    QString tempWord;
    QString tempAlt;

    if( Translation::isOldFormat ) {
        // There is no uid in this version.  The uid will be generated by the parent vocabulary.
        in >> tempLang >> tempWord >> tempAlt;
#ifdef DEBUG
        cout << "tempLang=" << tempLang << " tempWord=" << tempWord << " tempAlt=" << tempAlt << endl;
#endif
    }
    else {
        in >> tempUid;
#ifdef DEBUG
        cout << "tempUid=" << tempUid << endl;
#endif
        in >> tempLang >> tempWord >> tempAlt;
#ifdef DEBUG
        cout << "tempLang=" << tempLang << " tempWord=" << tempWord << " tempAlt=" << tempAlt << endl;
#endif
    }
    translation = Translation( QUuid( tempUid ), tempLang, tempWord, tempAlt );
    
    return( in );
}

