#include "SearchDialog.h"
#include "icons/editTerm.xpm"
#include "icons/goVocab.xpm"
#include "icons/removeTerm.xpm"
#include "icons/resetQueryForm.xpm"
#include "icons/search.xpm"

SearchDialog::SearchDialog( QWidget* parent, Controller* controller )
    : QDialog( parent, "SearchDialog", true ), controller( controller ) {
    init();
    updateFonts();
}

SearchDialog::~SearchDialog() {
}

void SearchDialog::init() {
    queryPanel = new QHBox( this );
    queryPanel->setSpacing( 2 );
    resetButton = new QPushButton( queryPanel, "ResetButton" );
    resetButton->setPixmap( ZPIXMAP( resetQueryForm_xpm ) );
    connect( resetButton, SIGNAL( clicked() ), this, SLOT( reset() ) );
    queryField = new HistoryField( queryPanel, "QueryField" );
    queryField->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
    searchButton = new QPushButton( ZPIXMAP( search_xpm ), tr( "Search" ), queryPanel, "SearchButton" );
    searchButton->setDefault( true );
    connect( searchButton, SIGNAL( clicked() ), queryField->lineEdit(), SIGNAL( returnPressed() ) );
    connect( searchButton, SIGNAL( clicked() ), this, SLOT( search() ) );

    int prefHeight = resetButton->sizeHint().height();
    queryField->setMaximumHeight( prefHeight );
    searchButton->setMaximumHeight( prefHeight ); 

    resultsHeaderPanel = new QHBox( this );
    resultsHeaderPanel->setSpacing( 2 );
    resultsHeaderLabel = new QLabel( tr( "Results" ), resultsHeaderPanel );
    resultsHeaderFiller = new QWidget( resultsHeaderPanel, "ResultsHeaderFiller" );
    resultsCounterLabel = new QLabel( resultsHeaderPanel );
    resultsCounterLabel->setAlignment( AlignRight );

    resultsListView = new SmartListView( this, "ResultsListView" );
    int colFirstLanguage = resultsListView->addColumn( QObject::tr( controller->getPreferences().getFirstLanguage() ) );
    int colTestLanguage = resultsListView->addColumn( QObject::tr( controller->getPreferences().getTestLanguage() ) );
    resultsListView->setColumnWidthMode( colFirstLanguage, QListView::Manual );
    resultsListView->setColumnWidthMode( colTestLanguage, QListView::Manual );
    resultsListView->addColumn( tr( "Glossary" ) );
    resultsListView->addColumn( tr( "Location" ) );
    resultsListView->setAllColumnsWide( true );
    resultsListView->setAllColumnsShowFocus( true );
    //resultsListView->setStretchColumn( 1 );
    resultsListView->setSelectionMode( QListView::Extended );
    connect( resultsListView, SIGNAL( selectionChanged() ), this, SLOT( updateUi() ) );

    resultsButtonsPanel = new QHBox( this );
    resultsButtonsPanel->setSpacing( 2 );
    goResultVocabButton = new QPushButton( ZPIXMAP( goVocab_xpm ), tr( "View Glossary" ), resultsButtonsPanel, "GoResultVocabButton" );
    connect( goResultVocabButton, SIGNAL( clicked() ), this, SLOT( goResultVocab() ) );
    editResultTermButton = new QPushButton( ZPIXMAP( editTerm_xpm ), tr( "Edit Term" ), resultsButtonsPanel, "EditResultTermButton" );
    connect( editResultTermButton, SIGNAL( clicked() ), this, SLOT( editResultTerm() ) );
    removeResultTermButton = new QPushButton( ZPIXMAP( removeTerm_xpm ), tr( "Remove Term(s)" ), resultsButtonsPanel, "RemoveTermButton" );
    connect( removeResultTermButton, SIGNAL( clicked() ), this, SLOT( removeResultTerms() ) );

    prefHeight = removeResultTermButton->sizeHint().height();
    goResultVocabButton->setMaximumHeight( prefHeight );
    editResultTermButton->setMaximumHeight( prefHeight );

    mainLayout = new QVBoxLayout( this );
    mainLayout->setMargin( 6 );
    mainLayout->setSpacing( 2 );
    mainLayout->addWidget( queryPanel );
    mainLayout->addWidget( resultsHeaderPanel );
    mainLayout->addWidget( resultsListView, 1 );
    mainLayout->addWidget( resultsButtonsPanel );

    setCaption( tr( "Search..." ) );

    updateUi();
}

void SearchDialog::show() {
    Preferences& prefs = controller->getPreferences();
    if( Util::getLanguageCode( resultsListView->columnText( 0 ) ) != prefs.getFirstLanguage() ||
        Util::getLanguageCode( resultsListView->columnText( 1 ) ) != prefs.getTestLanguage() ) {
        resultsListView->setColumnText( 0, QObject::tr( prefs.getFirstLanguage() ) );
        resultsListView->setColumnText( 1, QObject::tr( prefs.getTestLanguage() ) );
        reset();
    }
    QDialog::show();
    queryField->setFocus();
}

