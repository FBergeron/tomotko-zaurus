#include "PreferencesDialog.h"

const QString PreferencesDialog::fontSizeNameList[] = { 
    QT_TR_NOOP( "VerySmall" ), QT_TR_NOOP( "Small" ), QT_TR_NOOP( "Medium" ), QT_TR_NOOP( "Large" ), QT_TR_NOOP( "VeryLarge" ) 
};

const QString PreferencesDialog::firstLanguageList[] = { 
    QString( "en" ), QString( "fr" ), QString( "es" ), QString( "ja" ) 
};

const QString PreferencesDialog::studyLanguageList[] = { 
    QString( "en" ), QString( "fr" ), QString( "es" ), QString( "ja" ), QString( "de" ), 
    QString( "ar" ), QString( "bg" ), QString( "cs" ), QString( "da" ), QString( "el" ),
    QString( "eo" ), QString( "fi" ), QString( "he" ), QString( "hi" ), QString( "it" ), 
    QString( "ko" ), QString( "la" ), QString( "no" ), QString( "nl" ), QString( "pt" ), QString( "rm" ), 
    QString( "ro" ), QString( "ru" ), QString( "sv" ), QString( "sw" ), QString( "th" ), 
    QString( "tl" ), QString( "vi" ), QString( "zh" )
};

PreferencesDialog::PreferencesDialog( QWidget* parent, Preferences* prefs ) 
    : QDialog( parent, 0, true ), prefs( prefs ), grabAccelKeyFor( NULL ), ignoreReturn( false ), keyboardAccelModified( false )  {
    init();
}

PreferencesDialog::~PreferencesDialog() {
    delete languageUpdateNotifier;
}

