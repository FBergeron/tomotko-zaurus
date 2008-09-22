#include "TranslationSelectionDialog.h"

TranslationSelectionDialog::TranslationSelectionDialog( const QString& caption, const QString& message, const QStringList& languages, int selectionMode, Controller* controller, QWidget* parent ) 
    : QDialog( parent, 0, TRUE ), controller( controller ) {
    init( caption, message, languages, selectionMode );
}

void TranslationSelectionDialog::init( const QString& caption, const QString& message, const QStringList& languages, int selectionMode ) {
    QFont mediumFont( controller->getPreferences().getMediumFont() );

    messageLabel = new QLabel( message, this, "MessageLabel" );

    languageList = new SmartListView( this, "LanguageList" ); 
    languageList->setStretchColumn( 0 );
    languageList->setAllColumnsShowFocus( true );
    languageList->addColumn( tr( "Languages" ) );
    for( QStringList::ConstIterator it = languages.begin(); it != languages.end(); it++ ) {
        QString lang = *it;
        QCheckListItem* item = new QCheckListItem( languageList, QObject::tr( lang ), QCheckListItem::CheckBox );
        switch( selectionMode ) {
            case selectionModeTargetLanguage : 
                if( lang == controller->getPreferences().getTestLanguage() )
                    item->setOn( true );
                break;
            case selectionModeStudyLanguages :
                if( controller->getPreferences().isStudyLanguage( lang ) )
                    item->setOn( true );
                break;
            case selectionModeAllLanguages :
                item->setOn( true );
                break;
        }
        languageList->insertItem( item );
    }
    checkAllLanguagesButton = new QPushButton( tr( "CheckAllLanguages" ), this, "CheckAllLanguagesButton" );
    connect( checkAllLanguagesButton, SIGNAL( clicked() ), this, SLOT( checkAllLanguages() ) );

    mainLayout = new QVBoxLayout( this );
    mainLayout->setMargin( 10 );
    mainLayout->setSpacing( 2 );
    mainLayout->addWidget( messageLabel );
    mainLayout->addWidget( languageList, 1 );
    mainLayout->addWidget( checkAllLanguagesButton );

    setCaption( caption );
}

TranslationSelectionDialog::~TranslationSelectionDialog() {
}

QStringList TranslationSelectionDialog::getSelectedLanguages() {
    QStringList selectedLanguages;
    for( QCheckListItem* item = (QCheckListItem*)languageList->firstChild(); item; item = (QCheckListItem*)item->nextSibling() ) {
        if( item->isOn() )
            selectedLanguages.append( Util::getLanguageCode( item->text() ) );
    }
    return( selectedLanguages );
}

void TranslationSelectionDialog::checkAllLanguages() const {
    for( QCheckListItem* item = (QCheckListItem*)languageList->firstChild(); item; item = (QCheckListItem*)item->nextSibling() ) {
        item->setOn( true );
    }
}
