#include "MainWindow.h"
#include "icons/about.xpm"
#include "icons/startQuiz.xpm"
#include "icons/glossaryManager.xpm"
#include "icons/import.xpm"
#include "icons/export.xpm"
#include "icons/preferences.xpm"
#include "icons/quit.xpm"
#include "icons/editcut.xpm"
#include "icons/editcopy.xpm"
#include "icons/editpaste.xpm"
#include "icons/blueArrow.xpm"
#include "icons/eye.xpm"
#include "icons/goodAns.xpm"
#include "icons/badAns.xpm"
#include "icons/addTerm.xpm"
#include "icons/editTerm.xpm"
#include "icons/maximize.xpm"
#include "icons/search.xpm"
#include "icons/removeTerm.xpm"
#include "icons/addFolder.xpm"
#include "icons/addVocab.xpm"
#include "icons/removeItem.xpm"
#include "icons/checkAllTerms.xpm"
#include "icons/inverseCheckedTerms.xpm"
#include "icons/gradeAnswer1.xpm"
#include "icons/gradeAnswer2.xpm"
#include "icons/gradeAnswer3.xpm"
#include "icons/gradeAnswer4.xpm"
#include "icons/gradeAnswer5.xpm"
#include "icons/gradeAnswer6.xpm"
#include "icons/flag_en.xpm"
#include "icons/flag_es.xpm"
#include "icons/flag_fr.xpm"
#include "icons/flag_ja.xpm"
#include "icons/flag_zh.xpm"
#include "icons/flag_de.xpm"

