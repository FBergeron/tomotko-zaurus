#include "CharacterDialog.h"

CharacterDialog::CharacterDialog( QWidget* parent, const QString& imageFile ) 
    : QDialog( parent, 0, true ), imageFormat( QString::null ) {

    image = new QLabel( this );
    //image->setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Expanding ) );
    image->setAlignment( AlignHCenter | AlignVCenter );
    //image->setScaledContents( true );

    QFileInfo info( imageFile );
    if( info.exists() ) {
        imageFormat = QPixmap::imageFormat( imageFile );
        if( imageFormat == "GIF" || imageFormat == "PNG" ) {
            imageAbsolutePath = imageFile;
            if( imageFormat == "GIF" ) {
                const QMovie& movie( imageFile );
                image->setMovie( movie );
            }
            else if( imageFormat == "PNG" ) {
                QPixmap pixmap( imageFile );
                image->setPixmap( pixmap );
                //resizeImage();
            }
        }
    }

    mainLayout = new QVBoxLayout( this );
    mainLayout->setSpacing( 2 );
    mainLayout->addWidget( image );
    mainLayout->activate();

    //setCaption( tr( "CharacterDialogCaption" ) );
}

CharacterDialog::~CharacterDialog() {
}

//void CharacterDialog::init() {
//    tab = new QTabWidget( this, "Tab" );
//
//    quizPage = new QWidget( tab, "QuizPage" );
//
//    quizAlgorithmsPanel = new QWidget( quizPage, "QuizAlgorithmsPanel" );
//    quizAlgorithmsPanelLayout = new QHBoxLayout( quizAlgorithmsPanel );
//
//    quizAlgorithmOptionsPanel = new QVButtonGroup( tr( "Algorithm" ), quizAlgorithmsPanel, "QuizAlgorithmOptionsPanel" );
//    quizLengthOptionsPanel = new QVGroupBox( tr( "Length" ), quizAlgorithmsPanel, "QuizLengthOptionsPanel" );
//    quizAlgorithmsPanelLayout->addWidget( quizAlgorithmOptionsPanel );
//    quizAlgorithmsPanelLayout->addWidget( quizLengthOptionsPanel, 1 );
//
//    revealingOptionsPanel = new QHGroupBox( tr( "RevealingOrders" ), quizPage, "RevealingOptionsPanel" );
//  
//    sequencesViewPanel = new QVBox( revealingOptionsPanel ); 
//    sequencesViewPanel->setSpacing( 2 );
//    sequencesView = new SmartListView( sequencesViewPanel, "SequencesView" );
//    sequencesView->addColumn( QString::null );
//    sequencesView->header()->hide();
//    sequencesViewButtons = new QHBox( sequencesViewPanel );
//    sequencesViewButtons->setSpacing( 2 );
//    //sequencesViewButtonsFiller = new QWidget( sequencesViewButtons, "SequenceViewButtonsFiller" ); 
//    addSequenceButton = new QPushButton( "+", sequencesViewButtons, "AddSequenceButton" );
//    QToolTip::add( addSequenceButton, tr( "Add revealing sequence" ) );
//    connect( addSequenceButton, SIGNAL( clicked() ), this, SLOT( addSequence() ) );
//    removeSequenceButton = new QPushButton( "-", sequencesViewButtons, "RemoveSequenceButton" );
//    QToolTip::add( removeSequenceButton, tr( "Remove revealing sequence" ) );
//    connect( removeSequenceButton, SIGNAL( clicked() ), this, SLOT( removeSequence() ) );
//    //sequencesViewButtons->setStretchFactor( sequencesViewButtonsFiller, 1 );
//
//    initSequences();
//    connect( sequencesView, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( updateUi() ) );
//
//    revealingOptionsPanel->addSpace( 6 );
//
//    sequencesLabelBox = new QVGroupBox( tr( "QuizWindow" ), revealingOptionsPanel, "SequencesLabelBox" );
//    sequencesLabel = new QLabel( sequencesLabelBox );
//    sequencesLabel->setPixmap( Resource::loadPixmap( "SequenceMapLandscape" ) );
//
//    sequencesViewPanel->setMaximumHeight( sequencesLabelBox->sizeHint().height() );
//
//    quizAlgoOriginalRadioButton = new QRadioButton( tr( "OriginalQuizAlgorithm" ), quizAlgorithmOptionsPanel );
//    quizAlgoSuperMemo2RadioButton = new QRadioButton( tr( "SuperMemo2Algorithm" ), quizAlgorithmOptionsPanel );
//    if( prefs->getQuizAlgorithm() == Preferences::ORIGINAL )
//        quizAlgoOriginalRadioButton->setChecked( true );
//    else if( prefs->getQuizAlgorithm() == Preferences::SUPERMEMO2 )
//        quizAlgoSuperMemo2RadioButton->setChecked( true );
//
//    quizLengthSlider = new QSlider( 0, OriginalQuiz::poolCount - 1, 1, 1, QSlider::Horizontal, quizLengthOptionsPanel, "QuizLengthSlider" );
//    quizLengthLabelsPanel = new QHBox( quizLengthOptionsPanel, "QuizLengthLabelsPanel" );
//    quizLengthShortestLabel = new QLabel( tr( "QuizLengthShortest" ), quizLengthLabelsPanel, "QuizLengthShortestLabel" );
//    quizLengthMediumLabel = new QLabel( tr( "QuizLengthMedium" ), quizLengthLabelsPanel, "QuizLengthMediumLabel" );
//    quizLengthMediumLabel->setAlignment( QLabel::AlignCenter );
//    quizLengthLongestLabel = new QLabel( tr( "QuizLengthLongest" ), quizLengthLabelsPanel, "QuizLengthLongestLabel" );
//    quizLengthLongestLabel->setAlignment( QLabel::AlignRight );
//    
//    quizLengthSlider->setValue( prefs->getQuizLength() ); 
//
//    quizPageFiller = new QWidget( quizPage, "QuizPageFiller" );
//
//    quizLayout = new QVBoxLayout( quizPage );
//    quizLayout->addWidget( quizAlgorithmsPanel );
//    quizLayout->addWidget( revealingOptionsPanel );
//    quizLayout->addWidget( quizPageFiller, 1 );
//
//    fontPage = new QWidget( tab, "FontPage" );
//
//    //fontsPanel = new QVGroupBox( tr( "Fonts" ), fontPage, "FontsPanel" );
//    fontsPanel = new QVBox( fontPage, "FontsPanel" );
//    fontsPanel->setSpacing( 2 );
//
//    labelsFontPanel = new QHBox( fontsPanel, "LabelsFontPanel" );
//    labelsFontPanel->setSpacing( 2 );
//    labelsFontLabel = new QLabel( tr( "LabelsFont" ), labelsFontPanel, "LabelsFontLabel" );
//    labelsFontFamilyComboBox = new QComboBox( labelsFontPanel, "LabelsFontFamilyComboBox" );
//    labelsFontSizeComboBox = new QComboBox( labelsFontPanel, "LabelsFontSizeComboBox" );
//    resetDefaultLabelsFontButton = new QPushButton( tr( "ResetDefaultValue" ), labelsFontPanel, "ResetDefaultLabelsFontButton" );
//    connect( resetDefaultLabelsFontButton, SIGNAL( clicked() ), this, SLOT( resetDefaultLabelsFont() ) );
//
//    initFontFamilyValues( labelsFontFamilyComboBox );
//    selectFontFamily( labelsFontFamilyComboBox, prefs->getLabelsFontFamily() );
//    initFontSizeValues( labelsFontSizeComboBox );
//    selectFontSize( labelsFontSizeComboBox, prefs->getLabelsFontSizeModifier() );
//
//    fontPanel = new QHBox( fontsPanel, "FontPanel" );
//    fontPanel->setSpacing( 2 );
//    fontLabel = new QLabel( tr( "FieldsFont" ), fontPanel, "FontLabel" );
//    fontFamilyComboBox = new QComboBox( fontPanel, "FontFamilyComboBox" );
//    fontSizeComboBox = new QComboBox( fontPanel, "FontSizeComboBox" );
//    resetDefaultFontButton = new QPushButton( tr( "ResetDefaultValue" ), fontPanel, "ResetDefaultFontButton" );
//    connect( resetDefaultFontButton, SIGNAL( clicked() ), this, SLOT( resetDefaultFont() ) );
//
//    initFontFamilyValues( fontFamilyComboBox );
//    selectFontFamily( fontFamilyComboBox, prefs->getFontFamily() );
//    initFontSizeValues( fontSizeComboBox );
//    selectFontSize( fontSizeComboBox, prefs->getFontSizeModifier() );
//
//    fontOverridesPanel = new QVGroupBox( tr( "LanguageFontOverrides" ), fontsPanel, "LanguageFontsPanel" );
//    fontOverridesScrollView = new QScrollView( fontOverridesPanel, "FontOverridesScrollView" );
//    fontOverridesScrollView->setResizePolicy( QScrollView::AutoOneFit );
//    fontOverridesBox = new QWidget( fontOverridesScrollView->viewport() );
//    fontOverridesBoxLayout = new QVBoxLayout( fontOverridesBox );
//    fontOverridesScrollView->addChild( fontOverridesBox );
//    initFontOverrides();
//
//    fontPageFiller = new QWidget( fontPage, "FontPageFiller" );
//
//    fontPageLayout = new QVBoxLayout( fontPage );
//    fontPageLayout->setSpacing( 2 );
//    fontPageLayout->addWidget( fontsPanel );
//    fontPageLayout->addWidget( fontPageFiller, 1 );
//
//    interfacePage = new QWidget( tab, "InterfacePage" );
//
//    miscInterfaceOptionsPanel = new QGrid( 2, interfacePage, "MiscInterfaceOptionsPanel" );
//
//    digraphPanel = new QVGroupBox( tr( "BuiltInSupportForAccents" ), miscInterfaceOptionsPanel, "DigraphPanel" );
//    digraphCheckBox = new QCheckBox( tr( "DigraphesEnabled" ), digraphPanel, "DigraphCheckBox" );
//    digraphCheckBox->setChecked( prefs->isDigraphEnabled() );
//
//    hideQuizButtonPanel = new QVGroupBox( tr( "QuizOptions" ), miscInterfaceOptionsPanel, "HideQuizButtonPanel" );
//    hideQuizButtonCheckBox = new QCheckBox( tr( "hideQuizButtons" ), hideQuizButtonPanel, "HideQuizButtonCheckBox" );
//    hideQuizButtonCheckBox->setChecked( prefs->areQuizButtonsHidden() );
//
//    showAltTextInTermListPanel = new QVGroupBox( tr( "GlossaryManagerOptions" ), miscInterfaceOptionsPanel, "ShowAltTextInTermListPanel" );
//    showAltTextInTermListCheckBox = new QCheckBox( tr( "ShowAltInTermList" ), showAltTextInTermListPanel, "ShowAltInTermListCheckBox" );
//    showAltTextInTermListCheckBox->setChecked( prefs->isAltInTermListShown() );
//
//    keyboardAccelPanel = new QVGroupBox( tr( "Keyboard Accelerators" ), interfacePage, "KeyboardAccelPanel" );
//    keyboardAccelListView = new SmartListView( keyboardAccelPanel, "KeyboardAccelListView" );
//    keyboardAccelListView->addColumn( tr( "Action" ), 280 );
//    keyboardAccelListView->addColumn( tr( "Key" ), 140 );
//    keyboardAccelListView->setSorting( -1 );
//    keyboardAccelListView->setAllColumnsShowFocus( true );
//    keyboardAccelListView->setStretchColumn( 0 );
//    int actionCount = sizeof( action ) / sizeof( QAction* );
//    for( int i = actionCount - 1; i >= 0; i-- ) {
//        KeyActionListViewItem* actionItem = new KeyActionListViewItem( keyboardAccelListView, action[ i ], (Action)i );
//        actionItem->setText( 0, action[ i ]->text() );
//        actionItem->setPixmap( 0, action[ i ]->iconSet().pixmap() );
//        keyboardAccelListView->insertItem( actionItem );
//    }
//    connect( keyboardAccelListView, SIGNAL( returnPressed( QListViewItem* ) ), this, SLOT( keyActionClicked( QListViewItem* ) ) );
//    connect( keyboardAccelListView, SIGNAL( currentChanged( QListViewItem* ) ), this, SLOT( cancelSetAccelKey() ) );
//    connect( tab, SIGNAL( selected( const QString& ) ), this, SLOT( cancelSetAccelKey() ) );
//
//    keyboardAccelButtonPanel = new QWidget( keyboardAccelPanel, "KeyboardAccelButtonPanel" );
//    keyboardAccelButtonPanelFiller = new QWidget( keyboardAccelButtonPanel, "KeyboardAccelButtonPanelFiller" );  
//    clearAccelKeyButton = new QPushButton( tr( "Clear key" ), keyboardAccelButtonPanel );
//    setAccelKeyButton = new QPushButton( tr( "Set key" ), keyboardAccelButtonPanel );
//    resetAccelKeyButton = new QPushButton( tr( "Reset key" ), keyboardAccelButtonPanel );
//    
//    keyboardAccelButtonPanelLayout = new QHBoxLayout( keyboardAccelButtonPanel );
//    keyboardAccelButtonPanelLayout->setSpacing( 2 );
//    keyboardAccelButtonPanelLayout->addWidget( keyboardAccelButtonPanelFiller, 1 );
//    keyboardAccelButtonPanelLayout->addWidget( clearAccelKeyButton );
//    keyboardAccelButtonPanelLayout->addWidget( setAccelKeyButton );
//    keyboardAccelButtonPanelLayout->addWidget( resetAccelKeyButton );
//
//    connect( clearAccelKeyButton, SIGNAL( clicked() ), this, SLOT( clearAccelKey() ) );
//    connect( setAccelKeyButton, SIGNAL( clicked() ), this, SLOT( setAccelKey() ) );
//    connect( resetAccelKeyButton, SIGNAL( clicked() ), this, SLOT( resetAccelKey() ) );
//
//    interfacePageFiller = new QWidget( interfacePage, "InterfacePageFiller" );
//
//    interfacePageLayout = new QVBoxLayout( interfacePage );
//    interfacePageLayout->setSpacing( 2 );
//    interfacePageLayout->addWidget( miscInterfaceOptionsPanel );
//    interfacePageLayout->addWidget( keyboardAccelPanel );
//    interfacePageLayout->addWidget( interfacePageFiller, 1 );
//
//    languagePage = new QWidget( tab, "LanguagePage" );
//
//    languagesPanel = new QVBox( languagePage, "LanguagesPanel" );
//    languagesPanel->setSpacing( 2 );
//
//    studyLanguagesListView = new SmartListView( languagesPanel, "StudyLanguagesListView" );
//    studyLanguagesListView->setAllColumnsShowFocus( true );
//    studyLanguagesListView->addColumn( tr( "StudyLanguages" ) );
//    studyLanguagesListView->setStretchColumn( 0 );
//    languagesPanel->setStretchFactor( studyLanguagesListView, 1 );
//   
//    languageUpdateNotifier = new LanguageUpdateNotifier();
//    connect( languageUpdateNotifier, SIGNAL( languageAdded( const QString& ) ), this, SLOT( addFontOverride( const QString& ) ) );
//    connect( languageUpdateNotifier, SIGNAL( languageRemoved( const QString& ) ), this, SLOT( removeFontOverride( const QString& ) ) );
//
//    initStudyLanguageValues();
//
//    languagesRightPanel = new QWidget( languagesPanel, "LanguagesRightPanel" );
//    languagesRightPanelLayout = new QVBoxLayout( languagesRightPanel );
//
//    languagesRightPanelFiller = new QWidget( languagesRightPanel, "LanguageRightPanelFiller" );
//
//    languagesRightPanelLayout->addWidget( languagesRightPanelFiller, 1 );
//
//    languagePageFiller = new QWidget( languagePage, "LanguagePageFiller" );
//
//    languageLayout = new QVBoxLayout( languagePage );
//    languageLayout->setSpacing( 2 );
//    languageLayout->addWidget( languagesPanel );
//    languageLayout->addWidget( languagePageFiller, 1 );
//    tab->addTab( quizPage, tr( "Quiz" ) );
//    tab->addTab( languagePage, tr( "Languages" ) );
//    tab->addTab( fontPage, tr( "Fonts" ) );
//    tab->addTab( interfacePage, tr( "Interface" ) );
//
//    mainLayout = new QVBoxLayout( this );
//    mainLayout->addWidget( tab, 1 );
//
//    setCaption( tr( "Preferences..." ) );
//
//    updateUi();
//}