void PreferencesDialog::init() {
    tab = new QTabWidget( this, "Tab" );

    quizPage = new QScrollView( tab, "QuizPage" );
    quizPage->setResizePolicy( QScrollView::AutoOneFit );
    quizPageBox = new QWidget( quizPage->viewport(), "QuizPage" );
    quizPage->addChild( quizPageBox );

    quizAlgorithmsPanel = new QWidget( quizPageBox, "QuizAlgorithmsPanel" );
    quizAlgorithmsPanelLayout = new QHBoxLayout( quizAlgorithmsPanel );

    quizAlgorithmOptionsPanel = new QVButtonGroup( tr( "Algorithm" ), quizAlgorithmsPanel, "QuizAlgorithmOptionsPanel" );
    quizLengthOptionsPanel = new QVGroupBox( tr( "Length" ), quizAlgorithmsPanel, "QuizLengthOptionsPanel" );
    quizAlgorithmsPanelLayout->addWidget( quizAlgorithmOptionsPanel );
    quizAlgorithmsPanelLayout->addWidget( quizLengthOptionsPanel, 1 );

    quizCharacterImageOptionsPanel = new QHGroupBox( tr( "Images associated to characters" ), quizPageBox, "QuizCharacterImageOptions" );

    quizCharacterImageLocationLabel = new QLabel( tr( "CharacterImageLocation" ), quizCharacterImageOptionsPanel );
    quizCharacterImageLocationField = new QLineEdit( quizCharacterImageOptionsPanel );
    quizCharacterImageLocationField->setText( prefs->getQuizCharacterImageLocation() );
    quizCharacterImageLocationSetButton = new QPushButton( tr( "SetCharacterImageLocation" ), quizCharacterImageOptionsPanel );
    connect( quizCharacterImageLocationSetButton, SIGNAL( clicked() ), this, SLOT( setCharacterImageLocation() ) );
    quizCharacterImageLocationClearButton = new QPushButton( tr( "ClearCharacterImageLocation" ), quizCharacterImageOptionsPanel );
    connect( quizCharacterImageLocationClearButton, SIGNAL( clicked() ), this, SLOT( clearCharacterImageLocation() ) );
    quizCharacterImageOptionsPanel->addSpace( 10 );
    quizCharacterImageTypeCheckbox = new QCheckBox( tr( "IsCharacterImageAnimated" ), quizCharacterImageOptionsPanel );
    quizCharacterImageTypeCheckbox->setChecked( prefs->isQuizCharacterImagesAnimated() );

    revealingOptionsPanel = new QHGroupBox( tr( "RevealingOrders" ), quizPageBox, "RevealingOptionsPanel" );
  
    sequencesViewPanel = new QVBox( revealingOptionsPanel ); 
    sequencesViewPanel->setSpacing( 2 );
    sequencesView = new SmartListView( sequencesViewPanel, "SequencesView" );
    sequencesView->addColumn( QString::null );
    sequencesView->header()->hide();
    sequencesViewButtons = new QHBox( sequencesViewPanel );
    sequencesViewButtons->setSpacing( 2 );
    //sequencesViewButtonsFiller = new QWidget( sequencesViewButtons, "SequenceViewButtonsFiller" ); 
    addSequenceButton = new QPushButton( "+", sequencesViewButtons, "AddSequenceButton" );
    QToolTip::add( addSequenceButton, tr( "Add revealing sequence" ) );
    connect( addSequenceButton, SIGNAL( clicked() ), this, SLOT( addSequence() ) );
    removeSequenceButton = new QPushButton( "-", sequencesViewButtons, "RemoveSequenceButton" );
    QToolTip::add( removeSequenceButton, tr( "Remove revealing sequence" ) );
    connect( removeSequenceButton, SIGNAL( clicked() ), this, SLOT( removeSequence() ) );
    //sequencesViewButtons->setStretchFactor( sequencesViewButtonsFiller, 1 );

    initSequences();
    connect( sequencesView, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( updateUi() ) );

    revealingOptionsPanel->addSpace( 6 );

    sequencesLabelBox = new QVGroupBox( tr( "QuizWindow" ), revealingOptionsPanel, "SequencesLabelBox" );
    sequencesLabel = new QLabel( sequencesLabelBox );
    sequencesLabel->setPixmap( Resource::loadPixmap( "SequenceMapLandscape" ) );

    sequencesViewPanel->setMaximumHeight( sequencesLabelBox->sizeHint().height() );

    quizAlgoOriginalRadioButton = new QRadioButton( tr( "OriginalQuizAlgorithm" ), quizAlgorithmOptionsPanel );
    quizAlgoSuperMemo2RadioButton = new QRadioButton( tr( "SuperMemo2Algorithm" ), quizAlgorithmOptionsPanel );
    if( prefs->getQuizAlgorithm() == Preferences::ORIGINAL )
        quizAlgoOriginalRadioButton->setChecked( true );
    else if( prefs->getQuizAlgorithm() == Preferences::SUPERMEMO2 )
        quizAlgoSuperMemo2RadioButton->setChecked( true );

    quizLengthSlider = new QSlider( 0, OriginalQuiz::poolCount - 1, 1, 1, QSlider::Horizontal, quizLengthOptionsPanel, "QuizLengthSlider" );
    quizLengthLabelsPanel = new QHBox( quizLengthOptionsPanel, "QuizLengthLabelsPanel" );
    quizLengthShortestLabel = new QLabel( tr( "QuizLengthShortest" ), quizLengthLabelsPanel, "QuizLengthShortestLabel" );
    quizLengthMediumLabel = new QLabel( tr( "QuizLengthMedium" ), quizLengthLabelsPanel, "QuizLengthMediumLabel" );
    quizLengthMediumLabel->setAlignment( QLabel::AlignCenter );
    quizLengthLongestLabel = new QLabel( tr( "QuizLengthLongest" ), quizLengthLabelsPanel, "QuizLengthLongestLabel" );
    quizLengthLongestLabel->setAlignment( QLabel::AlignRight );
    
    quizLengthSlider->setValue( prefs->getQuizLength() ); 

    quizPageFiller = new QWidget( quizPageBox, "QuizPageFiller" );

    quizLayout = new QVBoxLayout( quizPageBox );
    quizLayout->addWidget( quizAlgorithmsPanel );
    quizLayout->addWidget( quizCharacterImageOptionsPanel );
    quizLayout->addWidget( revealingOptionsPanel );
    quizLayout->addWidget( quizPageFiller, 1 );

    fontPage = new QWidget( tab, "FontPage" );

    //fontsPanel = new QVGroupBox( tr( "Fonts" ), fontPage, "FontsPanel" );
    fontsPanel = new QVBox( fontPage, "FontsPanel" );
    fontsPanel->setSpacing( 2 );

    labelsFontPanel = new QHBox( fontsPanel, "LabelsFontPanel" );
    labelsFontPanel->setSpacing( 2 );
    labelsFontLabel = new QLabel( tr( "LabelsFont" ), labelsFontPanel, "LabelsFontLabel" );
    labelsFontFamilyComboBox = new QComboBox( labelsFontPanel, "LabelsFontFamilyComboBox" );
    labelsFontSizeComboBox = new QComboBox( labelsFontPanel, "LabelsFontSizeComboBox" );
    resetDefaultLabelsFontButton = new QPushButton( tr( "ResetDefaultValue" ), labelsFontPanel, "ResetDefaultLabelsFontButton" );
    connect( resetDefaultLabelsFontButton, SIGNAL( clicked() ), this, SLOT( resetDefaultLabelsFont() ) );

    initFontFamilyValues( labelsFontFamilyComboBox );
    selectFontFamily( labelsFontFamilyComboBox, prefs->getLabelsFontFamily() );
    initFontSizeValues( labelsFontSizeComboBox );
    selectFontSize( labelsFontSizeComboBox, prefs->getLabelsFontSizeModifier() );

    fontPanel = new QHBox( fontsPanel, "FontPanel" );
    fontPanel->setSpacing( 2 );
    fontLabel = new QLabel( tr( "FieldsFont" ), fontPanel, "FontLabel" );
    fontFamilyComboBox = new QComboBox( fontPanel, "FontFamilyComboBox" );
    fontSizeComboBox = new QComboBox( fontPanel, "FontSizeComboBox" );
    resetDefaultFontButton = new QPushButton( tr( "ResetDefaultValue" ), fontPanel, "ResetDefaultFontButton" );
    connect( resetDefaultFontButton, SIGNAL( clicked() ), this, SLOT( resetDefaultFont() ) );

    initFontFamilyValues( fontFamilyComboBox );
    selectFontFamily( fontFamilyComboBox, prefs->getFontFamily() );
    initFontSizeValues( fontSizeComboBox );
    selectFontSize( fontSizeComboBox, prefs->getFontSizeModifier() );

    fontOverridesPanel = new QVGroupBox( tr( "LanguageFontOverrides" ), fontsPanel, "LanguageFontsPanel" );
    fontOverridesScrollView = new QScrollView( fontOverridesPanel, "FontOverridesScrollView" );
    fontOverridesScrollView->setResizePolicy( QScrollView::AutoOneFit );
    fontOverridesBox = new QWidget( fontOverridesScrollView->viewport() );
    fontOverridesBoxLayout = new QVBoxLayout( fontOverridesBox );
    fontOverridesScrollView->addChild( fontOverridesBox );
    initFontOverrides();

    fontPageFiller = new QWidget( fontPage, "FontPageFiller" );

    fontPageLayout = new QVBoxLayout( fontPage );
    fontPageLayout->setSpacing( 2 );
    fontPageLayout->addWidget( fontsPanel );
    fontPageLayout->addWidget( fontPageFiller, 1 );

    interfacePage = new QWidget( tab, "InterfacePage" );

    miscInterfaceOptionsPanel = new QGrid( 2, interfacePage, "MiscInterfaceOptionsPanel" );

    digraphPanel = new QVGroupBox( tr( "BuiltInSupportForAccents" ), miscInterfaceOptionsPanel, "DigraphPanel" );
    digraphCheckBox = new QCheckBox( tr( "DigraphesEnabled" ), digraphPanel, "DigraphCheckBox" );
    digraphCheckBox->setChecked( prefs->isDigraphEnabled() );

    hideQuizButtonPanel = new QVGroupBox( tr( "QuizOptions" ), miscInterfaceOptionsPanel, "HideQuizButtonPanel" );
    hideQuizButtonCheckBox = new QCheckBox( tr( "hideQuizButtons" ), hideQuizButtonPanel, "HideQuizButtonCheckBox" );
    hideQuizButtonCheckBox->setChecked( prefs->areQuizButtonsHidden() );

    showAltTextInTermListPanel = new QVGroupBox( tr( "GlossaryManagerOptions" ), miscInterfaceOptionsPanel, "ShowAltTextInTermListPanel" );
    showAltTextInTermListCheckBox = new QCheckBox( tr( "ShowAltInTermList" ), showAltTextInTermListPanel, "ShowAltInTermListCheckBox" );
    showAltTextInTermListCheckBox->setChecked( prefs->isAltInTermListShown() );

    keyboardAccelPanel = new QVGroupBox( tr( "Keyboard Accelerators" ), interfacePage, "KeyboardAccelPanel" );
    keyboardAccelListView = new SmartListView( keyboardAccelPanel, "KeyboardAccelListView" );
    keyboardAccelListView->addColumn( tr( "Action" ), 280 );
    keyboardAccelListView->addColumn( tr( "Key" ), 140 );
    keyboardAccelListView->setSorting( -1 );
    keyboardAccelListView->setAllColumnsShowFocus( true );
    keyboardAccelListView->setStretchColumn( 0 );
    int actionCount = sizeof( action ) / sizeof( QAction* );
    for( int i = actionCount - 1; i >= 0; i-- ) {
        KeyActionListViewItem* actionItem = new KeyActionListViewItem( keyboardAccelListView, action[ i ], (Action)i );
        actionItem->setText( 0, action[ i ]->text() );
        actionItem->setPixmap( 0, action[ i ]->iconSet().pixmap() );
        keyboardAccelListView->insertItem( actionItem );
    }
    connect( keyboardAccelListView, SIGNAL( returnPressed( QListViewItem* ) ), this, SLOT( keyActionClicked( QListViewItem* ) ) );
    connect( keyboardAccelListView, SIGNAL( currentChanged( QListViewItem* ) ), this, SLOT( cancelSetAccelKey() ) );
    connect( tab, SIGNAL( selected( const QString& ) ), this, SLOT( cancelSetAccelKey() ) );

    keyboardAccelButtonPanel = new QWidget( keyboardAccelPanel, "KeyboardAccelButtonPanel" );
    keyboardAccelButtonPanelFiller = new QWidget( keyboardAccelButtonPanel, "KeyboardAccelButtonPanelFiller" );  
    clearAccelKeyButton = new QPushButton( tr( "Clear key" ), keyboardAccelButtonPanel );
    setAccelKeyButton = new QPushButton( tr( "Set key" ), keyboardAccelButtonPanel );
    resetAccelKeyButton = new QPushButton( tr( "Reset key" ), keyboardAccelButtonPanel );
    
    keyboardAccelButtonPanelLayout = new QHBoxLayout( keyboardAccelButtonPanel );
    keyboardAccelButtonPanelLayout->setSpacing( 2 );
    keyboardAccelButtonPanelLayout->addWidget( keyboardAccelButtonPanelFiller, 1 );
    keyboardAccelButtonPanelLayout->addWidget( clearAccelKeyButton );
    keyboardAccelButtonPanelLayout->addWidget( setAccelKeyButton );
    keyboardAccelButtonPanelLayout->addWidget( resetAccelKeyButton );

    connect( clearAccelKeyButton, SIGNAL( clicked() ), this, SLOT( clearAccelKey() ) );
    connect( setAccelKeyButton, SIGNAL( clicked() ), this, SLOT( setAccelKey() ) );
    connect( resetAccelKeyButton, SIGNAL( clicked() ), this, SLOT( resetAccelKey() ) );

    interfacePageFiller = new QWidget( interfacePage, "InterfacePageFiller" );

    interfacePageLayout = new QVBoxLayout( interfacePage );
    interfacePageLayout->setSpacing( 2 );
    interfacePageLayout->addWidget( miscInterfaceOptionsPanel );
    interfacePageLayout->addWidget( keyboardAccelPanel );
    interfacePageLayout->addWidget( interfacePageFiller, 1 );

    languagePage = new QWidget( tab, "LanguagePage" );

    languagesPanel = new QVBox( languagePage, "LanguagesPanel" );
    languagesPanel->setSpacing( 2 );

    studyLanguagesListView = new SmartListView( languagesPanel, "StudyLanguagesListView" );
    studyLanguagesListView->setAllColumnsShowFocus( true );
    studyLanguagesListView->addColumn( tr( "StudyLanguages" ) );
    studyLanguagesListView->setStretchColumn( 0 );
    languagesPanel->setStretchFactor( studyLanguagesListView, 1 );
   
    languageUpdateNotifier = new LanguageUpdateNotifier();
    connect( languageUpdateNotifier, SIGNAL( languageAdded( const QString& ) ), this, SLOT( addFontOverride( const QString& ) ) );
    connect( languageUpdateNotifier, SIGNAL( languageRemoved( const QString& ) ), this, SLOT( removeFontOverride( const QString& ) ) );

    initStudyLanguageValues();

    languagesRightPanel = new QWidget( languagesPanel, "LanguagesRightPanel" );
    languagesRightPanelLayout = new QVBoxLayout( languagesRightPanel );

    languagesRightPanelFiller = new QWidget( languagesRightPanel, "LanguageRightPanelFiller" );

    languagesRightPanelLayout->addWidget( languagesRightPanelFiller, 1 );

    languageLayout = new QVBoxLayout( languagePage );
    languageLayout->setSpacing( 2 );
    languageLayout->addWidget( languagesPanel, 1 );

    tab->addTab( quizPage, tr( "Quiz" ) );
    tab->addTab( languagePage, tr( "Languages" ) );
    tab->addTab( fontPage, tr( "Fonts" ) );
    tab->addTab( interfacePage, tr( "Interface" ) );

    mainLayout = new QVBoxLayout( this );
    mainLayout->addWidget( tab, 1 );

    setCaption( tr( "Preferences..." ) );

    updateUi();
}

