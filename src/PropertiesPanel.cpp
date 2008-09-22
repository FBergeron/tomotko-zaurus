#include "PropertiesPanel.h"

PropertiesPanel::PropertiesPanel( const Preferences& prefs, QWidget* parent ) 
    : QVBox( parent ), prefs( prefs ), editedVocab( NULL), editedFolder( NULL ) {
    setSpacing( 2 );
    init();
}

void PropertiesPanel::setVocabulary( Vocabulary* vocab ) {
    removeListeners();

    editedVocab = vocab;

    descriptionMultiLineEdit->setText( editedVocab->getDescription() );
    authorLineEdit->setText( editedVocab->getAuthor() );
    creationDateValueLabel->setText( editedVocab->getCreationDate().toString() );
    modificationDateValueLabel->setText( editedVocab->getModificationDate().toString() );

    updateCounters();

    addListeners();
}

void PropertiesPanel::setFolder( Folder* folder ) {
    removeListeners();

    editedFolder = folder;

    descriptionMultiLineEdit->setText( editedFolder->getDescription() );
    authorLineEdit->setText( editedFolder->getAuthor() );
    creationDateValueLabel->setText( editedFolder->getCreationDate().toString() );
    modificationDateValueLabel->setText( editedFolder->getModificationDate().toString() );

    updateCounters();

    addListeners();
}

void PropertiesPanel::updateCounters() {
    if( getType() == QString( "Vocabulary" ) ) {
        uint termCount = 0;
        uint checkedTermCount = 0;
        uint selectedTermCount = 0;
        if( editedVocab ) {
            bool isReachableFromRoot = editedVocab->isMarkedForStudy() && editedVocab->getParent()->isReachableFromRoot();
            if( prefs.isLanguageFilterEnabled() )
                editedVocab->getItemsCount( &termCount, &checkedTermCount, &selectedTermCount, isReachableFromRoot, 
                    prefs.getFirstLanguage(), prefs.getTestLanguage() );
            else
                editedVocab->getItemsCount( &termCount, &checkedTermCount, &selectedTermCount, isReachableFromRoot );
        }

        contentListView->clear();
        QListViewItem* termCountItem = new QListViewItem( contentListView, tr( "Words" ), 
            QString::number( selectedTermCount ), QString::number( checkedTermCount ), QString::number( termCount ) );
        contentListView->insertItem( termCountItem );
    }
    else if( getType() == QString( "Folder" ) ) {
        uint termCount = 0;
        uint vocabCount = 0;
        uint folderCount = 0;
        uint checkedTermCount = 0;
        uint checkedVocabCount = 0;
        uint checkedFolderCount = 0;
        uint selectedTermCount = 0;
        uint selectedVocabCount = 0;
        uint selectedFolderCount = 0;

        if( editedFolder ) {
            if( prefs.isLanguageFilterEnabled() )
                editedFolder->getItemsCount( &termCount, &vocabCount, &folderCount, 
                    &checkedTermCount, &checkedVocabCount, &checkedFolderCount, 
                        &selectedTermCount, &selectedVocabCount, &selectedFolderCount, 
                            editedFolder->isReachableFromRoot(),
                                prefs.getFirstLanguage(), prefs.getTestLanguage() );
            else
                editedFolder->getItemsCount( &termCount, &vocabCount, &folderCount, 
                    &checkedTermCount, &checkedVocabCount, &checkedFolderCount,
                        &selectedTermCount, &selectedVocabCount, &selectedFolderCount, editedFolder->isReachableFromRoot() );
        }

        contentListView->clear();
        QListViewItem* termCountItem = new QListViewItem( contentListView, tr( "Words" ), 
            QString::number( selectedTermCount ), QString::number( checkedTermCount ), QString::number( termCount ) );
        QListViewItem* vocabCountItem = new QListViewItem( contentListView, tr( "Glossaries" ), 
            QString::number( selectedVocabCount ), QString::number( checkedVocabCount ), QString::number( vocabCount ) );
        QListViewItem* folderCountItem = new QListViewItem( contentListView, tr( "Folders" ), 
            QString::number( selectedFolderCount ), QString::number( checkedFolderCount ), QString::number( folderCount ) );
        contentListView->insertItem( termCountItem );
        contentListView->insertItem( vocabCountItem );
        contentListView->insertItem( folderCountItem );
    }
    contentListView->updateGeometry();
    layout()->invalidate();
}