MainWindow::MainWindow( Controller* controller )
    : QMainWindow( 0, "example application main window", WDestructiveClose ), control( controller ) {
    Preferences& prefs( controller->getPreferences() );

    translator = new QTranslator( this );
    translator->load( "toMOTko", QString( "/opt/Qtopia/i18n/" ) + prefs.getInterfaceLanguage() );
    qApp->installTranslator( translator );

    setToolBarsMovable( false );

    QPEToolBar* topBar = new QPEToolBar( this );
    topBar->setHorizontalStretchable( true );

    menuBar = new QPEMenuBar( topBar );
    menuBar->setMargin( 0 );

    toolBar = new QPEToolBar( this );

    languageSelectorPanel = new QHBox( toolBar, "LanguageSelectorPanel" );
    firstLanguageComboBox = new QComboBox( languageSelectorPanel, "FirstLanguageComboBox" );
    languageSelectorLabel = new QLabel( languageSelectorPanel, "LanguageSelectorLabel" );
    languageSelectorLabel->setPixmap( ZPIXMAP( blueArrow_xpm ) );
    testLanguageComboBox = new QComboBox( languageSelectorPanel, "TestLanguageComboBox" );
    updateFirstLanguageValues();
    updateTestLanguageValues();
    connect( firstLanguageComboBox, SIGNAL( activated( const QString& ) ), this, SLOT( setFirstLanguage( const QString& ) ) );
    connect( testLanguageComboBox, SIGNAL( activated( const QString& ) ), this, SLOT( setTestLanguage( const QString& ) ) );

    copyAction = Util::createAction( QObject::tr( "Copy" ), editcopy_xpm, this, SLOT( copy() ), CTRL + Key_C );
    cutAction = Util::createAction( QObject::tr( "Cut" ), editcut_xpm, this, SLOT( cut() ), CTRL + Key_X );
    pasteAction = Util::createAction( QObject::tr( "Paste" ), editpaste_xpm, this, SLOT( paste() ), CTRL + Key_V );

    progressBar = new ProgressBar( toolBar, "ProgressBar" );
    connect( controller, SIGNAL( progressChanged( int ) ), progressBar, SLOT( setProgress( int ) ) );
    connect( progressBar, SIGNAL( progressBarClicked() ), this, SLOT( showProgressDetails() ) );
    progressBar->hide();
    progressBar->setCenterIndicator( true );

    setIcon( Resource::loadPixmap( QString( "toMOTko" ) ) ); 

    mainPanel = new QWidgetStack( this, "MainPanel" );

    quizFrame = new QuizFrame( control, mainPanel, "QuizFrame" );

    vocabManagerFrame = new VocabularyManagerFrame( control, mainPanel, "VocabularyManagerFrame" );
    vocabManagerFrame->setDigraphEnabled( prefs.isDigraphEnabled() ); 

    action[ ACTION_REVEAL ] = Util::createAction( tr( "Reveal" ), eye_xpm, 
        quizFrame, SLOT( reveal() ), prefs.getAccelerator( ACTION_REVEAL ) );
    action[ ACTION_RIGHT_ANSWER ] = Util::createAction( tr( "RightAnswer" ), goodAns_xpm, 
        quizFrame, SLOT( rightAnswer() ), prefs.getAccelerator( ACTION_RIGHT_ANSWER ) );
    action[ ACTION_WRONG_ANSWER ] = Util::createAction( tr( "WrongAnswer" ), badAns_xpm, 
        quizFrame, SLOT( wrongAnswer() ), prefs.getAccelerator( ACTION_WRONG_ANSWER ) );
    action[ ACTION_EDIT_QUIZ_TERM ] = Util::createAction( tr( "EditQuizTerm" ), editTerm_xpm, 
        quizFrame, SLOT( editCurrentTerm() ), prefs.getAccelerator( ACTION_EDIT_QUIZ_TERM ) );
    action[ ACTION_START_QUIZ ] = Util::createAction( tr( "StartQuiz" ), startQuiz_xpm, 
        this, SLOT( startQuiz() ), prefs.getAccelerator( ACTION_START_QUIZ ) );
    action[ ACTION_MANAGE_GLOSSARIES ] = Util::createAction( tr( "ManageGlossaries" ), glossaryManager_xpm, 
        this, SLOT( invokeVocabularyManager() ), prefs.getAccelerator( ACTION_MANAGE_GLOSSARIES ) );
    action[ ACTION_IMPORT ] = Util::createAction( tr( "Import..." ), import_xpm, 
        this, SLOT( importData() ), prefs.getAccelerator( ACTION_IMPORT ) );
    action[ ACTION_EXPORT ] = Util::createAction( tr( "Export..." ), export_xpm, 
        this, SLOT( exportData() ), prefs.getAccelerator( ACTION_EXPORT ) );
    action[ ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS ] = Util::createAction( tr( "ShowAllGlossariesAndTerms" ), NULL, 
        this, SLOT( toggleLanguageFilter() ), prefs.getAccelerator( ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS ), true );
    action[ ACTION_PREFERENCES ] = Util::createAction( tr( "Preferences..." ), preferences_xpm, 
        this, SLOT( preferences() ), prefs.getAccelerator( ACTION_PREFERENCES ) );
    action[ ACTION_QUIT ] = Util::createAction( tr( "Quit" ), quit_xpm, 
        this, SLOT( quit() ), prefs.getAccelerator( ACTION_QUIT ) );
    action[ ACTION_ADD_FOLDER ] = Util::createAction( tr( "AddFolder" ), addFolder_xpm, 
        vocabManagerFrame, SLOT( addFolder() ), prefs.getAccelerator( ACTION_ADD_FOLDER ) );
    action[ ACTION_ADD_GLOSSARY ] = Util::createAction( tr( "AddGlossary" ), addVocab_xpm, 
        vocabManagerFrame, SLOT( addVocab() ), prefs.getAccelerator( ACTION_ADD_GLOSSARY ) );
    action[ ACTION_REMOVE_ITEM ] = Util::createAction( tr( "RemoveItem" ), removeItem_xpm, 
        vocabManagerFrame, SLOT( removeItem() ), prefs.getAccelerator( ACTION_REMOVE_ITEM ) );
    action[ ACTION_ADD_TERM ] = Util::createAction( tr( "AddTerm" ), addTerm_xpm, 
        vocabManagerFrame, SLOT( addTerm() ), prefs.getAccelerator( ACTION_ADD_TERM ) );
    action[ ACTION_EDIT_TERM ] = Util::createAction( tr( "EditTerm" ), editTerm_xpm, 
        vocabManagerFrame, SLOT( editTerm() ), prefs.getAccelerator( ACTION_EDIT_TERM ) );
    action[ ACTION_REMOVE_TERMS ] = Util::createAction( tr( "RemoveTerm" ), removeTerm_xpm, 
        vocabManagerFrame, SLOT( removeTerms() ), prefs.getAccelerator( ACTION_REMOVE_TERMS ) );
    action[ ACTION_CHECK_ALL_TERMS ] = Util::createAction( tr( "CheckAllTerms" ), checkAllTerms_xpm, 
        vocabManagerFrame, SLOT( checkAllTerms() ), prefs.getAccelerator( ACTION_CHECK_ALL_TERMS ) );
    action[ ACTION_INVERSE_CHECKED_TERMS ] = Util::createAction( tr( "InverseCheckedTerms" ), 
        inverseCheckedTerms_xpm, vocabManagerFrame, SLOT( inverseCheckedTerms() ), prefs.getAccelerator( ACTION_INVERSE_CHECKED_TERMS ) );
    action[ ACTION_MAXIMIZE ] = Util::createAction( tr( "Maximize" ), maximize_xpm, 
        this, SLOT( toggleMaximize( bool ) ), prefs.getAccelerator( ACTION_MAXIMIZE ), true );
    action[ ACTION_SEARCH ] = Util::createAction( tr( "Search..." ), search_xpm, 
        this, SLOT( search() ), prefs.getAccelerator( ACTION_SEARCH ) );
    action[ ACTION_GRADE_ANSWER_1 ] = Util::createAction( tr( "gradeAnswer1" ), gradeAnswer1_xpm,
        quizFrame, SLOT( gradeAnswer1() ), prefs.getAccelerator( ACTION_GRADE_ANSWER_1 ) );
    action[ ACTION_GRADE_ANSWER_2 ] = Util::createAction( tr( "gradeAnswer2" ), gradeAnswer2_xpm,
        quizFrame, SLOT( gradeAnswer2() ), prefs.getAccelerator( ACTION_GRADE_ANSWER_2 ) );
    action[ ACTION_GRADE_ANSWER_3 ] = Util::createAction( tr( "gradeAnswer3" ), gradeAnswer3_xpm,
        quizFrame, SLOT( gradeAnswer3() ), prefs.getAccelerator( ACTION_GRADE_ANSWER_3 ) );
    action[ ACTION_GRADE_ANSWER_4 ] = Util::createAction( tr( "gradeAnswer4" ), gradeAnswer4_xpm,
        quizFrame, SLOT( gradeAnswer4() ), prefs.getAccelerator( ACTION_GRADE_ANSWER_4 ) );
    action[ ACTION_GRADE_ANSWER_5 ] = Util::createAction( tr( "gradeAnswer5" ), gradeAnswer5_xpm,
        quizFrame, SLOT( gradeAnswer5() ), prefs.getAccelerator( ACTION_GRADE_ANSWER_5 ) );
    action[ ACTION_GRADE_ANSWER_6 ] = Util::createAction( tr( "gradeAnswer6" ), gradeAnswer6_xpm,
        quizFrame, SLOT( gradeAnswer6() ), prefs.getAccelerator( ACTION_GRADE_ANSWER_6 ) );

    actionsMenu = new QPopupMenu( this );
    actionsMenu->setCheckable( true );
    menuBar->insertItem( tr( "Actions" ), actionsMenu );
   
    action[ ACTION_START_QUIZ ]->addTo( actionsMenu );

    action[ ACTION_MANAGE_GLOSSARIES ]->addTo( actionsMenu );

    showAllVocabSeparatorId = actionsMenu->insertSeparator();
    action[ ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS ]->addTo( actionsMenu );

    actionsMenu->insertSeparator();
    action[ ACTION_IMPORT ]->addTo( actionsMenu );
    action[ ACTION_EXPORT ]->addTo( actionsMenu );

    actionsMenu->insertSeparator();

    action[ ACTION_PREFERENCES ]->addTo( actionsMenu );

    actionsMenu->insertSeparator();

    action[ ACTION_QUIT ]->addTo( actionsMenu );

    editionMenu = new QPopupMenu( this );
    cutAction->addTo( editionMenu );
    copyAction->addTo( editionMenu );
    pasteAction->addTo( editionMenu );

    editionMenu->insertSeparator();

    action[ ACTION_SEARCH ]->addTo( editionMenu );

    helpMenu = new QPopupMenu( this );
    menuBar->insertItem( tr( "?" ), helpMenu );
    helpMenu->insertItem( ZPIXMAP( about_xpm ), tr( "About..." ), this, SLOT( about() ) );
    helpMenu->insertSeparator( 1 );
    
    languageActionGroup = new QActionGroup( this ); 

    QDir i18nDir( QString( "/opt/Qtopia/i18n" ) );
    QStringList langDirs = i18nDir.entryList( QString( "*" ) );
    QStringList sortedLanguages;
    for( uint i = 0; i < langDirs.count(); i++ ) {
        if( langDirs[ i ] == QString( "." ) || langDirs[ i ] == QString( ".." ) )
            continue;
        QString locale = langDirs[ i ];
        QString langDirStr = QString( "/opt/Qtopia/i18n/" ) + QString( locale );
        QDir langDir( langDirStr );
        if( langDir.exists( QString( "toMOTko.qm" ) ) ) {
            QTranslator translator( this );
            translator.load( QString( "toMOTko.qm" ), langDirStr );
            QString language = translator.find( QString( "QObject" ), locale );
            sortedLanguages.append( language );
            availableLanguages[ language ] = locale;
        }
    }
    sortedLanguages.sort();
    for( uint i = 0; i < sortedLanguages.count(); i++ ) {
        QAction* langAction = new QAction( this );
        langAction->setToggleAction( true );
        langAction->setText( sortedLanguages[ i ] );
        
        // Refactor this if later.  Look at createAction() method for hint (maybe).
        QString langCode = availableLanguages[ sortedLanguages[ i ] ];
        if( langCode == QString( "en" ) )
            langAction->setIconSet( ZPIXMAP( flag_en_xpm ) );
        else if( langCode == QString( "es" ) )
            langAction->setIconSet( ZPIXMAP( flag_es_xpm ) );
        else if( langCode == QString( "fr" ) )
            langAction->setIconSet( ZPIXMAP( flag_fr_xpm ) );
        else if( langCode == QString( "ja" ) )
            langAction->setIconSet( ZPIXMAP( flag_ja_xpm ) );
        else if( langCode == QString( "zh" ) )
            langAction->setIconSet( ZPIXMAP( flag_zh_xpm ) );
        else if( langCode == QString( "de" ) )
            langAction->setIconSet( ZPIXMAP( flag_de_xpm ) );

        langAction->addTo( helpMenu );
        languageActionGroup->insert( langAction );
        if( controller->getPreferences().getInterfaceLanguage() == availableLanguages[ sortedLanguages[ i ] ] )
            langAction->setOn( true );
    }
    
    connect( quizFrame, SIGNAL( quizShown() ), progressBar, SLOT( show() ) );
    connect( quizFrame, SIGNAL( quizShown() ), languageSelectorPanel, SLOT( hide() ) );
    connect( quizFrame, SIGNAL( quizHidden() ), progressBar, SLOT( hide() ) );
    connect( quizFrame, SIGNAL( quizHidden() ), languageSelectorPanel, SLOT( show() ) );
    connect( quizFrame, SIGNAL( quizHidden() ), control, SLOT( concludeQuiz() ) );

    mainPanel->addWidget( quizFrame, frameQuizIndex );
    mainPanel->addWidget( vocabManagerFrame, frameVocabManagerIndex );
    connect( vocabManagerFrame, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( updateMenus( QListViewItem* ) ) );
    setCentralWidget( mainPanel );
    setLanguageFilterEnabled( controller->getPreferences().isLanguageFilterEnabled() );
    invokeVocabularyManager();

    setCaption( QString( "toMOTko" ) );

    updateFonts();

    connect( languageActionGroup, SIGNAL( selected( QAction* ) ), this, SLOT( switchLanguage( QAction* ) ) );
}