//void CharacterDialog::accept() {
//    cancelSetAccelKey();
//
//    if( !isRevealingSequenceSelectionValid() ) {
//        tab->showPage( quizPage ); 
//        QMessageBox::warning( this, QObject::tr( "Warning" ), tr( "RevealingOrderMandatory" ) );
//        return;
//    }
//
//    if( !isStudyLanguageSelectionValid() ) {
//        tab->showPage( languagePage ); 
//        QMessageBox::warning( this, QObject::tr( "Warning" ), tr( "StudyLanguagesMandatory" ) );
//        return;
//    }
//
//    if( quizAlgoOriginalRadioButton->isChecked() )
//        prefs->setQuizAlgorithm( Preferences::ORIGINAL );
//    else if( quizAlgoSuperMemo2RadioButton->isChecked() )
//        prefs->setQuizAlgorithm( Preferences::SUPERMEMO2 );
//
//    prefs->setQuizLength( quizLengthSlider->value() );
//
//    prefs->clearRevealingSequences();
//    for( SequenceListItem* item = (SequenceListItem*)sequencesView->firstChild(); item; item = (SequenceListItem*)item->nextSibling() )
//        prefs->addRevealingSequence( item->getSequence() );
//
//    prefs->setLabelsFontFamily( labelsFontFamilyComboBox->text( labelsFontFamilyComboBox->currentItem() ) );
//    int fontSizeNameListLength = sizeof( fontSizeNameList ) / sizeof( QString );
//    int labelsFontSizeModifier =  labelsFontSizeComboBox->currentItem() - ( fontSizeNameListLength - 1 ) / 2;
//    prefs->setLabelsFontSizeModifier( labelsFontSizeModifier );
//
//    prefs->setFontFamily( fontFamilyComboBox->text( fontFamilyComboBox->currentItem() ) );
//    int fontSizeModifier =  fontSizeComboBox->currentItem() - ( fontSizeNameListLength - 1 ) / 2;
//    prefs->setFontSizeModifier( fontSizeModifier );
//
//    prefs->clearFontOverrideFamilies();
//    prefs->clearFontOverrideSizes();
//    int fontOverrideCount = fontOverrideLabels.count();
//    for( int i = 0; i < fontOverrideCount; i++ ) {
//        const QString& language = Util::getLanguageCode( fontOverrideLabels.at( i )->text() );
//        if( fontOverrideFamilyComboBoxes.at( i )->currentItem() > 0 )
//            prefs->setFontOverrideFamily( language, fontOverrideFamilyComboBoxes.at( i )->currentText() );
//        if( fontOverrideSizeComboBoxes.at( i )->currentItem() > 0 ) {
//            int fontSizeModifier =  ( fontOverrideSizeComboBoxes.at( i )->currentItem() - 1 ) - ( fontSizeNameListLength - 1 ) / 2;
//            prefs->setFontOverrideSize( language, fontSizeModifier );
//        }
//    }
//
//    bool firstLanguageExists = false;
//    bool testLanguageExists = false;
//    prefs->clearStudyLanguages();
//    for( QCheckListItem* item = (QCheckListItem*)studyLanguagesListView->firstChild(); item; item = (QCheckListItem*)item->nextSibling() ) {
//        if( item->isOn() ) {
//            QString langCode( Util::getLanguageCode( item->text() ) );
//            prefs->addStudyLanguage( langCode );
//            if( !firstLanguageExists )
//                firstLanguageExists = ( prefs->getFirstLanguage() == langCode );
//            if( !testLanguageExists )
//                testLanguageExists = (prefs->getTestLanguage() == langCode );
//        }
//    }
//    if( !firstLanguageExists )
//        prefs->setFirstLanguage( QString::null );
//    if( !testLanguageExists )
//        prefs->setTestLanguage( QString::null );
//
//    prefs->setDigraphEnabled( digraphCheckBox->isOn() );
//    prefs->setQuizButtonsHidden( hideQuizButtonCheckBox->isOn() );
//    prefs->setAltInTermListShown( showAltTextInTermListCheckBox->isOn() );
//
//    if( keyboardAccelModified ) {
//        for( KeyActionListViewItem* item = (KeyActionListViewItem*)keyboardAccelListView->firstChild(); item ; 
//            item = (KeyActionListViewItem*)item->nextSibling() ) {
//            QAction *action = item->getAction();
//            action->setAccel( item->getKey() );
//            prefs->setAccelerator( item->getActionIndex(), item->getKey() );
//        }
//    }
//
//    QDialog::accept();
//}