void SearchDialog::updateFonts() {
    QFont mediumFont( controller->getPreferences().getMediumFont() );
    QFont labelsFont( controller->getPreferences().getLabelsFont() );

    queryField->setFont( mediumFont );
    searchButton->setFont( labelsFont );
    resetButton->setFont( labelsFont );

    resultsHeaderPanel->setFont( labelsFont );
    resultsCounterLabel->setFont( labelsFont );

    QFont firstLangFont = controller->getPreferences().getMediumFont( controller->getPreferences().getFirstLanguage() );
    QFont testLangFont = controller->getPreferences().getMediumFont( controller->getPreferences().getTestLanguage() );
    resultsListView->setFont( mediumFont );
    resultsListView->header()->setFont( labelsFont );
    for( ResultListItem* item = (ResultListItem*)resultsListView->firstChild(); item; item = (ResultListItem*)item->nextSibling() ) {
        item->setFont( 0, firstLangFont );
        item->setFont( 1, testLangFont );
    }

    goResultVocabButton->setFont( labelsFont );
    editResultTermButton->setFont( labelsFont );
    removeResultTermButton->setFont( labelsFont );
}

void SearchDialog::updateUi() {
    uint selectedTermCount = getSelectedTermCount();
    goResultVocabButton->setEnabled( selectedTermCount == 1 );
    editResultTermButton->setEnabled( selectedTermCount == 1 );
    removeResultTermButton->setEnabled( selectedTermCount > 0 );
}

void SearchDialog::retranslateUi() {
    searchButton->setText( tr( "Search" ) );
    resultsHeaderLabel->setText( tr( "Results" ) );
    resultsCounterLabel->setText( tr( "%1 term(s) found" ).arg( controller->getSearchResultsCount() ) );
    resultsListView->setColumnText( 0, QObject::tr( controller->getPreferences().getFirstLanguage() ) );
    resultsListView->setColumnText( 1, QObject::tr( controller->getPreferences().getTestLanguage() ) );
    resultsListView->setColumnText( 2, tr( "Glossary" ) );
    resultsListView->setColumnText( 3, tr( "Location" ) );
    goResultVocabButton->setText( tr( "View Glossary" ) );
    editResultTermButton->setText( tr( "Edit Term" ) );
    removeResultTermButton->setText( tr( "Remove Term(s)" ) );
    goResultVocabButton->adjustSize();
    editResultTermButton->adjustSize();
    removeResultTermButton->adjustSize();
    setCaption( tr( "Search..." ) );

    updateUi();
}

void SearchDialog::search() {
    const Preferences& prefs = controller->getPreferences();
    QValueList<TermKey> results = controller->search( queryField->currentText() );
    resultsListView->clear();
    for( QValueList<TermKey>::ConstIterator it = results.begin(); it != results.end(); it++ ) {
        const TermKey& termKey = *it;
        Term* term = ( prefs.isLanguageFilterEnabled() ? 
            controller->getTerm( termKey, prefs.getFirstLanguage(), prefs.getTestLanguage() ) : controller->getTerm( termKey ) );
        Vocabulary* vocab = controller->getVocabTree()->getVocabulary( termKey.getVocabUid() );
        if( vocab ) {
            ResultListItem* resultItem = new ResultListItem( resultsListView, term, 
                prefs.getFirstLanguage(), prefs.getTestLanguage(), vocab->getTitle(), vocab->getParent()->getHumanReadablePath(),
                    prefs.isAltInTermListShown() );
            resultItem->setFont( 0, prefs.getMediumFont( prefs.getFirstLanguage() ) );
            resultItem->setFont( 1, prefs.getMediumFont( prefs.getTestLanguage() ) );
        }
    }
    resultsListView->verticalScrollBar()->setValue( 0 );
    resultsCounterLabel->setText( tr( "%1 term(s) found" ).arg( results.count() ) );
    updateUi();
}

void SearchDialog::reset() {
    controller->clearSearch();
    resultsListView->clear();
    resultsCounterLabel->clear();
    updateUi();
    queryField->clearEdit();
    queryField->setFocus();
}

void SearchDialog::goResultVocab() {
    ResultListItem* currItem = (ResultListItem*)resultsListView->currentItem();
    if( currItem ) {
        Term* term = currItem->getTerm();
        if( term ) {
            emit showTermRequested( TermKey( term->getUid(), term->getVocabUid() ) );
            close();
        }
    }
}

void SearchDialog::editResultTerm() {
    ResultListItem* currItem = (ResultListItem*)resultsListView->currentItem();
    if( currItem ) {
        Term* term = currItem->getTerm();
        if( term ) {
            Vocabulary* vocab = controller->getVocabTree()->getVocabulary( term->getVocabUid() );
            TermDialog dialog( *vocab, controller, this, *term );
            dialog.showMaximized();
            int result = dialog.exec();
            if( result ) { 
                Term newTerm = dialog.getTerm();
                term->addTranslation( newTerm.getTranslation( controller->getPreferences().getFirstLanguage() ) );
                term->addTranslation( newTerm.getTranslation( controller->getPreferences().getTestLanguage() ) );
                BilingualKey commentKey( controller->getPreferences().getFirstLanguage(), controller->getPreferences().getTestLanguage() );
                term->addComment( commentKey, newTerm.getComment( commentKey ) );
                term->setImagePath( newTerm.getImagePath() );
                currItem->updateUi();
                vocab->setModificationDate( QDateTime::currentDateTime() );
                vocab->setDirty( true );
            }
        }
    }
}