MainWindow::~MainWindow() {
    delete( control );
}

Controller* MainWindow::controller() {
    return( control );
}

void MainWindow::updateMenus( QListViewItem* ) {
    action[ ACTION_START_QUIZ ]->setText( mainPanel->visibleWidget() == quizFrame ? tr( "RestartQuiz" ) : tr( "StartQuiz" ) );
    action[ ACTION_START_QUIZ ]->setMenuText( mainPanel->visibleWidget() == quizFrame ? tr( "RestartQuiz" ) : tr( "StartQuiz" ) );
    action[ ACTION_MANAGE_GLOSSARIES ]->setEnabled( mainPanel->visibleWidget() != vocabManagerFrame ); 
    if( mainPanel->visibleWidget() == vocabManagerFrame ) {
        if( menuBar->indexOf( editionMenuId ) == -1 )
            editionMenuId = menuBar->insertItem( QObject::tr( "Edition" ), editionMenu, -1, 1 );
        action[ ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS ]->setEnabled( true );
        action[ ACTION_IMPORT ]->setEnabled( vocabManagerFrame->isImportAllowed() );
        action[ ACTION_EXPORT ]->setEnabled( vocabManagerFrame->isExportAllowed() );
    }
    else {
        if( editionMenu->parent() ) {
            if( menuBar->indexOf( editionMenuId ) != -1 )
                menuBar->removeItem( editionMenuId );
            if( menuBar->indexOf( viewMenuId ) != -1 )
                menuBar->removeItem( viewMenuId );
        }
        action[ ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS ]->setEnabled( false );
        action[ ACTION_IMPORT ]->setEnabled( false );
        action[ ACTION_EXPORT ]->setEnabled( false );
    }
}