void PropertiesPanel::init() {
    QFont mediumFont( prefs.getMediumFont() );
    QFont labelsFont( prefs.getLabelsFont() );
    bool isDigraphEnabled( prefs.isDigraphEnabled() );

    descriptionLabel = new QLabel( tr( "Description" ), this, "DescriptionLabel" );
    descriptionMultiLineEdit = new DigraphMultiLineEdit( this, "DescriptionMultiLineEdit" );
    descriptionMultiLineEdit->setWrapPolicy( QMultiLineEdit::Anywhere );
    descriptionMultiLineEdit->setWordWrap( QMultiLineEdit::WidgetWidth );
    descriptionMultiLineEdit->setFont( mediumFont );
    descriptionMultiLineEdit->setDigraphEnabled( isDigraphEnabled );
    setStretchFactor( descriptionMultiLineEdit, 1 );

    contentLabel = new QLabel( tr( "Content" ), this, "ContentLabel" );
    contentListView = new ContentListView( this, "ContentListView" );
    contentListView->setAllColumnsShowFocus( true );
    contentListView->setSelectionMode( QListView::Extended );
    /*int colItems = */contentListView->addColumn( tr( "Items" ) );
    int colSelected = contentListView->addColumn( tr( "Selected" ) );
    int colChecked = contentListView->addColumn( tr( "Checked" ) );
    int colTotal = contentListView->addColumn( tr( "Total" ) );
    contentListView->setColumnAlignment( colSelected, QListView::AlignRight );
    contentListView->setColumnAlignment( colChecked, QListView::AlignRight );
    contentListView->setColumnAlignment( colTotal, QListView::AlignRight );
    contentListView->setStretchColumn( 0 );

    simplePropsPanel = new QHBox( this, "SimplePropsPanel" );
    simplePropsPanel->setSpacing( 10 );

    simplePropsLabelsPanel = new QVBox( simplePropsPanel, "SimplePropsLabelsPanel" );
    simplePropsFieldsPanel = new QVBox( simplePropsPanel, "SimplePropsFieldsPanel" );

    authorLabel = new QLabel( tr( "Author" ), simplePropsLabelsPanel, "AuthorLabel" );
    authorLineEdit = new DigraphLineEdit( simplePropsFieldsPanel, "AuthorLineEdit" ); 
    authorLineEdit->setFont( mediumFont );
    authorLineEdit->setDigraphEnabled( isDigraphEnabled );

    creationDateLabel = new QLabel( tr( "CreationDate" ), simplePropsLabelsPanel, "CreationDateLabel" );
    creationDateValueLabel = new QLabel( QString::null, simplePropsFieldsPanel, "CreationDateLineEdit" ); 
    creationDateValueLabel->setFont( mediumFont );

    modificationDateLabel = new QLabel( tr( "ModificationDate" ), simplePropsLabelsPanel, "ModificationDateLabel" );
    modificationDateValueLabel = new QLabel( QString::null, simplePropsFieldsPanel, "ModificationDateLineEdit" ); 
    modificationDateValueLabel->setFont( mediumFont );
}

PropertiesPanel::~PropertiesPanel() {
}

void PropertiesPanel::updateAuthor( const QString& author ) {
    if( getType() == QString( "Vocabulary" ) ) {
        editedVocab->setAuthor( author );
        editedVocab->setModificationDate( QDateTime::currentDateTime() );
        editedVocab->setDirty( true );
    }
    else if( getType() == QString( "Folder" ) ) {
        editedFolder->setAuthor( author );
        editedFolder->setModificationDate( QDateTime::currentDateTime() );
        editedFolder->setDirty( true );
    }
}

void PropertiesPanel::updateDescription() {
    if( getType() == QString( "Vocabulary" ) ) {
        editedVocab->setDescription( descriptionMultiLineEdit->text() );
        editedVocab->setModificationDate( QDateTime::currentDateTime() );
        editedVocab->setDirty( true );
    }
    else if( getType() == QString( "Folder" ) ) {
        editedFolder->setDescription( descriptionMultiLineEdit->text() );
        editedFolder->setModificationDate( QDateTime::currentDateTime() );
        editedFolder->setDirty( true );
    }
}

QString PropertiesPanel::getType() const {
    return( editedVocab ? QString( "Vocabulary" ) : QString( "Folder" ) );
}

void PropertiesPanel::updateFonts() {
    QFont mediumFont( prefs.getMediumFont() );
    QFont labelsFont( prefs.getLabelsFont() );

    descriptionLabel->setFont( labelsFont );
    descriptionMultiLineEdit->setFont( mediumFont );
    contentLabel->setFont( labelsFont );
    contentListView->setFont( mediumFont ); 
    authorLabel->setFont( labelsFont );
    authorLineEdit->setFont( mediumFont );
    creationDateLabel->setFont( labelsFont );
    creationDateValueLabel->setFont( mediumFont );
    modificationDateLabel->setFont( labelsFont );
    modificationDateValueLabel->setFont( mediumFont );
}

void PropertiesPanel::setDigraphEnabled( bool isEnabled ) {
    descriptionMultiLineEdit->setDigraphEnabled( isEnabled );
    authorLineEdit->setDigraphEnabled( isEnabled );
}

void PropertiesPanel::retranslateUi() {
    descriptionLabel->setText( tr( "Description" ) );
    contentLabel->setText( tr( "Content" ) );
    contentListView->setColumnText( 0, tr( "Items" ) ); 
    contentListView->setColumnText( 1, tr( "Selected" ) ); 
    contentListView->setColumnText( 2, tr( "Checked" ) ); 
    contentListView->setColumnText( 3, tr( "Total" ) ); 
    authorLabel->setText( tr( "Author" ) );
    creationDateLabel->setText( tr( "CreationDate" ) );
    modificationDateLabel->setText( tr( "ModificationDate" ) );
}

void PropertiesPanel::addListeners() {
    connect( authorLineEdit, SIGNAL( textChanged( const QString& ) ),
        this, SLOT( updateAuthor( const QString& ) ) );
    connect( descriptionMultiLineEdit, SIGNAL( textChanged() ),
        this, SLOT( updateDescription() ) );
}

void PropertiesPanel::removeListeners() {
    disconnect( authorLineEdit, SIGNAL( textChanged( const QString& ) ),
        this, SLOT( updateAuthor( const QString& ) ) );
    disconnect( descriptionMultiLineEdit, SIGNAL( textChanged() ),
        this, SLOT( updateDescription() ) );
}