void SearchDialog::removeResultTerms() {
    doRemoveTerms();
}

uint SearchDialog::getSelectedTermCount() const {
    uint count = 0;
    ResultListItem* termItem = (ResultListItem*)resultsListView->firstChild();
    while( termItem ) {
        if( resultsListView->isSelected( termItem ) )
            count++;
        termItem = (ResultListItem*)termItem->nextSibling();
    }
    return( count );
}

void SearchDialog::doRemoveTerms( bool allowSelectTrans = true, bool confirmBeforeRemove = true ) {
    int selectedItemCount = 0;
    // Find all the translation languages of the selected terms.
    QStringList translationLanguages;

    for( ResultListItem* termItem = (ResultListItem*)resultsListView->firstChild(); termItem; termItem = (ResultListItem*)termItem->nextSibling() ) {
        if( resultsListView->isSelected( termItem ) ) {
            selectedItemCount++;
            Term* term = termItem->getTerm();
            for( Term::TranslationMap::ConstIterator it = term->translationsBegin(); it != term->translationsEnd(); it++ ) {
                const Translation& trans = it.data();
                if( !translationLanguages.contains( trans.getLanguage() ) )
                    translationLanguages.append( trans.getLanguage() );
            }
        }
    }

    if( selectedItemCount == 0 )
        return;

    if( translationLanguages.count() <= 2 ) {
        int response;

        if( confirmBeforeRemove ) {
            QMessageBox msgBox( QObject::tr( "Warning" ), tr( "ConfirmRemoveSelectedTerms" ),
                QMessageBox::Warning,
                QMessageBox::Yes,
                QMessageBox::No | QMessageBox::Default | QMessageBox::Escape,
                QMessageBox::NoButton,
                this );
            msgBox.setButtonText( QMessageBox::Yes, tr( "Yes" ) );
            msgBox.setButtonText( QMessageBox::No, tr( "No" ) );

            response = msgBox.exec();
        }
        else 
            response = QMessageBox::Yes;

        if( response == QMessageBox::Yes ) {
            for( ResultListItem* termItem = (ResultListItem*)resultsListView->firstChild(); termItem; ) {
                ResultListItem* nextTermItem = (ResultListItem*)termItem->nextSibling();
                if( resultsListView->isSelected( termItem ) ) {
                    Term* term = termItem->getTerm();
                    Vocabulary* vocab = controller->getVocabTree()->getVocabulary( term->getVocabUid() );
                    term->setMarkedForDeletion( true );
                    delete( termItem );
                    vocab->setModificationDate( QDateTime::currentDateTime() );
                    vocab->setDirty( true );
                }
                termItem = nextTermItem;
            }

            resultsListView->clearSelection();
            updateUi();
            emit termsRemoved();
        }
    }
    else {
        int response;
        QStringList selectedLanguages;
        if( allowSelectTrans ) {
            TranslationSelectionDialog msgBox( tr( "MultipleTranslationsDetectedForRemoveTermsCaption" ), tr( "MultipleTranslationsDetectedForRemoveTerms" ), 
                translationLanguages, TranslationSelectionDialog::selectionModeTargetLanguage, controller, this );
            msgBox.setMaximumHeight( size().height() - 40 );
            msgBox.setMaximumWidth( size().width() - 40 );
            response = msgBox.exec();
            if( response )
                selectedLanguages = msgBox.getSelectedLanguages();
        }
        else {
            selectedLanguages = QStringList();
            selectedLanguages.append( controller->getPreferences().getFirstLanguage() );
            selectedLanguages.append( controller->getPreferences().getTestLanguage() );
        }
        if( selectedLanguages.count() == 0 )
            return;

        for( ResultListItem* termItem = (ResultListItem*)resultsListView->firstChild(); termItem; ) {
            ResultListItem* nextTermItem = (ResultListItem*)termItem->nextSibling();

            if( resultsListView->isSelected( termItem ) ) {
                Term* term = termItem->getTerm();
                Vocabulary* vocab = controller->getVocabTree()->getVocabulary( term->getVocabUid() );

                for( QStringList::ConstIterator it = selectedLanguages.begin(); it != selectedLanguages.end(); it++ ) {
                    QString lang = *it;
                    term->removeTranslation( lang );
                    term->removeComments( lang );
                }
                
                if( term->getTranslationCount() == 0 ) {
                    term->setMarkedForDeletion( true );
                    delete( termItem );
                    vocab->setModificationDate( QDateTime::currentDateTime() );
                    vocab->setDirty( true );
                }
            }

            termItem = nextTermItem;
        }

        resultsListView->clearSelection();
        updateUi();
    }
}

void SearchDialog::setDigraphEnabled( bool isDigraphEnabled ) {
    queryField->setDigraphEnabled( isDigraphEnabled );
}