void PreferencesDialog::initFontFamilyValues( QComboBox* comboBox, bool withEmptyEntry = false ) const {
    QFontDatabase fontDatabase;
    QStringList families = fontDatabase.families( false );
    if( withEmptyEntry ) 
        comboBox->insertItem( QString::null );
    comboBox->insertStringList( families );
}

void PreferencesDialog::initFontSizeValues( QComboBox* comboBox, bool withEmptyEntry = false ) const {
    int fontSizeNameListLength = sizeof( fontSizeNameList ) / sizeof( QString );
    if( withEmptyEntry ) 
        comboBox->insertItem( QString::null );
    for( int i = 0; i < fontSizeNameListLength; i++ )
        comboBox->insertItem( tr( fontSizeNameList[ i ] ) );
}

void PreferencesDialog::initFontOverrides() {
    QValueList<QString> lang( prefs->getStudyLanguages() );
    int i = 0;
    for( QValueList<QString>::ConstIterator it = lang.begin(); it != lang.end(); it++, i++ ) {
        QString language( *it );
        QHBox* fontOverrideBox = new QHBox( fontOverridesBox );
        fontOverrideBox->setSpacing( 2 );
        fontOverrideBox->setMargin( 2 );
        QLabel* fontOverrideLabel = new QLabel( QObject::tr( language ), fontOverrideBox );
        QComboBox* fontOverrideFamilyComboBox = new QComboBox( fontOverrideBox );
        initFontFamilyValues( fontOverrideFamilyComboBox, true );
        if( prefs->isFontOverrideFamilyDefined( language ) ) 
            selectFontFamily( fontOverrideFamilyComboBox, prefs->getFontOverrideFamily( language ) );
        QComboBox* fontOverrideSizeComboBox = new QComboBox( fontOverrideBox );
        initFontSizeValues( fontOverrideSizeComboBox, true );
        if( prefs->isFontOverrideSizeDefined( language ) ) 
            selectFontSize( fontOverrideSizeComboBox, prefs->getFontOverrideSize( language ), true );
        fontOverrideBox->setStretchFactor( fontOverrideLabel, 1 );

        fontOverrideBoxes.append( fontOverrideBox );
        fontOverrideLabels.append( fontOverrideLabel );
        fontOverrideFamilyComboBoxes.append( fontOverrideFamilyComboBox );
        fontOverrideSizeComboBoxes.append( fontOverrideSizeComboBox );

        fontOverridesBoxLayout->addWidget( fontOverrideBox, 0 );
    }
    fontOverridesBoxFiller = new QWidget( fontOverridesBox );
    fontOverridesBoxLayout->addWidget( fontOverridesBoxFiller, 1 );
}

