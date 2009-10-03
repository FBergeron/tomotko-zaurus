#include "VocabularyManagerFrame.h"
#include "icons/addFolder.xpm"
#include "icons/addVocab.xpm"
#include "icons/removeItem.xpm"
#include "icons/addTerm.xpm"
#include "icons/editTerm.xpm"
#include "icons/removeTerm.xpm"
#include "icons/checkAllTerms.xpm"
#include "icons/inverseCheckedTerms.xpm"
#include "icons/maximize.xpm"

VocabularyManagerFrame::VocabularyManagerFrame( Controller* controller, QWidget *parent = 0, const char* name = 0 )
    : QWidget( parent, name ), searchDialog( NULL ), controller( controller ) {
    treePanel = new QVBox( this, "TreePanel" );
    treePanel->setSpacing( 2 );

    vocabTreeView = new VocabTreeView( *controller, treePanel, "VocabTreeView" );
    vocabTreeView->setTreeStepSize( 24 );
    vocabTreeView->addColumn( tr( "Glossaries" ) );
    vocabTreeView->setStretchColumn( 0 );
    
    treeButtonPanel = new QHBox( treePanel, "TreeButtonPanel" );

    addFolderButton = new QPushButton( tr( "AddFolder" ), treeButtonPanel, "AddFolderButton" );
    addFolderButton->setPixmap( ZPIXMAP( addFolder_xpm ) );
    QToolTip::add( addFolderButton, tr( "AddFolder" ) );
    connect( addFolderButton, SIGNAL( clicked() ), this, SLOT( addFolder() ) );

    addVocabButton = new QPushButton( tr( "AddGlossary" ), treeButtonPanel, "AddVocabButton" );
    addVocabButton->setPixmap( ZPIXMAP( addVocab_xpm ) );
    QToolTip::add( addVocabButton, tr( "AddGlossary" ) );
    connect( addVocabButton, SIGNAL( clicked() ), this, SLOT( addVocab() ) );

    removeItemButton = new QPushButton( tr( "RemoveItem" ), treeButtonPanel, "RemoveItemButton" );
    removeItemButton->setPixmap( ZPIXMAP( removeItem_xpm ) ); 
    QToolTip::add( removeItemButton, tr( "RemoveItem" ) );
    connect( removeItemButton, SIGNAL( clicked() ), this, SLOT( removeItem() ) );
   
    detailsPanel = new QWidgetStack( this, "DetailsPanel" );

    folderDetailsPanel = new QVBox( detailsPanel, "FolderDetailsPanel" );
    folderDetailsPanel->setSpacing( 2 );
    folderDetailsFolderHeaderPanel = new QHBox( folderDetailsPanel, "FolderDetailsFolderHeaderPanel" );
    folderDetailsFolderHeaderPanel->setSpacing( 10 );
    folderDetailsTitle = new QLabel( tr( "Folder" ), folderDetailsFolderHeaderPanel, "FolderDetailsTitle" );
    folderDetailsFolderTitlePanel = new QHBox( folderDetailsFolderHeaderPanel, "FolderDetailsFolderTitlePanel" );
    folderDetailsFolderTitlePanel->setSpacing( 2 );
    folderDetailsFolderTitleLabel = new QLabel( tr( "Title" ), folderDetailsFolderTitlePanel, "FolderDetailsFolderTitleLabel" );
    folderDetailsFolderTitleLineEdit = new DigraphLineEdit( folderDetailsFolderTitlePanel, "FolderDetailsFolderTitleLineEdit" );

    folderDetailsFolderMaximizeDetailsButton = new QPushButton( tr( "MaximizeDetails" ), folderDetailsFolderTitlePanel, "FolderDetailsFolderMaximizeDetailsButton" );
    folderDetailsFolderMaximizeDetailsButton->setPixmap( ZPIXMAP( maximize_xpm ) );
    folderDetailsFolderMaximizeDetailsButton->setToggleButton( true );
    folderDetailsFolderMaximizeDetailsButton->setMaximumHeight( 
        folderDetailsFolderTitleLineEdit->sizeHint().height() > 24 ? folderDetailsFolderTitleLineEdit->sizeHint().height() : 24 );
    folderDetailsFolderMaximizeDetailsButton->setMaximumWidth( folderDetailsFolderMaximizeDetailsButton->maximumHeight() );
    QToolTip::add( folderDetailsFolderMaximizeDetailsButton, tr( "MaximizeDetails" ) );
    connect( folderDetailsFolderMaximizeDetailsButton, SIGNAL( toggled( bool ) ), this, SLOT( toggleMaximizeDetails( bool ) ) );

    folderDetailsPropsPanel = new PropertiesPanel( controller->getPreferences(), folderDetailsPanel );

    vocabDetailsPanel = new QVBox( detailsPanel, "VocabDetailsPanel" );
    vocabDetailsPanel->setSpacing( 2 );
    vocabDetailsVocabHeaderPanel = new QHBox( vocabDetailsPanel, "VocabDetailsVocabHeaderPanel" );
    vocabDetailsVocabHeaderPanel->setSpacing( 10 );
    vocabDetailsTitle = new QLabel( tr( "Glossary" ), vocabDetailsVocabHeaderPanel, "VocabDetailsTitle" );
    vocabDetailsVocabTitlePanel = new QHBox( vocabDetailsVocabHeaderPanel, "VocabDetailsVocabTitlePanel" );
    vocabDetailsVocabTitlePanel->setSpacing( 2 );
    vocabDetailsVocabTitleLabel = new QLabel( tr( "Title" ), vocabDetailsVocabTitlePanel, "VocabDetailsVocabTitleLabel" );
    vocabDetailsVocabTitleLineEdit = new DigraphLineEdit( vocabDetailsVocabTitlePanel, "VocabDetailsVocabTitleLineEdit" );

    vocabDetailsVocabMaximizeDetailsButton = new QPushButton( tr( "MaximizeDetails" ), vocabDetailsVocabTitlePanel, "FolderDetailsFolderMaximizeDetailsButton" );
    vocabDetailsVocabMaximizeDetailsButton->setPixmap( ZPIXMAP( maximize_xpm ) );
    vocabDetailsVocabMaximizeDetailsButton->setToggleButton( true );
    vocabDetailsVocabMaximizeDetailsButton->setMaximumHeight( 
        folderDetailsFolderTitleLineEdit->sizeHint().height() > 24 ? folderDetailsFolderTitleLineEdit->sizeHint().height() : 24 );
    vocabDetailsVocabMaximizeDetailsButton->setMaximumWidth( folderDetailsFolderMaximizeDetailsButton->maximumHeight() );
    QToolTip::add( vocabDetailsVocabMaximizeDetailsButton, tr( "MaximizeDetails" ) );
    connect( vocabDetailsVocabMaximizeDetailsButton, SIGNAL( toggled( bool ) ), this, SLOT( toggleMaximizeDetails( bool ) ) );

    vocabDetailsTabWidget = new QTabWidget( vocabDetailsPanel, "VocabDetailsTabWidget" );

    vocabDetailsPropsPanel = new PropertiesPanel( controller->getPreferences(), vocabDetailsTabWidget );
    connect( vocabDetailsTabWidget, SIGNAL( currentChanged( QWidget* ) ), vocabDetailsPropsPanel, SLOT( updateCounters() ) ); 

    vocabDetailsTermsPanel = new QVBox( vocabDetailsTabWidget, "VocabDetailsTermsPanel" );
    termList = new SmartListView( vocabDetailsTermsPanel, "TermList" );
    termList->setAllColumnsShowFocus( true );
    termList->setSelectionMode( QListView::Extended );
    int colFirstLanguage = termList->addColumn( QObject::tr( controller->getPreferences().getFirstLanguage() )/*, 188*/ );
    int colTestLanguage = termList->addColumn( QObject::tr( controller->getPreferences().getTestLanguage() )/*, 188*/ );
    termList->setColumnWidthMode( colFirstLanguage, QListView::Manual );
    termList->setColumnWidthMode( colTestLanguage, QListView::Manual );
    termList->setAllColumnsWide( true );
    connect( termList, SIGNAL( selectionChanged() ), this, SLOT( updateTermList() ) ); 

    termControlPanel = new QWidget( vocabDetailsTermsPanel, "TermControlPanel" );
    termControlPanelLayout = new QHBoxLayout( termControlPanel );
    termControlPanelLayout->setSpacing( 2 );

    checkControlPanel = new QHBox( termControlPanel, "CheckControlPanel" );

    checkAllTermsButton = new QPushButton( tr( "CheckAllTerms" ), checkControlPanel, "CheckAllTermsButton" );
    checkAllTermsButton->setPixmap( ZPIXMAP( checkAllTerms_xpm ) ); 
    QToolTip::add( checkAllTermsButton, tr( "CheckAllTerms" ) );
    connect( checkAllTermsButton, SIGNAL( clicked() ), this, SLOT( checkAllTerms() ) );

    inverseCheckedTermsButton = new QPushButton( tr( "InverseCheckedTerms" ), checkControlPanel, "InverseCheckedTermsButton" );
    inverseCheckedTermsButton->setPixmap( ZPIXMAP( inverseCheckedTerms_xpm ) ); 
    QToolTip::add( inverseCheckedTermsButton, tr( "InverseCheckedTerms" ) );
    connect( inverseCheckedTermsButton, SIGNAL( clicked() ), this, SLOT( inverseCheckedTerms() ) );

    addRemoveTermPanel = new QHBox( termControlPanel, "AddRemoveTermPanel" );

    addTermButton = new QPushButton( tr( "AddTerm" ), addRemoveTermPanel, "AddTermButton" );
    addTermButton->setPixmap( ZPIXMAP( addTerm_xpm ) ); 
    QToolTip::add( addTermButton, tr( "AddTerm" ) );
    connect( addTermButton, SIGNAL( clicked() ), this, SLOT( addTerm() ) );

    editTermButton = new QPushButton( tr( "EditTerm" ), addRemoveTermPanel, "EditTermButton" );
    editTermButton->setPixmap( ZPIXMAP( editTerm_xpm ) ); 
    QToolTip::add( editTermButton, tr( "EditTerm" ) );
    connect( editTermButton, SIGNAL( clicked() ), this, SLOT( editTerm() ) );
    
    removeTermButton = new QPushButton( tr( "RemoveTerm" ), addRemoveTermPanel, "RemoveTermButton" );
    removeTermButton->setPixmap( ZPIXMAP( removeTerm_xpm ) );
    QToolTip::add( removeTermButton, tr( "RemoveTerm" ) );
    connect( removeTermButton, SIGNAL( clicked() ), this, SLOT( removeTerms() ) );
   
    termControlPanelLayout->addWidget( checkControlPanel, 0 );
    termControlPanelLayout->addWidget( addRemoveTermPanel, 1 );
    vocabDetailsTabWidget->addTab( vocabDetailsPropsPanel, tr( "Properties" ) );
    vocabDetailsTabWidget->addTab( vocabDetailsTermsPanel, tr( "Words" ) );

    detailsPanel->addWidget( folderDetailsPanel, panelFolderIndex );
    detailsPanel->addWidget( vocabDetailsPanel, panelVocabIndex );

    loadData();
    updateTree();
    vocabTreeView->setSelected( vocabTreeRoot, true );

    detailsPanel->raiseWidget( panelFolderIndex );

    // We add the tree listener at the end to ignore events generated when creating
    // the text widgets.
    connect( vocabTreeView, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( updateUi() ) ); 
    updateFonts();
    updateUi();
}