void MainWindow::updateFonts() {
    QFont labelsFont( control->getPreferences().getLabelsFont() ); 

    qApp->setFont( labelsFont );
    firstLanguageComboBox->setFont( labelsFont );
    testLanguageComboBox->setFont( labelsFont );
    progressBar->setFont( labelsFont );

    quizFrame->updateFonts();
    vocabManagerFrame->updateFonts();
}

bool MainWindow::isDigraphEnabled() const {
    return( vocabManagerFrame->isDigraphEnabled() );
}

void MainWindow::setDigraphEnabled( bool isEnabled ) {
    vocabManagerFrame->setDigraphEnabled( isEnabled );
}

void MainWindow::retranslateUi() {
    menuBar->changeItem( menuBar->idAt( 0 ), tr( "Actions" ) );
    if( mainPanel->visibleWidget() == vocabManagerFrame )
        menuBar->changeItem( menuBar->idAt( 1 ), QObject::tr( "Edition" ) );

    // Could use a loop here.
    action[ ACTION_REVEAL ]->setText( tr( "Reveal" ) );
    action[ ACTION_REVEAL ]->setMenuText( tr( "Reveal" ) );
    action[ ACTION_RIGHT_ANSWER ]->setText( tr( "RightAnswer" ) );
    action[ ACTION_RIGHT_ANSWER ]->setMenuText( tr( "RightAnswer" ) );
    action[ ACTION_WRONG_ANSWER ]->setText( tr( "WrongAnswer" ) );
    action[ ACTION_WRONG_ANSWER ]->setMenuText( tr( "WrongAnswer" ) );
    action[ ACTION_EDIT_QUIZ_TERM ]->setText( tr( "EditQuizTerm" ) );
    action[ ACTION_EDIT_QUIZ_TERM ]->setMenuText( tr( "EditQuizTerm" ) );
    action[ ACTION_START_QUIZ ]->setText( tr( "StartQuiz" ) );
    action[ ACTION_START_QUIZ ]->setMenuText( tr( "StartQuiz" ) );
    action[ ACTION_MANAGE_GLOSSARIES ]->setText( tr( "ManageGlossaries" ) );
    action[ ACTION_MANAGE_GLOSSARIES ]->setMenuText( tr( "ManageGlossaries" ) );
    action[ ACTION_IMPORT ]->setText( tr( "Import..." ) );
    action[ ACTION_IMPORT ]->setMenuText( tr( "Import..." ) );
    action[ ACTION_EXPORT ]->setText( tr( "Export..." ) );
    action[ ACTION_EXPORT ]->setMenuText( tr( "Export..." ) );
    action[ ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS ]->setText( tr( "ShowAllGlossariesAndTerms" ) );
    action[ ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS ]->setMenuText( tr( "ShowAllGlossariesAndTerms" ) );
    action[ ACTION_PREFERENCES ]->setText( tr( "Preferences..." ) );
    action[ ACTION_PREFERENCES ]->setMenuText( tr( "Preferences..." ) );
    action[ ACTION_QUIT ]->setText( tr( "Quit" ) );
    action[ ACTION_QUIT ]->setMenuText( tr( "Quit" ) );
    action[ ACTION_ADD_FOLDER ]->setText( tr( "AddFolder" ) );
    action[ ACTION_ADD_FOLDER ]->setMenuText( tr( "AddFolder" ) );
    action[ ACTION_ADD_GLOSSARY ]->setText( tr( "AddGlossary" ) );
    action[ ACTION_ADD_GLOSSARY ]->setMenuText( tr( "AddGlossary" ) );
    action[ ACTION_REMOVE_ITEM ]->setText( tr( "RemoveItem" ) );
    action[ ACTION_REMOVE_ITEM ]->setMenuText( tr( "RemoveItem" ) );
    action[ ACTION_ADD_TERM ]->setText( tr( "AddTerm" ) );
    action[ ACTION_ADD_TERM ]->setMenuText( tr( "AddTerm" ) );
    action[ ACTION_EDIT_TERM ]->setText( tr( "EditTerm" ) );
    action[ ACTION_EDIT_TERM ]->setMenuText( tr( "EditTerm" ) );
    action[ ACTION_REMOVE_TERMS ]->setText( tr( "RemoveTerm" ) );
    action[ ACTION_REMOVE_TERMS ]->setMenuText( tr( "RemoveTerm" ) );
    action[ ACTION_CHECK_ALL_TERMS ]->setText( tr( "CheckAllTerms" ) );
    action[ ACTION_CHECK_ALL_TERMS ]->setMenuText( tr( "CheckAllTerms" ) );
    action[ ACTION_INVERSE_CHECKED_TERMS ]->setText( tr( "InverseCheckedTerms" ) );
    action[ ACTION_INVERSE_CHECKED_TERMS ]->setMenuText( tr( "InverseCheckedTerms" ) );
    action[ ACTION_MAXIMIZE ]->setText( tr( "Maximize" ) );
    action[ ACTION_MAXIMIZE ]->setMenuText( tr( "Maximize" ) );
    action[ ACTION_SEARCH ]->setText( tr( "Search..." ) );
    action[ ACTION_SEARCH ]->setMenuText( tr( "Search..." ) );

    cutAction->setText( QObject::tr( "Cut" ) );
    cutAction->setMenuText( QObject::tr( "Cut" ) );
    copyAction->setText( QObject::tr( "Copy" ) );
    copyAction->setMenuText( QObject::tr( "Copy" ) );
    pasteAction->setText( QObject::tr( "Paste" ) );
    pasteAction->setMenuText( QObject::tr( "Paste" ) );

    helpMenu->changeItem( helpMenu->idAt( 0 ), tr( "About..." ) );
    
    updateFirstLanguageValues();
    updateTestLanguageValues();

    quizFrame->retranslateUi();
    vocabManagerFrame->retranslateUi();
}