void PreferencesDialog::initSequences() {
    int seqCount = prefs->getRevealingSequenceCount();
    for( int i = 0; i < seqCount; i++ ) {
        Sequence seq = prefs->getRevealingSequenceAt( i );
        SequenceListItem* item = new SequenceListItem( sequencesView, seq.toHumanReadableString(), seq ); 
        item->setOn( seq.isEnabled() );
    }
}

void PreferencesDialog::initStudyLanguageValues() const {
    int studyLanguageListLength = sizeof( studyLanguageList ) / sizeof( QString );
    for( int i = 0; i < studyLanguageListLength; i++ ) {
        StudyLanguageItem* lang = new StudyLanguageItem( studyLanguagesListView, languageUpdateNotifier );
        lang->setText( 0, QObject::tr( studyLanguageList[ i ] ) );
        bool isStudied( prefs->isStudyLanguage( studyLanguageList[ i ] ) );
        lang->setOn( isStudied );
        studyLanguagesListView->insertItem( lang );
    }
}

void PreferencesDialog::selectFontFamily( QComboBox* comboBox, const QString& fontFamily ) {
    for( int i = 0; i < comboBox->count(); i++ ) {
        if( comboBox->text( i ) == fontFamily )
            comboBox->setCurrentItem( i );
    }
}