VocabularyManagerFrame::~VocabularyManagerFrame() {
}

void VocabularyManagerFrame::updateShownItems() {
    keepSelection();
    updateTree();
    restoreTreeSelection();
    updateUi();
    restoreVocabSelection();
}

void VocabularyManagerFrame::updateTree() {
    if( vocabTreeView ) {
        vocabTreeView->setUpdatesEnabled( false );
        vocabTreeView->clear();
    }
    vocabTreeRoot = buildTreeRec( vocabTreeView, NULL, controller->getVocabTree() );
    //vocabTreeView->setSelected( vocabTreeRoot, true );
    //if( currentFolderId != -1 || currentVocabId != -1 )
    //    restoreSelection( currentFolderId, currentVocabId, currentTermId, selectedTermIdList );
    //else
    //    vocabTreeView->setSelected( vocabTreeRoot, true );
    if( vocabTreeView )
        vocabTreeView->setUpdatesEnabled( true );
}

void VocabularyManagerFrame::loadData() {
    QCopEnvelope busyEnv( "QPE/System", "busy()" ); 
    controller->loadData(); // If this returns false, handle the error.
    QCopEnvelope freeEnv( "QPE/System", "notBusy()" ); 
}

bool VocabularyManagerFrame::saveData() {
    QCopEnvelope busyEnv( "QPE/System", "busy()" ); 
    bool isOk = controller->saveData();
    QCopEnvelope freeEnv( "QPE/System", "notBusy()" ); 
    return( isOk );
}

bool VocabularyManagerFrame::isExportAllowed() const {
    TreeItem* selectedItem = (TreeItem*)vocabTreeView->currentItem();
    if( !selectedItem )
        return( false );
    if( selectedItem->isFolder() ) {
        FolderTreeItem* folderItem = (FolderTreeItem*)selectedItem;
        return( !folderItem->getFolder()->isEmpty() );
    }
    else {
        VocabTreeItem* vocabItem = (VocabTreeItem*)selectedItem;
        return( !vocabItem->getVocabulary()->isEmpty() );
    }
}

bool VocabularyManagerFrame::isImportAllowed() const {
    TreeItem* selectedItem = (TreeItem*)vocabTreeView->currentItem();
    return( selectedItem && selectedItem->isFolder() );
}

void VocabularyManagerFrame::cut() {
    QWidget* widget = qApp->focusWidget();
    if( widget ) {
        if( strcmp( widget->name(), "VocabTreeView" ) == 0 ) {
            TreeItem* selectedItem = (TreeItem*)vocabTreeView->currentItem();
            if( selectedItem ) {
                if( selectedItem->isFolder() ) {
                    FolderTreeItem* folderItem = (FolderTreeItem*)selectedItem;
                    Folder* folder = folderItem->getFolder();
                    controller->copy( folder );
                    doRemoveItem( false );
                }
                else {
                    VocabTreeItem* vocabItem = (VocabTreeItem*)selectedItem;
                    Vocabulary* vocab = vocabItem->getVocabulary();
                    controller->copy( vocab );
                    doRemoveItem( false );
                }
            }
        }
        else if( strcmp( widget->name(), "TermList" ) == 0 ) {
            copyTerms();
            doRemoveTerms( false, false );
        }
    }
}

