#include "Translation.h"

Translation::Translation() {
}

Translation::Translation( const QString lang, const QString word, const QString alt ) 
    : lang( lang ), word( word ), alt( alt ) {
}

Translation::~Translation() {
}

Translation::Translation( const Translation& trans )
    : lang( trans.lang ), word( trans.word ), alt( trans.alt ) {
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

QDataStream& operator<<( QDataStream& out, const Translation& translation ) {
    out << translation.lang << translation.word << translation.alt;
    return( out );
}

QDataStream& operator>>( QDataStream& in, Translation& translation ) {
    QString tempLang;
    QString tempWord;
    QString tempAlt;

    in >> tempLang >> tempWord >> tempAlt;
    translation = Translation( tempLang, tempWord, tempAlt );
    
    return( in );
}