void PreferencesDialog::selectFontSize( QComboBox* comboBox, int fontSizeModifier, bool withEmptyEntry = false ) {
    int fontSizeNameListLength = sizeof( fontSizeNameList ) / sizeof( QString );
    int sizeIndex = fontSizeModifier + ( fontSizeNameListLength - 1 ) / 2;
    if( withEmptyEntry )
        sizeIndex++;
    comboBox->setCurrentItem( sizeIndex );
}

void PreferencesDialog::selectLanguage( QComboBox* comboBox, const QString& langCode ) {
    int itemCount = comboBox->count();
    for( int i = 0; i < itemCount; i++ ) {
        if( comboBox->text( i ) == QObject::tr( langCode ) ) {
            comboBox->setCurrentItem( i );
            return;
        }
    }
}

void PreferencesDialog::accept() {
    cancelSetAccelKey();

    if( !isRevealingSequenceSelectionValid() ) {
        tab->showPage( quizPage ); 
        QMessageBox::warning( this, QObject::tr( "Warning" ), tr( "RevealingOrderMandatory" ) );
        return;
    }

    if( !isStudyLanguageSelectionValid() ) {
        tab->showPage( languagePage ); 
        QMessageBox::warning( this, QObject::tr( "Warning" ), tr( "StudyLanguagesMandatory" ) );
        return;
    }

    if( quizAlgoOriginalRadioButton->isChecked() )
        prefs->setQuizAlgorithm( Preferences::ORIGINAL );
    else if( quizAlgoSuperMemo2RadioButton->isChecked() )
        prefs->setQuizAlgorithm( Preferences::SUPERMEMO2 );

    prefs->setQuizCharacterImageLocation( quizCharacterImageLocationField->text() );
    prefs->setQuizCharacterImagesAnimated( quizCharacterImageTypeCheckbox->isOn() );
    prefs->setQuizLength( quizLengthSlider->value() );

    prefs->clearRevealingSequences();
    for( SequenceListItem* item = (SequenceListItem*)sequencesView->firstChild(); item; item = (SequenceListItem*)item->nextSibling() )
        prefs->addRevealingSequence( item->getSequence() );

    prefs->setLabelsFontFamily( labelsFontFamilyComboBox->text( labelsFontFamilyComboBox->currentItem() ) );
    int fontSizeNameListLength = sizeof( fontSizeNameList ) / sizeof( QString );
    int labelsFontSizeModifier =  labelsFontSizeComboBox->currentItem() - ( fontSizeNameListLength - 1 ) / 2;
    prefs->setLabelsFontSizeModifier( labelsFontSizeModifier );

    prefs->setFontFamily( fontFamilyComboBox->text( fontFamilyComboBox->currentItem() ) );
    int fontSizeModifier =  fontSizeComboBox->currentItem() - ( fontSizeNameListLength - 1 ) / 2;
    prefs->setFontSizeModifier( fontSizeModifier );

    prefs->clearFontOverrideFamilies();
    prefs->clearFontOverrideSizes();
    int fontOverrideCount = fontOverrideLabels.count();
    for( int i = 0; i < fontOverrideCount; i++ ) {
        const QString& language = Util::getLanguageCode( fontOverrideLabels.at( i )->text() );
        if( fontOverrideFamilyComboBoxes.at( i )->currentItem() > 0 )
            prefs->setFontOverrideFamily( language, fontOverrideFamilyComboBoxes.at( i )->currentText() );
        if( fontOverrideSizeComboBoxes.at( i )->currentItem() > 0 ) {
            int fontSizeModifier =  ( fontOverrideSizeComboBoxes.at( i )->currentItem() - 1 ) - ( fontSizeNameListLength - 1 ) / 2;
            prefs->setFontOverrideSize( language, fontSizeModifier );
        }
    }

    bool firstLanguageExists = false;
    bool testLanguageExists = false;
    prefs->clearStudyLanguages();
    for( QCheckListItem* item = (QCheckListItem*)studyLanguagesListView->firstChild(); item; item = (QCheckListItem*)item->nextSibling() ) {
        if( item->isOn() ) {
            QString langCode( Util::getLanguageCode( item->text() ) );
            prefs->addStudyLanguage( langCode );
            if( !firstLanguageExists )
                firstLanguageExists = ( prefs->getFirstLanguage() == langCode );
            if( !testLanguageExists )
                testLanguageExists = (prefs->getTestLanguage() == langCode );
        }
    }
    if( !firstLanguageExists )
        prefs->setFirstLanguage( QString::null );
    if( !testLanguageExists )
        prefs->setTestLanguage( QString::null );

    prefs->setDigraphEnabled( digraphCheckBox->isOn() );
    prefs->setQuizButtonsHidden( hideQuizButtonCheckBox->isOn() );
    prefs->setAltInTermListShown( showAltTextInTermListCheckBox->isOn() );

    if( keyboardAccelModified ) {
        for( KeyActionListViewItem* item = (KeyActionListViewItem*)keyboardAccelListView->firstChild(); item ; 
            item = (KeyActionListViewItem*)item->nextSibling() ) {
            QAction *action = item->getAction();
            action->setAccel( item->getKey() );
            prefs->setAccelerator( item->getActionIndex(), item->getKey() );
        }
    }

    QDialog::accept();
}