void VocabularyManagerFrame::copy() {
    QWidget* widget = qApp->focusWidget();
    if( widget ) {
        if( strcmp( widget->name(), "VocabTreeView" ) == 0 ) {
            TreeItem* selectedItem = (TreeItem*)vocabTreeView->currentItem();
            if( selectedItem ) {
                if( selectedItem->isFolder() ) {
                    FolderTreeItem* folderItem = (FolderTreeItem*)selectedItem;
                    Folder* folder = folderItem->getFolder();
                    controller->copy( folder );
                }
                else {
                    VocabTreeItem* vocabItem = (VocabTreeItem*)selectedItem;
                    Vocabulary* vocab = vocabItem->getVocabulary();
                    controller->copy( vocab );
                }
            }
        }
        else if( strcmp( widget->name(), "TermList" ) == 0 )
            copyTerms();
    }
}

void VocabularyManagerFrame::paste() {
    QWidget* widget = qApp->focusWidget();
    if( widget ) {
        if( strcmp( widget->name(), "VocabTreeView" ) == 0 ) {
            TreeItem* selectedItem = (TreeItem*)vocabTreeView->currentItem();
            if( selectedItem ) {
                QString dataType = controller->getClipboardDataType();
                if( selectedItem->isFolder() ) {
                    if( dataType == QString( "vocabulary" ) )
                        pasteVocabulary();
                    else if( dataType == QString( "folder" ) )
                        pasteFolder();
                }
                else {
                    if( dataType == QString( "terms" ) )
                        pasteTerms();
                }
            }
        }
        else if( strcmp( widget->name(), "TermList" ) == 0 ) {
            QString dataType = controller->getClipboardDataType();
            if( dataType == QString( "terms" ) )
                pasteTerms();
        }
    }
}

void VocabularyManagerFrame::importData() {
    TreeItem* selectedItem = (TreeItem*)vocabTreeView->currentItem();
    if( selectedItem && selectedItem->isFolder() ) {
        FolderTreeItem* folderItem = (FolderTreeItem*)selectedItem;
        Folder* folder = folderItem->getFolder(); 
        QDir dir( QPEApplication::documentDir() );
        ZFileDialog dialog( tr( "Import..." ), dir.path(), ZFileDialog::ExistingFile, false, this );
        dialog.setFilters( QStringList::split( QString( "," ), QString( ".tmk" ) ) );

        int result = dialog.exec();
        if( result && !dialog.selectedFile().isEmpty() ) {
            QString fileExtension = dialog.selectedFile().right( 4 );
            if( fileExtension == QString( ".tmk" ) ) {
                QCopEnvelope busyEnvLanguages( "QPE/System", "busy()" ); 
                QStringList translationLanguages = controller->getTranslationLanguagesFromFile( dialog.selectedFile() );
                QCopEnvelope freeEnvLanguages( "QPE/System", "notBusy()" ); 

                QStringList languagesToImport( translationLanguages );
                if( translationLanguages.count() > 2 ) {
                    TranslationSelectionDialog msgBox( tr( "Import..." ), tr( "MultipleTranslationsDetectedForImport" ), 
                        translationLanguages, TranslationSelectionDialog::selectionModeStudyLanguages, controller, this );
                    msgBox.setMaximumHeight( size().height() - 40 );
                    msgBox.setMaximumWidth( size().width() - 40 );
                    int response = msgBox.exec();
                    if( response )
                        languagesToImport = msgBox.getSelectedLanguages();
                    else  {
                        return; // Cancel import.
                    }
                }

                for( QStringList::ConstIterator it = languagesToImport.begin(); it != languagesToImport.end(); it++ ) {
                    const QString& lang = *it;
                    if( !controller->getPreferences().isStudyLanguage( lang ) )
                        controller->getPreferences().addStudyLanguage( lang );
                }

                QCopEnvelope busyEnvImport( "QPE/System", "busy()" ); 
                Base* newItem = controller->importData( folder, dialog.selectedFile(), languagesToImport );
                cerr << "newItem=" << newItem << endl;
                if( newItem ) {
                    TreeItem* newTreeItem = NULL;
                    if( strcmp( newItem->className(), "Folder" ) == 0 ) {
                        cerr << "newItem is a folder" << endl;
                        Folder* newFolder = (Folder*)newItem;
                        newFolder->setModificationDate( QDateTime::currentDateTime() );
                        newFolder->setDirty( true );
                        folder->add( newFolder );
                        newTreeItem = buildTreeRec( vocabTreeView, folderItem, newFolder, true ); 
                    }
                    else if( strcmp( newItem->className(), "Vocabulary" ) == 0 ) {
                        cerr << "newItem is vocab" << endl;
                        Vocabulary* newVocab = (Vocabulary*)newItem;
                        newVocab->setModificationDate( QDateTime::currentDateTime() );
                        newVocab->setDirty( true );
                        folder->add( newVocab );
                        newTreeItem = buildTreeRec( folderItem, newVocab ); 
                    }
                    cerr << "newTreeItem=" << newTreeItem << endl;
                    if( newTreeItem ) {
                        vocabTreeView->ensureItemVisible( newTreeItem );
                        vocabTreeView->setSelected( newTreeItem, true );
                        newTreeItem->setOpen( true );
                    }
                    else {
                        // This happens when the imported data is from a different language pair
                        // than the current one.  We reselect the selectedItem in this case.
                        vocabTreeView->setSelected( selectedItem, true );
                    }
                    QString msg = tr( "ImportSuccessful" );
                    if( !newTreeItem )
                        msg += tr( "InvisibleImport" );
                    QMessageBox::information( this, QObject::tr( "OperationSuccessful" ), msg );
                }
                else
                    QMessageBox::warning( this, QObject::tr( "OperationFailed" ), tr( "ItemImportFailed" ) );
                QCopEnvelope freeEnvImport( "QPE/System", "notBusy()" ); 
            }
            else
                QMessageBox::warning( this, QObject::tr( "OperationFailed" ), tr( "ItemImportFailedZipFileExpected" ) );
        }
    }
}

void VocabularyManagerFrame::exportData() {
    TreeItem* selectedItem = (TreeItem*)vocabTreeView->currentItem();
    if( selectedItem ) {
        if( selectedItem->isFolder() ) {
            FolderTreeItem* folderItem = (FolderTreeItem*)selectedItem;
            exportFolder( folderItem->getFolder() );
        }
        else {
            VocabTreeItem* vocabItem = (VocabTreeItem*)selectedItem;
            exportVocabulary( vocabItem->getVocabulary() );
        }
    }
}

void VocabularyManagerFrame::exportVocabulary( Vocabulary* vocab ) {
    QStringList translationLanguages = vocab->getTranslationLanguages();
    QStringList languagesToExport = translationLanguages;
    if( translationLanguages.count() > 2 ) {
        TranslationSelectionDialog msgBox( tr( "Export..." ), tr( "MultipleTranslationsDetectedForExport" ), 
            translationLanguages, TranslationSelectionDialog::selectionModeAllLanguages, controller, this );
        msgBox.setMaximumHeight( size().height() - 40 );
        msgBox.setMaximumWidth( size().width() - 40 );
        int response = msgBox.exec();
        if( response )
            languagesToExport = msgBox.getSelectedLanguages();
        else 
            return; // Cancel export.
    }

    QDir dir( QPEApplication::documentDir() );
    ZFileDialog dialog( tr( "Export..." ), dir.path(), ZFileDialog::AnyFile, false, this );
    dialog.setSelection( vocab->getTitle() + QString( ".tmk" ) ); 
    int result = dialog.exec();
    if( result ) {
        QCopEnvelope busyEnv( "QPE/System", "busy()" ); 
        bool isOk = controller->exportData( vocab, dialog.selectedFile(), &languagesToExport );
        QCopEnvelope freeEnv( "QPE/System", "notBusy()" ); 
        if( isOk )
            QMessageBox::information( this, QObject::tr( "OperationSuccessful" ), tr( "GlossaryExportSuccessful" ) );
        else
            QMessageBox::warning( this, QObject::tr( "OperationFailed" ), tr( "GlossaryExportFailed" ) );
    }
}