void MainWindow::switchLanguage( const QString& language ) {
    qApp->removeTranslator( translator );
    translator->load( "toMOTko", QString( "/opt/Qtopia/i18n/" ) + language );
    qApp->installTranslator( translator ); 
    retranslateUi();
}

void MainWindow::toggleMaximize( bool isOn ) {
    if( mainPanel->visibleWidget() == quizFrame )
        quizFrame->toggleMaximizeComment( isOn );
    else if( mainPanel->visibleWidget() == vocabManagerFrame )
        vocabManagerFrame->toggleMaximizeDetails( isOn );
}

void MainWindow::search() {
    if( mainPanel->visibleWidget() == vocabManagerFrame )
        vocabManagerFrame->search();
}

void MainWindow::showProgressDetails() {
    if( mainPanel->visibleWidget() == quizFrame )
        quizFrame->showProgressDetails();
}

void MainWindow::switchLanguage( QAction* langAction ) {
    QString language = availableLanguages[ langAction->text() ];
    switchLanguage( language );
    control->getPreferences().setInterfaceLanguage( language );
}

void MainWindow::closeEvent( QCloseEvent* ce ) {
    bool isOk = vocabManagerFrame->saveData();
    if( !isOk )
        QMessageBox::warning( this, QObject::tr( "Error" ), QObject::tr( "ErrorSavingData" ) );
    ce->accept();
}