void PreferencesDialog::updateUi() {
    removeSequenceButton->setEnabled( sequencesView->currentItem() ); 
}

bool PreferencesDialog::isRevealingSequenceSelectionValid() const {
    for( SequenceListItem* item = (SequenceListItem*)sequencesView->firstChild(); item; item = (SequenceListItem*)item->nextSibling() ) {
        if( item->isOn() )
            return( true );
    }

    return( false );
}

bool PreferencesDialog::isRevealingSequenceDefined( const QString& seqStr ) const {
    for( SequenceListItem* item = (SequenceListItem*)sequencesView->firstChild(); item; item = (SequenceListItem*)item->nextSibling() ) {
        if( item->getSequence().toHumanReadableString() == seqStr )
            return( true );
    }

    return( false );
}

bool PreferencesDialog::isStudyLanguageSelectionValid() const {
    int checkedLangCount = 0;
    for( QCheckListItem* item = (QCheckListItem*)studyLanguagesListView->firstChild(); item; item = (QCheckListItem*)item->nextSibling() ) {
        if( item->isOn() )
            checkedLangCount++;
    }
    return( checkedLangCount >= 2 );
}

void PreferencesDialog::resizeEvent( QResizeEvent* evt ) {
    QString pictFilename( "SequenceMap" + (QString)( evt->size().width() > evt->size().height() ? "Landscape" : "Portrait" ) );
    sequencesLabel->setPixmap( Resource::loadPixmap( pictFilename ) );
    sequencesViewPanel->setMaximumHeight( sequencesLabelBox->sizeHint().height() );
    QDialog::resizeEvent( evt );
}