void VocabularyManagerFrame::exportFolder( Folder* folder ) {
    QStringList translationLanguages = folder->getTranslationLanguages();
    QStringList languagesToExport = translationLanguages;
    if( translationLanguages.count() > 2 ) {
        TranslationSelectionDialog msgBox( tr( "Export..." ), tr( "MultipleTranslationsDetectedForFolderExport" ), 
            translationLanguages, TranslationSelectionDialog::selectionModeAllLanguages, controller, this );
        msgBox.setMaximumHeight( size().height() - 40 );
        msgBox.setMaximumWidth( size().width() - 40 );
        int response = msgBox.exec();
        if( response )
            languagesToExport = msgBox.getSelectedLanguages();
        else 
            return; // Cancel export.
    }

    QDir dir( QPEApplication::documentDir() );
    ZFileDialog dialog( tr( "Export..." ), dir.path(), ZFileDialog::AnyFile, false, this );
    dialog.setSelection( folder->getTitle() + QString( ".tmk" ) ); 
    int result = dialog.exec();
    if( result ) {
        QCopEnvelope busyEnv( "QPE/System", "busy()" ); 
        bool isOk = controller->exportData( folder, dialog.selectedFile(), &languagesToExport );
        QCopEnvelope freeEnv( "QPE/System", "notBusy()" ); 
        if( isOk )
            QMessageBox::information( this, QObject::tr( "OperationSuccessful" ), tr( "FolderExportSuccessful" ) );
        else
            QMessageBox::warning( this, QObject::tr( "OperationFailed" ), tr( "FolderExportFailed" ) );
    }
}

void VocabularyManagerFrame::updateFonts() {
    QFont mediumFont( controller->getPreferences().getMediumFont() );
    QFont labelsFont( controller->getPreferences().getLabelsFont() );

    vocabTreeView->setFont( mediumFont );
    vocabTreeView->header()->setFont( labelsFont );
    folderDetailsTitle->setFont( labelsFont ); 
    folderDetailsFolderTitleLabel->setFont( labelsFont );
    folderDetailsFolderTitleLineEdit->setFont( mediumFont );
    folderDetailsPropsPanel->updateFonts();
    folderDetailsFolderTitleLineEdit->setFont( mediumFont );
    vocabDetailsTitle->setFont( labelsFont );
    vocabDetailsVocabTitleLabel->setFont( labelsFont );
    vocabDetailsVocabTitleLineEdit->setFont( mediumFont );
    vocabDetailsPropsPanel->updateFonts();
    termList->setFont( mediumFont );
    termList->header()->setFont( labelsFont );
    for( TermListItem* item = (TermListItem*)termList->firstChild(); item; item = (TermListItem*)item->nextSibling() ) {
        item->setFont( 0, controller->getPreferences().getMediumFont( controller->getPreferences().getFirstLanguage() ) );
        item->setFont( 1, controller->getPreferences().getMediumFont( controller->getPreferences().getTestLanguage() ) );
    }
}

bool VocabularyManagerFrame::isDigraphEnabled() const {
    return( folderDetailsFolderTitleLineEdit->isDigraphEnabled() );
}

void VocabularyManagerFrame::keepSelection() {
    currentFolderId = -1; 
    currentVocabId = -1; 
    currentTermId = -1;
    selectedTermIdList.clear();

    TreeItem* currentItem = (TreeItem*)vocabTreeView->currentItem();
    if( currentItem ) {
        if( currentItem->isFolder() ) {
            FolderTreeItem* folderItem = (FolderTreeItem*)currentItem;
            currentFolderId = folderItem->getFolder()->getId();
        }
        else {
            VocabTreeItem* vocabItem = (VocabTreeItem*)currentItem;
            currentVocabId = vocabItem->getVocabulary()->getId();
            for( TermListItem* termItem = (TermListItem*)termList->firstChild(); termItem; termItem = (TermListItem*)termItem->nextSibling() ) {
                if( termItem->isSelected() ) {
                    if( currentTermId == -1 )
                        currentTermId = termItem->getTerm()->getId();
                    selectedTermIdList.append( termItem->getTerm()->getId() );
                }
            }
        }
    }
}

void VocabularyManagerFrame::restoreTreeSelection() {
    bool isTreeItemFound = false;
    for( TreeItem* item = (TreeItem*)vocabTreeView->firstChild(); item; item = (TreeItem*)item->itemBelow() ) {
        if( item->isFolder() ) {
            FolderTreeItem* folderItem = (FolderTreeItem*)item;
            if( currentFolderId == folderItem->getFolder()->getId() ) {
                isTreeItemFound = true;
                vocabTreeView->ensureItemVisible( item );
                vocabTreeView->setSelected( item, true );
                break;
            }
        }
        else {
            VocabTreeItem* vocabItem = (VocabTreeItem*)item;
            if( currentVocabId == vocabItem->getVocabulary()->getId() ) {
                isTreeItemFound = true;
                vocabTreeView->ensureItemVisible( item );
                vocabTreeView->setSelected( item, true );
                break;
            }
        }
    }
    if( !isTreeItemFound ) {
        vocabTreeView->setSelected( vocabTreeRoot, true );
        vocabTreeView->ensureItemVisible( vocabTreeRoot );
    }
}

void VocabularyManagerFrame::restoreVocabSelection() {
    if( currentTermId != -1 ) {
        termList->clearSelection();
        for( TermListItem* termItem = (TermListItem*)termList->firstChild(); termItem; termItem = (TermListItem*)termItem->nextSibling() ) {
            int termItemId = termItem->getTerm()->getId();
            if( selectedTermIdList.contains( termItemId ) )
                termList->setSelected( termItem, true );
            if( currentTermId == termItemId ) {
                termList->ensureItemVisible( termItem );
                termList->setCurrentItem( termItem );
                updateTermList();
                break;
            }
        }
    }
}

void VocabularyManagerFrame::setDigraphEnabled( bool isEnabled ) {
    folderDetailsFolderTitleLineEdit->setDigraphEnabled( isEnabled );
    vocabDetailsVocabTitleLineEdit->setDigraphEnabled( isEnabled );
    folderDetailsPropsPanel->setDigraphEnabled( isEnabled );
    vocabDetailsPropsPanel->setDigraphEnabled( isEnabled );
    if( searchDialog )
        searchDialog->setDigraphEnabled( isEnabled );
}

void VocabularyManagerFrame::retranslateUi() {
    vocabTreeView->setColumnText( 0, tr( "Glossaries" ) );
    folderDetailsTitle->setText( tr( "Folder" ) );
    folderDetailsFolderTitleLabel->setText( tr( "Title" ) );
    vocabDetailsTitle->setText( tr( "Glossary" ) );
    vocabDetailsVocabTitleLabel->setText( tr( "Title" ) );
    termList->setColumnText( 0, QObject::tr( controller->getPreferences().getFirstLanguage() ) );
    termList->setColumnText( 1, QObject::tr( controller->getPreferences().getTestLanguage() ) );
    folderDetailsPropsPanel->retranslateUi();
    vocabDetailsPropsPanel->retranslateUi();
    vocabDetailsTabWidget->changeTab( vocabDetailsPropsPanel, tr( "Properties" ) );
    vocabDetailsTabWidget->changeTab( vocabDetailsTermsPanel, tr( "Words" ) );
    if( searchDialog )
        searchDialog->retranslateUi();
    updateUi();
}