void MainWindow::about() {
    QMessageBox::about( this, tr( "AboutToMOTko..." ), tr( "AboutMessage" ) );
}

void MainWindow::startQuiz() {
    if( mainPanel->visibleWidget() == quizFrame ) 
        quizFrame->restartQuiz();
    else {
        bool resumeQuiz = false;
        if( control->isResumableQuizAvailable() ) {
            QMessageBox msgBox( QObject::tr( "Information" ), tr( "ConfirmResumeQuiz" ),
                QMessageBox::Warning,
                QMessageBox::Yes | QMessageBox::Default | QMessageBox::Escape,
                QMessageBox::No,
                QMessageBox::NoButton,
                this );
            msgBox.setButtonText( QMessageBox::Yes, tr( "Yes" ) );
            msgBox.setButtonText( QMessageBox::No, tr( "No" ) );
        
            int response = msgBox.exec();
            resumeQuiz = ( response == QMessageBox::Yes );
        }
        showQuiz();
        if( resumeQuiz ) {
            if( !quizFrame->resumeQuiz() )
                QMessageBox::warning( this, QObject::tr( "Error" ), QObject::tr( "CannotResumeQuiz" ) );
        }
        else
            quizFrame->startQuiz();
    }
}

void MainWindow::showQuiz() {
    mainPanel->raiseWidget( frameQuizIndex );
    updateMenus( NULL );
    cutAction->removeFrom( toolBar );
    copyAction->removeFrom( toolBar );
    pasteAction->removeFrom( toolBar );
    quizFrame->setFocus();
}