//void CharacterDialog::updateUi() {
//    removeSequenceButton->setEnabled( sequencesView->currentItem() ); 
//}

//void CharacterDialog::resizeEvent( QResizeEvent* evt ) {
//    QString pictFilename( "SequenceMap" + (QString)( evt->size().width() > evt->size().height() ? "Landscape" : "Portrait" ) );
//    sequencesLabel->setPixmap( Resource::loadPixmap( pictFilename ) );
//    sequencesViewPanel->setMaximumHeight( sequencesLabelBox->sizeHint().height() );
//    QDialog::resizeEvent( evt );
//}

//void CharacterDialog::keyActionClicked( QListViewItem* ) {
//    ignoreReturn = true;
//    setAccelKey();
//}
//
//void CharacterDialog::keyPressEvent( QKeyEvent *evt ) {
//    if( grabAccelKeyFor != NULL ) {
//        switch (evt->key()) {
//            case Key_Shift:
//            case Key_Control:
//            case Key_Meta:
//            case Key_Alt:
//            case Key_CapsLock:
//            case Key_NumLock:
//            case Key_ScrollLock:
//            case Key_F22:
//                evt->ignore();
//                break;
//            default:
//                if( ignoreReturn && evt->key() == Key_Return ) {
//                    evt->ignore();
//                    break;
//                }
//                //      printf( "key: %x modif: %x\n", evt->key(), evt->state() );
//
//                KeyActionListViewItem *ka = (KeyActionListViewItem*)grabAccelKeyFor;
//
//                int keyCode = evt->key();
//                if( (evt->state() & ShiftButton) != 0 )
//                    keyCode |= SHIFT;
//                if( (evt->state() & AltButton) != 0 )
//                    keyCode |= ALT;
//                if( (evt->state() & ControlButton) != 0 )
//                    keyCode |= CTRL;      
//
//                //    printf( "keycode: %x\n", keyCode );
//                releaseKeyboard();
//
//                // Check for duplicate key codes
//                bool cancelBinding = false;
//                for( KeyActionListViewItem *item = (KeyActionListViewItem*)keyboardAccelListView->firstChild(); item != NULL; 
//                    item = (KeyActionListViewItem*)item->nextSibling() ) {
//                    if( item == ka ) 
//                        continue;
//
//                    if( item->getKey() == keyCode ) {
//                        int sel = QMessageBox::warning( this, tr( "Duplicate key binding" ),
//                        tr( "Key '" ) + Util::describeKey( keyCode ) + tr( "'\nis currently assigned to\n'" ) + item->getAction()->text() +
//                            tr( "'.\nOverwrite the current assignment?" ), QMessageBox::No, QMessageBox::Yes );
//                        if( sel == QMessageBox::Yes ) 
//                            item->setKey( 0 );
//                        else {
//                            cancelBinding = true;
//                            break;
//                        }
//                    }
//                }
//
//                if( cancelBinding ) 
//                    ka->updateText();
//                else {
//                    ka->setKey( keyCode );
//                    keyboardAccelModified = true;
//                }
//
//                grabAccelKeyFor = NULL;
//                evt->accept();
//                break;
//        }
//    }
//    ignoreReturn = false;
//
//    QDialog::keyPressEvent( evt );
//}