bool VocabularyManagerFrame::areDetailsMaximized() const {
    TreeItem* selectedItem = (TreeItem*)vocabTreeView->currentItem();
    if( selectedItem ) {
        return( selectedItem->isFolder() ? 
            folderDetailsFolderMaximizeDetailsButton->isOn() : 
                vocabDetailsVocabMaximizeDetailsButton->isOn() );
    }
    return( false );
}


void VocabularyManagerFrame::toggleMaximizeDetails( bool isOn ) {
    if( isOn )
        maximizeDetailsPanel();
    else
        restoreDetailsPanel();
}

void VocabularyManagerFrame::updateGeometry() {
    QSize size( width(), height() );
    if( size.width() > size.height() ) {
        int treePanelWidth = ( treePanel->isVisible() ? (int)( size.width() / 2.5 ) : 0 );
        int detailsPanelWidth = size.width() - treePanelWidth;

        if( treePanel->isVisible() )
            treePanel->setGeometry( 0, 0, treePanelWidth, size.height() );
        detailsPanel->setGeometry( treePanelWidth, 0, detailsPanelWidth, size.height() );
    }
    else {
        int treePanelHeight = ( treePanel->isVisible() ? 170 : 0 );
        int detailsPanelHeight = size.height() - treePanelHeight;
        if( treePanel->isVisible() )
            treePanel->setGeometry( 0, 0, size.width(), treePanelHeight );
        detailsPanel->setGeometry( 0, treePanelHeight, size.width(), detailsPanelHeight );
    }
}

void VocabularyManagerFrame::resizeEvent( QResizeEvent* ) {
    updateGeometry();
    TreeItem* selectedItem = (TreeItem*)vocabTreeView->currentItem();
    if( selectedItem ) {
        if( selectedItem->isFolder() ) {
            VocabTreeItem* vocabItem = (VocabTreeItem*)selectedItem;
            vocabTreeView->ensureItemVisible( vocabItem );

            for( TermListItem* termItem = (TermListItem*)termList->firstChild(); termItem; termItem = (TermListItem*)termItem->nextSibling() ) {
                if( termItem->isSelected() ) {
                    termList->ensureItemVisible( termItem );
                    break;
                }
            }
        }
    }
}

void VocabularyManagerFrame::showEvent( QShowEvent* showEvt ) {
    QWidget::showEvent( showEvt );
    // Both folder and vocab maximize buttons should have the same state so we use the folder's one.
    // We do this because the state may be inconsistent because of QuizFrame.
    action[ ACTION_MAXIMIZE ]->setOn( folderDetailsFolderMaximizeDetailsButton->isOn() );  
}


void VocabularyManagerFrame::updateUi() {
    removeListeners();
    TreeItem* selectedItem = (TreeItem*)vocabTreeView->currentItem();
    removeItemButton->setEnabled( selectedItem );
    if( selectedItem ) {
        if( selectedItem->isFolder() ) {
            FolderTreeItem* folderItem = (FolderTreeItem*)selectedItem;
            updateCurrentFolder( folderItem );
        }
        else {
            VocabTreeItem* vocabItem = (VocabTreeItem*)selectedItem;
            updateCurrentVocab( vocabItem );
        }
    }
    if( termList && termList->childCount() > 0 ) {
        checkAllTermsButton->setEnabled( true );
        inverseCheckedTermsButton->setEnabled( true );
    }
    else {
        checkAllTermsButton->setEnabled( false );
        inverseCheckedTermsButton->setEnabled( false );
    }
    emit( selectionChanged( selectedItem ) );
    addListeners();
}

void VocabularyManagerFrame::updateCurrentFolder( FolderTreeItem* folderItem ) {
    QString firstLang = controller->getPreferences().getFirstLanguage();
    QString testLang = controller->getPreferences().getTestLanguage();
    bool isBothLangSet = ( firstLang != QString::null && testLang != QString::null );
    Folder* folder = folderItem->getFolder();
    folderDetailsFolderTitleLineEdit->setText( folder->getTitle() );
    folderDetailsPropsPanel->setFolder( folder );
    addVocabButton->setEnabled( isBothLangSet );
    addFolderButton->setEnabled( isBothLangSet );
    removeItemButton->setEnabled( folderItem != vocabTreeRoot );
    detailsPanel->raiseWidget( panelFolderIndex );
}

void VocabularyManagerFrame::updateCurrentVocab( VocabTreeItem* vocabItem ) {
    const Preferences& prefs = controller->getPreferences();
    Vocabulary* vocab = vocabItem->getVocabulary();
    vocabDetailsVocabTitleLineEdit->setText( vocab->getTitle() );
    vocabDetailsPropsPanel->setVocabulary( vocab );
    termList->setColumnText( 0, QObject::tr( prefs.getFirstLanguage() ) );
    termList->setColumnText( 1, QObject::tr( prefs.getTestLanguage() ) );
    termList->clear();
    for( Vocabulary::TermMap::Iterator it = vocab->begin(); it != vocab->end(); it++ ) {
        Term& term = it.data(); 

        if( term.isMarkedForDeletion() )
            continue;

        if( !prefs.isLanguageFilterEnabled() ||
                ( term.isTranslationExists( prefs.getFirstLanguage() ) &&
                term.isTranslationExists( prefs.getTestLanguage() ) ) ) {
            TermListItem* termItem = new TermListItem( termList, &term, prefs.getFirstLanguage(), prefs.getTestLanguage(), prefs.isAltInTermListShown() );
            termItem->setOn( term.isMarkedForStudy() );
            termItem->setFont( 0, prefs.getMediumFont( prefs.getFirstLanguage() ) );
            termItem->setFont( 1, prefs.getMediumFont( prefs.getTestLanguage() ) );
        }
    }
    termList->verticalScrollBar()->setValue( 0 );
    vocabItem->setOpen( true );
    addVocabButton->setEnabled( false );
    addFolderButton->setEnabled( false );
    detailsPanel->raiseWidget( panelVocabIndex );
    updateTermList();
}

void VocabularyManagerFrame::updateTermList() {
    termList->updateColumnsWidth();
    uint selectedTermCount = getSelectedTermCount();
    editTermButton->setEnabled( selectedTermCount == 1 );
    removeTermButton->setEnabled( selectedTermCount > 0 );
}

FolderTreeItem* VocabularyManagerFrame::addFolder() {
    return( addFolder( NULL ) );
}

//FolderTreeItem* VocabularyManagerFrame::addFolder( Folder* folder, QMap<int,Vocabulary>* vocabularies = NULL ) {
//    FolderTreeItem* newFolderItem = vocabTreeView->addFolder( folder, vocabularies );
//    if( newFolderItem ) {
//        newFolderItem->setPropertiesPanel( folderDetailsPropsPanel );
//        folderDetailsPropsPanel->setFolder( newFolderItem->getFolder() );
//        folderDetailsPropsPanel->updateCounters();
//        vocabTreeView->ensureItemVisible( newFolderItem );
//    }
//    return( newFolderItem );
//}