void MainWindow::invokeVocabularyManager() {
    mainPanel->raiseWidget( frameVocabManagerIndex );
    updateMenus( NULL );
    cutAction->addTo( toolBar );
    copyAction->addTo( toolBar );
    pasteAction->addTo( toolBar );
}

void MainWindow::importData() {
    vocabManagerFrame->importData();
    // Some study languages in the preferences may have been added after the import
    // so we need to update the language selectors.
    updateFirstLanguageValues();
    updateTestLanguageValues();
}

void MainWindow::exportData() {
    vocabManagerFrame->exportData();
}

void MainWindow::preferences() {
    // Would it be possible not to pass the size of the array?
    PreferencesDialog dialog( this, &(control->getPreferences()) );
    dialog.showMaximized();
    int result = dialog.exec();
    if( result ) {
        // Check if different from previous value before updating.
        updateFonts();
        switchLanguage( control->getPreferences().getInterfaceLanguage() );
        //if( mainPanel->visibleWidget() == quizFrame ) {
        //    // Update the quiz.  May be tricky if the current term has no data for the current first/test languages.
        //}
        //else if( mainPanel->visibleWidget() == vocabManagerFrame )
        updateFirstLanguageValues();
        updateTestLanguageValues();
        setDigraphEnabled( control->getPreferences().isDigraphEnabled() );
        quizFrame->setButtonsHidden( control->getPreferences().areQuizButtonsHidden() );
        vocabManagerFrame->updateShownItems();
    }
}

void MainWindow::cut() {
    QWidget* widget = qApp->focusWidget();
    if( widget ) {
        if( widget->inherits( "QLineEdit" ) )
            ((QLineEdit*)widget)->cut();
        else if( widget->inherits( "QMultiLineEdit" ) )
            ((QMultiLineEdit*)widget)->cut();
        else if( mainPanel->visibleWidget() == vocabManagerFrame )
            vocabManagerFrame->cut();
    }
}