void PreferencesDialog::resetDefaultLabelsFont() {
    selectFontFamily( labelsFontFamilyComboBox, prefs->getDefaultLabelsFontFamily() );
    selectFontSize( labelsFontSizeComboBox, prefs->getDefaultLabelsFontSizeModifier() );
}

void PreferencesDialog::resetDefaultFont() {
    selectFontFamily( fontFamilyComboBox, prefs->getDefaultFontFamily() );
    selectFontSize( fontSizeComboBox, prefs->getDefaultFontSizeModifier() );
}

void PreferencesDialog::addFontOverride( const QString& language ) {
    // Do nothing if the font override already exists.
    int fontOverrideCount = fontOverrideLabels.count();
    for( int i = 0; i < fontOverrideCount; i++ ) {
        if( fontOverrideLabels.at( i )->text() == QObject::tr( language ) )
            return;
    }
    
    QHBox* fontOverrideBox = new QHBox( fontOverridesBox );
    fontOverrideBox->setSpacing( 2 );
    fontOverrideBox->setMargin( 2 );
    QLabel* fontOverrideLabel = new QLabel( QObject::tr( language ), fontOverrideBox );
    QComboBox* fontOverrideFamilyComboBox = new QComboBox( fontOverrideBox );
    initFontFamilyValues( fontOverrideFamilyComboBox, true );
    if( prefs->isFontOverrideFamilyDefined( language ) ) 
        selectFontFamily( fontOverrideFamilyComboBox, prefs->getFontOverrideFamily( language ) );
    QComboBox* fontOverrideSizeComboBox = new QComboBox( fontOverrideBox );
    initFontSizeValues( fontOverrideSizeComboBox, true );
    if( prefs->isFontOverrideSizeDefined( language ) ) 
        selectFontSize( fontOverrideSizeComboBox, prefs->getFontOverrideSize( language ), true );
    fontOverrideBox->setStretchFactor( fontOverrideLabel, 1 );

    fontOverrideBoxes.append( fontOverrideBox );
    fontOverrideLabels.append( fontOverrideLabel );
    fontOverrideFamilyComboBoxes.append( fontOverrideFamilyComboBox );
    fontOverrideSizeComboBoxes.append( fontOverrideSizeComboBox );

    int indexOfFiller = fontOverridesBoxLayout->findWidget( fontOverridesBoxFiller );
    fontOverridesBoxLayout->insertWidget( indexOfFiller, fontOverrideBox ); 
}

void PreferencesDialog::removeFontOverride( const QString& language ) {
    int fontOverrideCount = fontOverrideLabels.count();
    for( int i = 0; i < fontOverrideCount; i++ ) {
        if( fontOverrideLabels.at( i )->text() == QObject::tr( language ) ) {
            QHBox* fontOverrideBox = fontOverrideBoxes.at( i );

            fontOverrideLabels.remove( i );
            fontOverrideFamilyComboBoxes.remove( i );
            fontOverrideSizeComboBoxes.remove( i );
            fontOverrideBoxes.remove( i );
            
            fontOverridesBox->removeChild( fontOverrideBox );

            break;
        }
    }
}

void PreferencesDialog::setAccelKey() {
    KeyActionListViewItem* item = (KeyActionListViewItem*)keyboardAccelListView->currentItem();
    if( item == NULL ) 
        return;
    item->setText( 1, tr( "<press key>" ) ); 
    grabAccelKeyFor = item;
    grabKeyboard();  
}

void PreferencesDialog::clearAccelKey() {
    cancelSetAccelKey();

    KeyActionListViewItem* item = (KeyActionListViewItem*)keyboardAccelListView->currentItem();
    if( item == NULL ) 
        return;
    item->setKey( 0 );
    keyboardAccelModified = true;
}