FolderTreeItem* VocabularyManagerFrame::addFolder( Folder* folder, QMap<QString,Vocabulary>* vocabularies = NULL ) {
    FolderTreeItem* newFolderItem = vocabTreeView->addFolder( folder, vocabularies );
    if( newFolderItem ) {
        newFolderItem->setPropertiesPanel( folderDetailsPropsPanel );
        folderDetailsPropsPanel->setFolder( newFolderItem->getFolder() );
        folderDetailsPropsPanel->updateCounters();
        vocabTreeView->ensureItemVisible( newFolderItem );
    }
    return( newFolderItem );
}

VocabTreeItem* VocabularyManagerFrame::addVocab() {
    return( addVocab( NULL ) );
}

VocabTreeItem* VocabularyManagerFrame::addVocab( Vocabulary* vocab ) {
    VocabTreeItem* newVocabItem = vocabTreeView->addVocab( vocab );
    if( newVocabItem ) {
        newVocabItem->setPropertiesPanel( vocabDetailsPropsPanel );
        vocabDetailsPropsPanel->setVocabulary( newVocabItem->getVocabulary() );
        vocabDetailsPropsPanel->updateCounters();
        vocabTreeView->ensureItemVisible( newVocabItem );
    }
    return( newVocabItem );
}

void VocabularyManagerFrame::removeItem() {
    doRemoveItem();
}