void MainWindow::copy() {
    QWidget* widget = qApp->focusWidget();
    if( widget ) {
        if( widget->inherits( "QLineEdit" ) )
            ((QLineEdit*)widget)->copy();
        else if( widget->inherits( "QMultiLineEdit" ) )
            ((QMultiLineEdit*)widget)->copy();
        else if( mainPanel->visibleWidget() == vocabManagerFrame )
                vocabManagerFrame->copy();
    }
}

void MainWindow::paste() {
    QWidget* widget = qApp->focusWidget();
    if( widget ) {
        if( widget->inherits( "QLineEdit" ) )
            ((QLineEdit*)widget)->paste();
        else if( widget->inherits( "QMultiLineEdit" ) )
            ((QMultiLineEdit*)widget)->paste();
        else if( mainPanel->visibleWidget() == vocabManagerFrame )
            vocabManagerFrame->paste();
    }
}

void MainWindow::toggleLanguageFilter() {
    setLanguageFilterEnabled( !action[ ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS ]->isOn() );
    vocabManagerFrame->updateShownItems();
}

void MainWindow::quit() {
    close( true );
}

void MainWindow::setLanguageFilterEnabled( bool isEnabled ) {
    action[ ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS ]->setOn( !isEnabled );
    control->getPreferences().setLanguageFilterEnabled( isEnabled );
}

void MainWindow::updateFirstLanguageValues() {
    updateLanguageSelector( firstLanguageComboBox );
    selectLanguage( firstLanguageComboBox, control->getPreferences().getFirstLanguage() );
}

void MainWindow::updateTestLanguageValues() {
    updateLanguageSelector( testLanguageComboBox );
    selectLanguage( testLanguageComboBox, control->getPreferences().getTestLanguage() );
}

void MainWindow::selectLanguage( QComboBox* comboBox, const QString& langCode ) {
    int itemCount = comboBox->count();
    for( int i = 0; i < itemCount; i++ ) {
        if( comboBox->text( i ) == QObject::tr( langCode ) ) {
            comboBox->setCurrentItem( i );
            return;
        }
    }
}

void MainWindow::updateLanguageSelector( QComboBox* comboBox ) {
    QValueList<QString> studyLanguages = control->getPreferences().getStudyLanguages();
    QStringList sortedLanguages;
    for( QValueList<QString>::ConstIterator it = studyLanguages.begin(); it != studyLanguages.end(); it++ ) {
        QString lang( *it );
        sortedLanguages.append( QObject::tr( lang ) );
    }
    sortedLanguages.sort(); 

    comboBox->clear();
    comboBox->insertItem( QString::null );
    for( QStringList::ConstIterator it = sortedLanguages.begin(); it != sortedLanguages.end(); it++ ) {
        QString lang( *it );
        comboBox->insertItem( lang );
    }
}

void MainWindow::setFirstLanguage( const QString& lang ) {
    if( lang == QObject::tr( control->getPreferences().getTestLanguage() ) && control->getPreferences().getTestLanguage() != QString::null )
        switchFirstAndTestLanguages();
    else
        control->getPreferences().setFirstLanguage( Util::getLanguageCode( lang ) );
    updateFonts();
    quizFrame->updateLanguageLabels();
    vocabManagerFrame->updateShownItems();
}

void MainWindow::setTestLanguage( const QString& lang ) {
    if( lang == QObject::tr( control->getPreferences().getFirstLanguage() ) && control->getPreferences().getFirstLanguage() != QString::null )
        switchFirstAndTestLanguages();
    else
        control->getPreferences().setTestLanguage( Util::getLanguageCode( lang ) );
    updateFonts();
    quizFrame->updateLanguageLabels();
    vocabManagerFrame->updateShownItems();
}

void MainWindow::switchFirstAndTestLanguages() {
    QString firstLang( control->getPreferences().getFirstLanguage() );
    QString testLang( control->getPreferences().getTestLanguage() );
    selectLanguage( firstLanguageComboBox, testLang );
    selectLanguage( testLanguageComboBox, firstLang );
    control->getPreferences().setFirstLanguage( testLang );
    control->getPreferences().setTestLanguage( firstLang );
}