void PreferencesDialog::resetAccelKey() {
    cancelSetAccelKey();

    KeyActionListViewItem* item = (KeyActionListViewItem*)keyboardAccelListView->currentItem();
    if( item == NULL ) 
        return;
    item->setKey( prefs->getDefaultAccelerator( item->getActionIndex() ) );
    keyboardAccelModified = true;
}

void PreferencesDialog::cancelSetAccelKey() {
    if( grabAccelKeyFor != NULL ) {
        KeyActionListViewItem* ka = (KeyActionListViewItem*)grabAccelKeyFor;
        ka->updateText();
        grabAccelKeyFor = NULL;
        releaseKeyboard();
    }
}

void PreferencesDialog::keyActionClicked( QListViewItem* ) {
    ignoreReturn = true;
    setAccelKey();
}

void PreferencesDialog::keyPressEvent( QKeyEvent *evt ) {
    if( grabAccelKeyFor != NULL ) {
        switch (evt->key()) {
            case Key_Shift:
            case Key_Control:
            case Key_Meta:
            case Key_Alt:
            case Key_CapsLock:
            case Key_NumLock:
            case Key_ScrollLock:
            case Key_F22:
                evt->ignore();
                break;
            default:
                if( ignoreReturn && evt->key() == Key_Return ) {
                    evt->ignore();
                    break;
                }
                //      printf( "key: %x modif: %x\n", evt->key(), evt->state() );

                KeyActionListViewItem *ka = (KeyActionListViewItem*)grabAccelKeyFor;

                int keyCode = evt->key();
                if( (evt->state() & ShiftButton) != 0 )
                    keyCode |= SHIFT;
                if( (evt->state() & AltButton) != 0 )
                    keyCode |= ALT;
                if( (evt->state() & ControlButton) != 0 )
                    keyCode |= CTRL;      

                //    printf( "keycode: %x\n", keyCode );
                releaseKeyboard();

                // Check for duplicate key codes
                bool cancelBinding = false;
                for( KeyActionListViewItem *item = (KeyActionListViewItem*)keyboardAccelListView->firstChild(); item != NULL; 
                    item = (KeyActionListViewItem*)item->nextSibling() ) {
                    if( item == ka ) 
                        continue;

                    if( item->getKey() == keyCode ) {
                        int sel = QMessageBox::warning( this, tr( "Duplicate key binding" ),
                        tr( "Key '" ) + Util::describeKey( keyCode ) + tr( "'\nis currently assigned to\n'" ) + item->getAction()->text() +
                            tr( "'.\nOverwrite the current assignment?" ), QMessageBox::No, QMessageBox::Yes );
                        if( sel == QMessageBox::Yes ) 
                            item->setKey( 0 );
                        else {
                            cancelBinding = true;
                            break;
                        }
                    }
                }

                if( cancelBinding ) 
                    ka->updateText();
                else {
                    ka->setKey( keyCode );
                    keyboardAccelModified = true;
                }

                grabAccelKeyFor = NULL;
                evt->accept();
                break;
        }
    }
    ignoreReturn = false;

    QDialog::keyPressEvent( evt );
}

void PreferencesDialog::setCharacterImageLocation() {
    QDir dir = QPEApplication::documentDir();
    if( !quizCharacterImageLocationField->text().isEmpty() ) {
        QFileInfo fileInfo( quizCharacterImageLocationField->text() );
        if( fileInfo.isDir() )
            dir.setPath( fileInfo.filePath() );
    }

    ZFileDialog dialog( tr( "SetCharacterImageLocation..." ), dir.path(), ZFileDialog::Directory, true, this ); 
    QStringList allowedExtensions = QStringList::split( QString( "," ), QString( ".gif,.png" ) );
    dialog.setFilters( allowedExtensions );

    int result = dialog.exec();
    if( result ) 
        quizCharacterImageLocationField->setText( dialog.dir()->path() );

}

void PreferencesDialog::clearCharacterImageLocation() {
    quizCharacterImageLocationField->clear();
}

void PreferencesDialog::addSequence() {
    SequenceDialog dialog( this );
    dialog.setFont( prefs->getLabelsFont() ); 
    dialog.showMaximized();
    int result = dialog.exec();
    if( result ) {
        Sequence sequence = dialog.getSequence();
        // Just add new sequence.  Ignore duplicates.
        if( !isRevealingSequenceDefined( sequence.toHumanReadableString() ) ) {
            SequenceListItem* item = new SequenceListItem( sequencesView, sequence.toHumanReadableString(), sequence ); 
            item->setOn( true );
        }
    }
}

void PreferencesDialog::removeSequence() {
    QListViewItem* currSeqItem = sequencesView->currentItem();
    if( currSeqItem ) {
        delete( currSeqItem );
        updateUi();
    }
}
