#include "LanguageUpdateNotifier.h"

void LanguageUpdateNotifier::addLanguage( const QString& language ) {
    emit languageAdded( language );
}

void LanguageUpdateNotifier::removeLanguage( const QString& language ) {
    emit languageRemoved( language );
}