void VocabularyManagerFrame::doRemoveItem( bool confirmBeforeRemove = true ) {
    int response;
    if( confirmBeforeRemove ) {
        QMessageBox msgBox( QObject::tr( "Warning" ), tr( "ConfirmRemoveItem" ),
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
        TreeItem* currentItem = (TreeItem*)vocabTreeView->currentItem();
        if( currentItem->isFolder() ) {
            FolderTreeItem* folderItem = (FolderTreeItem*)currentItem;
            Folder* folder = folderItem->getFolder();
            folder->setMarkedForDeletion( true );
            //folder->getParent()->remove( folder, false );
        }
        else {
            VocabTreeItem* vocabItem = (VocabTreeItem*)currentItem;
            Vocabulary* vocab = vocabItem->getVocabulary();
            vocab->setMarkedForDeletion( true );
            //vocab->getParent()->remove( vocab, false );
        }
        vocabTreeView->removeItem();
    }
}

void VocabularyManagerFrame::checkAllTerms() {
    TermListItem* termItem = (TermListItem*)termList->firstChild();
    while( termItem ) {
        termItem->setOn( true );
        termItem = (TermListItem*)termItem->nextSibling();
    }
    vocabDetailsPropsPanel->updateCounters();
}

void VocabularyManagerFrame::inverseCheckedTerms() {
    TermListItem* termItem = (TermListItem*)termList->firstChild();
    while( termItem ) {
        termItem->setOn( !termItem->isOn() );
        termItem = (TermListItem*)termItem->nextSibling();
    }
    vocabDetailsPropsPanel->updateCounters();
}

void VocabularyManagerFrame::addTerm() {
    TreeItem* currItem = (TreeItem*)vocabTreeView->currentItem();
    if( currItem && !currItem->isFolder() ) {
        VocabTreeItem* vocabItem = (VocabTreeItem*)vocabTreeView->currentItem();
        if( vocabItem ) {
            Vocabulary* vocab = vocabItem->getVocabulary();
            TermDialog dialog( *vocab, controller, this );
            dialog.showMaximized();
            int result = dialog.exec();
            if( result ) {
                Term newTerm( dialog.getTerm() );
                vocab->addTerm( newTerm );
                Term& term = vocab->getTerm( newTerm.getUid() );
                const Preferences& prefs = controller->getPreferences();
                TermListItem* newTermListItem = new TermListItem( termList, &term, 
                    prefs.getFirstLanguage(), prefs.getTestLanguage(), prefs.isAltInTermListShown() );
                newTermListItem->setOn( true );
                vocab->setModificationDate( QDateTime::currentDateTime() );
                vocab->setDirty( true );
                vocabDetailsPropsPanel->updateCounters();
                updateUi(); 
                for( TermListItem* termItem = (TermListItem*)termList->firstChild(); termItem; termItem = (TermListItem*)termItem->nextSibling() ) {
                    Term* termToSelect = termItem->getTerm();
                    if( termToSelect->getUid() == newTerm.getUid() ) {
                        termList->setSelected( termItem, true );
                        termList->ensureItemVisible( termItem );
                        break;
                    }
                }
            }
        }
    }
}

void VocabularyManagerFrame::editTerm() {
    VocabTreeItem* vocabItem = (VocabTreeItem*)vocabTreeView->currentItem();
    if( vocabItem ) {
        Vocabulary* vocab = vocabItem->getVocabulary();

        for( TermListItem* termItem = (TermListItem*)termList->firstChild(); termItem; termItem = (TermListItem*)termItem->nextSibling() ) {
            if( termItem->isSelected() ) {
                TermDialog dialog( *vocab, controller, this, *termItem->getTerm() );
                dialog.showMaximized();
                int result = dialog.exec();
                if( result ) { 
                    Term newTerm = dialog.getTerm();
                    termItem->getTerm()->addTranslation( newTerm.getTranslation( controller->getPreferences().getFirstLanguage() ) );
                    termItem->getTerm()->addTranslation( newTerm.getTranslation( controller->getPreferences().getTestLanguage() ) );
                    BilingualKey commentKey( controller->getPreferences().getFirstLanguage(), controller->getPreferences().getTestLanguage() );
                    termItem->getTerm()->addComment( commentKey, newTerm.getComment( commentKey ) );
                    termItem->getTerm()->setImagePath( newTerm.getImagePath() );
                    termItem->updateUi();
                    vocab->setModificationDate( QDateTime::currentDateTime() );
                    vocab->setDirty( true );
                }
                break;
            }
        }
    }
}

void VocabularyManagerFrame::removeTerms() {
    doRemoveTerms();
}

void VocabularyManagerFrame::doRemoveTerms( bool allowSelectTrans = true, bool confirmBeforeRemove = true ) {
    int selectedItemCount = 0;
    // Find all the translation languages of the selected terms.
    QStringList translationLanguages;

    for( TermListItem* termItem = (TermListItem*)termList->firstChild(); termItem; termItem = (TermListItem*)termItem->nextSibling() ) {
        if( termList->isSelected( termItem ) ) {
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
            VocabTreeItem* vocabItem = (VocabTreeItem*)vocabTreeView->currentItem();
            Vocabulary* vocab = vocabItem->getVocabulary();

            for( TermListItem* termItem = (TermListItem*)termList->firstChild(); termItem; ) {
                TermListItem* nextTermItem = (TermListItem*)termItem->nextSibling();
                if( termList->isSelected( termItem ) ) {
                    Term* term = termItem->getTerm();
                    term->setMarkedForDeletion( true );
                    cerr << "ALLO!! term " << term->getUid().toString() << " has been marked!!!" << endl;
                    delete( termItem );
                }
                termItem = nextTermItem;
            }

            vocab->setModificationDate( QDateTime::currentDateTime() );
            vocab->setDirty( true );
            termList->clearSelection();
            vocabDetailsPropsPanel->updateCounters();
            updateUi();
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

        VocabTreeItem* vocabItem = (VocabTreeItem*)vocabTreeView->currentItem();
        Vocabulary* vocab = vocabItem->getVocabulary();

        for( TermListItem* termItem = (TermListItem*)termList->firstChild(); termItem; ) {
            TermListItem* nextTermItem = (TermListItem*)termItem->nextSibling();

            if( termList->isSelected( termItem ) ) {
                Term* term = termItem->getTerm();

                for( QStringList::ConstIterator it = selectedLanguages.begin(); it != selectedLanguages.end(); it++ ) {
                    QString lang = *it;
                    term->removeTranslation( lang );
                }
                
                if( term->getTranslationCount() == 0 ) {
                    term->setMarkedForDeletion( true );
                    cerr << "ALLO!! term " << term->getUid().toString() << " has been marked!!!" << endl;
                    delete( termItem );
                }
            }

            termItem = nextTermItem;
        }

        vocab->setModificationDate( QDateTime::currentDateTime() );
        vocab->setDirty( true );
        termList->clearSelection();
        vocabDetailsPropsPanel->updateCounters();
        updateUi();
    }
}

void VocabularyManagerFrame::updateCurrentFolderTitle( const QString& title ) {
    FolderTreeItem* folderItem = (FolderTreeItem*)vocabTreeView->currentItem();
    folderItem->setText( 0, title );
    Folder* folder = folderItem->getFolder();
    folder->setTitle( title );
    folder->setModificationDate( QDateTime::currentDateTime() );
    folder->setDirty( true );
}

void VocabularyManagerFrame::updateCurrentVocabTitle( const QString& title ) {
    VocabTreeItem* vocabItem = (VocabTreeItem*)vocabTreeView->currentItem();
    vocabItem->setText( 0, title );
    Vocabulary* vocab = vocabItem->getVocabulary();
    vocab->setTitle( title );
    vocab->setModificationDate( QDateTime::currentDateTime() );
    vocab->setDirty( true );
}

FolderTreeItem* VocabularyManagerFrame::buildTreeRec( VocabTreeView* vocabTreeView, FolderTreeItem* parentItem, Folder* folder, bool closeFolder = false ) {
    //cerr << "buildTreeRec tv=" << vocabTreeView << " parentІtem=" << parentItem << " folder=" << folder << endl;
    //if( parentItem )
    //    cerr << "parentItemFolder=" << parentItem->getFolder()->getTitle() << endl; 
    //if( folder )
    //    cerr << "folder title=" << folder->getTitle() << " isMarkedForDeletion=" << folder->isMarkedForDeletion() << endl;
    FolderTreeItem* folderItem = NULL;
    if( !folder->isMarkedForDeletion() ) {
        if( parentItem )
            folderItem = new FolderTreeItem( parentItem, folder, controller->getPreferences() ); 
        else {
            // Root folder.
            folderItem = new FolderTreeItem( vocabTreeView, folder, controller->getPreferences() );
            folderItem->setEnabled( false );
        }
        if( folderItem ) {
            if( closeFolder )
                folderItem->setOpen( false );
            else
                folderItem->setOpen( controller->getPreferences().isFolderOpen( folder->getUid() ) );
            folderItem->setOn( folder->isMarkedForStudy() );
            folderItem->setPropertiesPanel( folderDetailsPropsPanel );
            //cerr << "folder isEmpty=" << folder->isEmpty() << endl;
            if( !folder->isEmpty() ) {
                QString firstLang = controller->getPreferences().getFirstLanguage();
                QString testLang = controller->getPreferences().getTestLanguage();
                bool isBothLangSet = ( firstLang != QString::null && testLang != QString::null );
                //cerr << "both lang set" << endl;
                if( isBothLangSet ) {
                    for( Base* folderChild = folder->first(); folderChild; folderChild = folder->next() ) {
                        if( strcmp( folderChild->className(), "Folder" ) == 0 ) {
                            Folder* childFolder = (Folder*)folderChild;
                            buildTreeRec( vocabTreeView, folderItem, childFolder, closeFolder );
                        }
                        else if( strcmp( folderChild->className(), "Vocabulary" ) == 0 ) {
                            Vocabulary* vocab = (Vocabulary*)folderChild;
                            buildTreeRec( folderItem, vocab );
                        }
                    }
                    if( folderItem->childCount() == 0 && folderItem->getFolder()->getParent() ) {
                        cerr << "delete the folder item=" << folderItem << endl;
                        delete folderItem;
                        folderItem = NULL;
                    }
                }
            }
        }
    }
    return( folderItem );
}

VocabTreeItem* VocabularyManagerFrame::buildTreeRec( FolderTreeItem* parentItem, Vocabulary* vocab ) {
    QString firstLang = controller->getPreferences().getFirstLanguage();
    QString testLang = controller->getPreferences().getTestLanguage();
    bool isVocabVisible = !vocab->isMarkedForDeletion() && 
        ( !controller->getPreferences().isLanguageFilterEnabled() || vocab->containsTermWithTranslations( firstLang, testLang ) );
    //cerr << "buildTreeRec parentItem=" << parentItem << " vocab=" << vocab << " isVocabVisible=" << isVocabVisible << endl;
    if( isVocabVisible ) {
        VocabTreeItem* vocabItem = new VocabTreeItem( parentItem, vocab );    
        vocabItem->setOpen( false );
        vocabItem->setOn( vocab->isMarkedForStudy() );
        vocabItem->setPropertiesPanel( vocabDetailsPropsPanel );
        return( vocabItem );
    }
    else
        return( NULL );
}

void VocabularyManagerFrame::copyTerms() const {
    QString firstLang( controller->getPreferences().getFirstLanguage() );
    QString testLang( controller->getPreferences().getTestLanguage() );
    QValueList<Term> termsToCopy;

    TermListItem* termItem = (TermListItem*)termList->firstChild();
    while( termItem ) {
        if( termList->isSelected( termItem ) ) {
            Term* term = termItem->getTerm();
            Term* termCopy = new Term( 0, 0 ); // Don't need to copy ids.

            // We copy only translations for the currenlty selected languages.
            QStringList languages;
            languages << firstLang << testLang;

            for( QStringList::ConstIterator it = languages.begin(); it != languages.end(); it++ ) {
                const QString& lang = *it;
                if( term->isTranslationExists( lang ) ) {
                    Translation transCopy( term->getTranslation( lang ) );
                    termCopy->addTranslation( transCopy );
                }
            }
            BilingualKey commentKey( firstLang, testLang );
            if( term->isCommentExists( commentKey ) )
                termCopy->addComment( commentKey, term->getComment( commentKey ) );

            QString absoluteImagePath = term->getImagePath();
            if( !absoluteImagePath.isNull() && absoluteImagePath.left( 1 ) != "/" ) {
                TreeItem* currItem = (TreeItem*)vocabTreeView->currentItem();
                if( currItem && !currItem->isFolder() ) {
                    VocabTreeItem* vocabItem = (VocabTreeItem*)vocabTreeView->currentItem();
                    Vocabulary* vocab = vocabItem->getVocabulary();
                    absoluteImagePath = controller->getApplicationDirName() + "/" + vocab->getParent()->getPath() +
                        "/v-" + vocab->getUid().toString() + "/" + absoluteImagePath;
                }
            }
            cerr << "absoluteImagePath=" << absoluteImagePath << endl;
            termCopy->setImagePath( absoluteImagePath );

            termsToCopy.append( *termCopy );
        }
        termItem = (TermListItem*)termItem->nextSibling();
    }

    if( !termsToCopy.isEmpty() ) {
        QByteArray data;
        QDataStream out( data, IO_WriteOnly );
        out << termsToCopy;
        controller->setClipboardData( QString( "terms" ), Util::qCompress( data ) );
    }
    cerr << "copyTerms done" << endl;
}

void VocabularyManagerFrame::pasteTerms() {
    QByteArray data( Util::qUncompress( controller->getClipboardData() ) );
    QDataStream in( data, IO_ReadOnly );

    QValueList<Term> termsToPaste;
    in >> termsToPaste;

    VocabTreeItem* currVocabItem = (VocabTreeItem*)vocabTreeView->currentItem();
    Vocabulary* currVocab = currVocabItem->getVocabulary();
    for( QValueList<Term>::ConstIterator it = termsToPaste.begin(); it != termsToPaste.end(); it++ ) {
        const Term& term = *it;
        Term newTerm( 0, currVocab->getUid(), Util::createUuid() );
        for( Term::TranslationMap::ConstIterator it2 = term.translationsBegin(); it2 != term.translationsEnd(); it2++ ) {
            const Translation& trans = it2.data();
            newTerm.addTranslation( trans );
        }
        for( Term::CommentMap::ConstIterator it3 = term.commentsBegin(); it3 != term.commentsEnd(); it3++ ) {
            const BilingualKey& key = it3.key();
            const QString& comment = it3.data();
            newTerm.addComment( key, comment );
        }
        if( !term.getImagePath().isNull() ) {
            // If the path refers to a file outside toMOTko's directory, we just copy the path.
            // Otherwise, we copy the image as well as the container vocabulary may be moved or deleted ulteriorly.
            if( term.getImagePath().left( controller->getApplicationDirName().length() ) == controller->getApplicationDirName() ) {
                const QString& vocabLocation = controller->getApplicationDirName() + "/" + currVocab->getParent()->getPath() +
                    "/v-" + currVocab->getUid().toString() + "/";
                QFileInfo imageToCopyInfo( term.getImagePath() );
                QString imageFilename = imageToCopyInfo.fileName();
                if( Util::copy( term.getImagePath(), vocabLocation + imageFilename ) )
                    newTerm.setImagePath( imageFilename ); 
                else
                    cerr << "Could not copy " << term.getImagePath() << " to " << ( vocabLocation + imageFilename ) << endl;
            }
            else 
                newTerm.setImagePath( term.getImagePath() ); 
        }

        currVocab->addTerm( newTerm );
        currVocab->setModificationDate( QDateTime::currentDateTime() );
        currVocab->setDirty( true );
    }
    vocabDetailsPropsPanel->updateCounters();
    updateUi();
}

void VocabularyManagerFrame::pasteVocabulary() {
    QByteArray data( Util::qUncompress( controller->getClipboardData() ) );
    QDataStream in( data, IO_ReadOnly );

    Vocabulary vocabToPaste;
    in >> vocabToPaste;

    VocabTreeItem* newVocabItem = addVocab( &vocabToPaste );
    vocabTreeView->setSelected( newVocabItem, true ); 
}

void VocabularyManagerFrame::pasteFolder() {
cerr << "pasteFolder begin" << endl;
    QByteArray data( Util::qUncompress( controller->getClipboardData() ) );
    QDataStream in( data, IO_ReadOnly );

    Folder folderToPaste;
    QMap<QString,Vocabulary> vocabularies;
    cerr << 1 << endl;
    in >> folderToPaste;
    cerr << 2 << endl;
    in >> vocabularies;
    cerr << 3 << endl;

    FolderTreeItem* newFolderItem = addFolder( &folderToPaste, &vocabularies );
    // As the tree updates are disabled when adding new folders recursively,
    // we close the parent node.  When we select the new node, the parent node
    // will open when we call ensureItemVisible().  This way, the tree is updated
    // properly.
    newFolderItem->parent()->setOpen( false );
    vocabTreeView->setSelected( newFolderItem, true );
    vocabTreeView->ensureItemVisible( newFolderItem );
cerr << "pasteFolder end" << endl;
}

void VocabularyManagerFrame::addListeners() {
    connect( folderDetailsFolderTitleLineEdit, SIGNAL( textChanged( const QString& ) ),
        this, SLOT( updateCurrentFolderTitle( const QString& ) ) );
    connect( vocabDetailsVocabTitleLineEdit, SIGNAL( textChanged( const QString& ) ),
        this, SLOT( updateCurrentVocabTitle( const QString& ) ) );
}

void VocabularyManagerFrame::removeListeners() {
    disconnect( folderDetailsFolderTitleLineEdit, SIGNAL( textChanged( const QString& ) ),
        this, SLOT( updateCurrentFolderTitle( const QString& ) ) );
    disconnect( vocabDetailsVocabTitleLineEdit, SIGNAL( textChanged( const QString& ) ),
        this, SLOT( updateCurrentVocabTitle( const QString& ) ) );
}

void VocabularyManagerFrame::maximizeDetailsPanel() {
    action[ ACTION_MAXIMIZE ]->setOn( true );
    // Set both folder and vocab maximize buttons to the same state because easier to manage.
    folderDetailsFolderMaximizeDetailsButton->setOn( true );
    vocabDetailsVocabMaximizeDetailsButton->setOn( true );
    treePanel->hide();
    updateGeometry();
    update();
}

void VocabularyManagerFrame::restoreDetailsPanel() {
    action[ ACTION_MAXIMIZE ]->setOn( false );
    // Set both folder and vocab maximize buttons to the same state because easier to manage.
    folderDetailsFolderMaximizeDetailsButton->setOn( false );
    vocabDetailsVocabMaximizeDetailsButton->setOn( false );
    treePanel->show();
    updateGeometry();
    update();
}

uint VocabularyManagerFrame::getSelectedTermCount() const {
    uint count = 0;
    TermListItem* termItem = (TermListItem*)termList->firstChild();
    while( termItem ) {
        if( termList->isSelected( termItem ) )
            count++;
        termItem = (TermListItem*)termItem->nextSibling();
    }
    return( count );
}

void VocabularyManagerFrame::search() {
    if( !searchDialog ) {
        searchDialog = new SearchDialog( this, controller );
        searchDialog->setDigraphEnabled( controller->getPreferences().isDigraphEnabled() );
        connect( searchDialog, SIGNAL( showTermRequested( const TermKey& ) ), this, SLOT( showTerm( const TermKey& ) ) ); 
        connect( searchDialog, SIGNAL( termsRemoved() ), this, SLOT( updateUi() ) );
    }
    searchDialog->updateFonts();
    searchDialog->showMaximized();
    searchDialog->exec();
}

void VocabularyManagerFrame::showTerm( const TermKey& termKey ) {
    VocabTreeItem* vocabTreeItem = vocabTreeView->getVocabTreeItem( termKey.getVocabUid() );
    if( vocabTreeItem ) {
        vocabTreeView->ensureItemVisible( vocabTreeItem );
        vocabTreeView->setSelected( vocabTreeItem, true );
        detailsPanel->raiseWidget( panelVocabIndex );
        vocabDetailsTabWidget->showPage( vocabDetailsTermsPanel );
        for( TermListItem* termItem = (TermListItem*)termList->firstChild(); termItem; termItem = (TermListItem*)termItem->nextSibling() ) {
            QUuid termItemUid = termItem->getTerm()->getUid();
            if( termItemUid == termKey.getTermUid() ) {
                termList->clearSelection();
                termList->ensureItemVisible( termItem );
                termList->setSelected( termItem, true );
                break;
            }
        }
    }
}
