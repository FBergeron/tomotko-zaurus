#include "StudyLanguageItem.h"

StudyLanguageItem::StudyLanguageItem( QListView* parent, LanguageUpdateNotifier* languageUpdateNotifier )
    : QCheckListItem( parent, "StudyLanguageItem", CheckBox ), languageUpdateNotifier( languageUpdateNotifier ) {
    setSelectable( false );
}

StudyLanguageItem::~StudyLanguageItem() {
}

void StudyLanguageItem::stateChange( bool isOn ) {
    QCheckListItem::stateChange( isOn );
    if( isOn )
        languageUpdateNotifier->addLanguage( Util::getLanguageCode( text() ) );
    else
        languageUpdateNotifier->removeLanguage( Util::getLanguageCode( text() ) );
}
