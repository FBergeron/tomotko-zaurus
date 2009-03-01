#include "QuizFrame.h"
#include "icons/eye.xpm"
#include "icons/goodAns.xpm"
#include "icons/badAns.xpm"
#include "icons/editTerm.xpm"
#include "icons/maximize.xpm"

QuizFrame::QuizFrame( Controller* controller, QWidget *parent = 0, const char* name = 0 )
    : QWidget( parent, name ), controller( controller ) {
    init();
}

void QuizFrame::init() {
    Preferences& prefs = controller->getPreferences();
    QFont largeFont = prefs.getLargeFont();
    QFont mediumFont = prefs.getMediumFont();
    QFont labelsFont = prefs.getLabelsFont();
    QString firstLang( controller->getQuizFirstLanguage() );
    QString testLang( controller->getQuizTestLanguage() );

    topPanel = new QHBox( this, "TopPanel" );
    topLeftPanel = new QVBox( topPanel, "TopLeftPanel" );

    firstLangPanel = new QHGroupBox( QObject::tr( firstLang ), topLeftPanel, "FirstLangPanel" );
    testLangPanel = new QVGroupBox( QObject::tr( testLang ), topLeftPanel, "TestLangPanel" );

    controlPanel = new QWidget( this, "ControlPanel" );
    controlPanelLayout = new QHBoxLayout( controlPanel );
    controlPanelLayout->setSpacing( 2 );

    answerControlPanel = new QHBox( controlPanel, "AnswerControlPanel" );
     
    revealAllDataButton = new QPushButton( tr( "Reveal" ), answerControlPanel, "RevealAllData" );
    revealAllDataButton->setPixmap( ZPIXMAP( eye_xpm ) );
    revealAllDataButton->setEnabled( false );
    revealAllDataButton->installEventFilter( this );
    QToolTip::add( revealAllDataButton, tr( "Reveal" ) );
    connect( revealAllDataButton, SIGNAL( clicked() ), this, SLOT( reveal() ) );

    rightAnswerButton = new QPushButton( tr( "RightAnswer" ), answerControlPanel, "RightAnswer" );
    rightAnswerButton->setPixmap( ZPIXMAP( goodAns_xpm ) ); 
    rightAnswerButton->setEnabled( false );
    rightAnswerButton->installEventFilter( this );
    QToolTip::add( rightAnswerButton, tr( "RightAnswer" ) );
    connect( rightAnswerButton, SIGNAL( clicked() ), this, SLOT( rightAnswer() ) );

    wrongAnswerButton = new QPushButton( tr( "WrongAnswer" ), answerControlPanel, "WrongAnswer" );
    wrongAnswerButton->setPixmap( ZPIXMAP( badAns_xpm ) );
    wrongAnswerButton->setEnabled( false );
    wrongAnswerButton->installEventFilter( this );
    QToolTip::add( wrongAnswerButton, tr( "WrongAnswer" ) );
    connect( wrongAnswerButton, SIGNAL( clicked() ), this, SLOT( wrongAnswer() ) );

    editionButton = new QPushButton( tr( "EditWord" ), controlPanel, "EditData" );
    editionButton->setPixmap( ZPIXMAP( editTerm_xpm ) ); 
    editionButton->setMinimumWidth( 100 );
    editionButton->setEnabled( false );
    editionButton->installEventFilter( this );
    QToolTip::add( editionButton, tr( "EditWord" ) );
    connect( editionButton, SIGNAL( clicked() ), this, SLOT( editCurrentTerm() ) );

    controlPanelLayout->addWidget( answerControlPanel, 1 );
    controlPanelLayout->addWidget( editionButton, 0 );
    
    firstLangTermPanel = new QWidget( firstLangPanel, "FirstLangTermPanel" ); 
    firstLangTermPanelLayout = new QHBoxLayout( firstLangTermPanel );
    firstLangTermPanelLayout->setSpacing( 2 );
    firstLangTermLabel = new QLabel( tr( "Word/Expr." ), firstLangTermPanel, "FirstLangTermLabel" );

    firstLangTermStack = new QWidgetStack( firstLangTermPanel, "FirstLangTermStack" );
    firstLangTermLineEdit = new ScrollableLineEdit( firstLangTermStack, "FirstLangTermLineEdit" );
    firstLangTermLineEdit->setReadOnly( true );
    firstLangTermLineEdit->installEventFilter( this );
    firstLangTermButton = new QPushButton( tr( "???" ), firstLangTermStack, "FirstLangTermButton" );
    firstLangTermButton->installEventFilter( this );
    QToolTip::add( firstLangTermStack, tr( "Reveal" ) );
    connect( firstLangTermButton, SIGNAL( clicked() ), this, SLOT( revealFirstLangTerm() ) );  
    firstLangTermStack->addWidget( firstLangTermLineEdit, 0 );
    firstLangTermStack->addWidget( firstLangTermButton, 1 );
    firstLangTermStack->setMinimumSize( firstLangTermButton->sizeHint() );

    firstLangTermPanelLayout->addWidget( firstLangTermLabel, 0 );
    firstLangTermPanelLayout->addWidget( firstLangTermStack, 1 );
    
    testLangTopPanel = new QWidget( testLangPanel, "TestLangTopPanel" );
    testLangTopPanelLayout = new QHBoxLayout( testLangTopPanel );
    testLangTopPanelLayout->setSpacing( 2 );

    testLangLabelsPanel = new QVBox( testLangTopPanel, "TestLangLabelsPanel" );
    testLangLabelsPanel->setSpacing( 2 );
    testLangTermAltLabel = new QLabel( tr( "Alt./Phon." ), testLangLabelsPanel, "TestLangTermAltLabel" );
    testLangTermLabel = new QLabel( tr( "Word/Expr." ), testLangLabelsPanel, "TestLangTermLabel" );

    testLangFieldsPanel = new QVBox( testLangTopPanel, "TestLangFieldsPanel" );
    testLangFieldsPanel->setSpacing( 2 );
    testLangTermAltStack = new QWidgetStack( testLangFieldsPanel, "TestLangTermAltStack" );
    testLangTermAltLineEdit = new ScrollableLineEdit( testLangTermAltStack, "TestLangTermAltLineEdit" );
    testLangTermAltLineEdit->setReadOnly( true );
    testLangTermAltLineEdit->installEventFilter( this );
    testLangTermAltButton = new QPushButton( tr( "???" ), testLangTermAltStack, "TestLangTermAltButton" );
    testLangTermAltStack->installEventFilter( this );
    QToolTip::add( testLangTermAltButton, tr( "Reveal" ) );
    connect( testLangTermAltButton, SIGNAL( clicked() ), this, SLOT( revealAltTerm() ) );  
    testLangTermAltStack->addWidget( testLangTermAltLineEdit, 0 );
    testLangTermAltStack->addWidget( testLangTermAltButton, 1 );
    testLangTermAltStack->setMinimumSize( testLangTermAltButton->sizeHint() );

    testLangTermStack = new QWidgetStack( testLangFieldsPanel, "TestLangTermStack" );
    testLangTermLineEdit = new ScrollableLineEdit( testLangTermStack, "TestLangTermLineEdit" );
    testLangTermLineEdit->setReadOnly( true );
    testLangTermLineEdit->installEventFilter( this );
    testLangTermButton = new QPushButton( tr( "???" ), testLangTermStack, "TestLangTermButton" );
    testLangTermStack->installEventFilter( this );
    QToolTip::add( testLangTermButton, tr( "Reveal" ) );
    connect( testLangTermButton, SIGNAL( clicked() ), this, SLOT( revealTestLangTerm() ) );  

    testLangTermStack->addWidget( testLangTermLineEdit, 0 );
    testLangTermStack->addWidget( testLangTermButton, 1 );
    testLangTermStack->setMinimumSize( testLangTermButton->sizeHint() );

    testLangTopPanelLayout->addWidget( testLangLabelsPanel );
    testLangTopPanelLayout->addWidget( testLangFieldsPanel, 1 );

    commentBox = new QVBox( this, "CommentBox" );
    commentBox->setSpacing( 2 );

    commentLabelPanel = new QWidget( commentBox, "CommentLabelPanel" );
    commentLabelPanelLayout = new QHBoxLayout( commentLabelPanel );
    commentLabelPanelLayout->setSpacing( 2 );
    commentLabel = new QLabel( tr( "Examples/Comments" ), commentLabelPanel, "TestLangCommentLabel" );
    maximizeCommentButton = new QPushButton( tr( "MaximizeComment" ), commentLabelPanel, "MaximizeCommentButton" );
    maximizeCommentButton->setPixmap( ZPIXMAP( maximize_xpm ) );
    maximizeCommentButton->setToggleButton( true );
    maximizeCommentButton->setMaximumHeight( commentLabel->sizeHint().height() > 24 ? commentLabel->sizeHint().height() : 24 );
    maximizeCommentButton->setMaximumWidth( maximizeCommentButton->maximumHeight() );
    maximizeCommentButton->installEventFilter( this );
    QToolTip::add( maximizeCommentButton, tr( "MaximizeComment" ) );
    connect( maximizeCommentButton, SIGNAL( toggled( bool ) ), this, SLOT( toggleMaximizeComment( bool ) ) );
    commentLabelPanelLayout->addWidget( commentLabel );
    commentLabelPanelLayout->addStretch( 1 );
    commentLabelPanelLayout->addWidget( maximizeCommentButton );

    commentStack = new QWidgetStack( commentBox, "TestLangCommentStack" );
    commentStack->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );

    commentMultiLineEdit = new ScrollableMultiLineEdit( commentStack, "TestLangCommentMultiLineEdit" );
    commentMultiLineEdit->setWrapPolicy( QMultiLineEdit::Anywhere );
    commentMultiLineEdit->setWordWrap( QMultiLineEdit::WidgetWidth );
    commentMultiLineEdit->setReadOnly( true );
    commentMultiLineEdit->installEventFilter( this );
    commentButton = new QPushButton( tr( "???" ), commentStack, "TestLangCommentButton" );
    commentButton->installEventFilter( this );
    QToolTip::add( commentButton, tr( "Reveal" ) );
    connect( commentButton, SIGNAL( clicked() ), this, SLOT( revealComment() ) );  
    commentStack->addWidget( commentMultiLineEdit, 0 );
    commentStack->addWidget( commentButton, 1 );

    imageBox = new ImageBox( tr( "Image" ), tr( "???" ), tr( "Reveal" ), topPanel, "ImageBox" );

    topLeftPanel->setMaximumHeight( topLeftPanel->maximumHeight() );
    imageBox->setMaximumHeight( topLeftPanel->maximumHeight() );

    mainLayout = new QVBoxLayout( this );
    mainLayout->setSpacing( 2 );
    mainLayout->addWidget( topPanel );
    mainLayout->addWidget( commentBox, 1 );
    mainLayout->addWidget( controlPanel );
    mainLayout->activate();

    updateFonts();

    setButtonsHidden( prefs.areQuizButtonsHidden() );
}

QuizFrame::~QuizFrame() {
}

void QuizFrame::startQuiz() {
    setButtonsEnabled( false );
    controller->startQuiz();
    controller->prepareQuiz();
    updateLanguageLabels();
    updateFonts();

    if( !controller->isQuizInProgress() ) {
        QMessageBox::warning( this, QObject::tr( "Information" ), tr( "NoTermsMarkedForStudy" ) );
        return;
    }

    askNextTerm();
}

void QuizFrame::restartQuiz() {
    setButtonsEnabled( false );
    controller->restartQuiz();
    controller->prepareQuiz();
    updateLanguageLabels();
    updateFonts();

    if( !controller->isQuizInProgress() ) {
        QMessageBox::warning( this, QObject::tr( "Information" ), tr( "NoTermsMarkedForStudy" ) );
        return;
    }

    askNextTerm();
}

void QuizFrame::resumeQuiz() {
    controller->resumeQuiz();
    controller->prepareQuiz();
    updateLanguageLabels();
    updateFonts();
    askCurrentTerm();
}

void QuizFrame::setTerm( const Term& term ) {
    Translation firstLangTranslation = term.getTranslation( controller->getQuizFirstLanguage() );
    Translation testLangTranslation = term.getTranslation( controller->getQuizTestLanguage() );

    const QString firstLangWord = firstLangTranslation.getWord();
    const QString testLangAlt = testLangTranslation.getAlt();
    const QString testLangWord = testLangTranslation.getWord();

    QString comment;
    BilingualKey commentKey( controller->getQuizFirstLanguage(), controller->getQuizTestLanguage() );
    if( term.isCommentExists( commentKey ) )
        comment = term.getComment( commentKey );

    firstLangTermLineEdit->setText( firstLangWord );
    firstLangTermLineEdit->setCursorPosition( 0 );

    testLangTermAltLineEdit->setText( testLangAlt );
    testLangTermAltLineEdit->setCursorPosition( 0 );

    testLangTermLineEdit->setText( testLangWord );
    testLangTermLineEdit->setCursorPosition( 0 );

    commentMultiLineEdit->setText( comment );
    commentMultiLineEdit->setCursorPosition( 0, 0 );

    Folder* vocabTree = controller->getVocabTree();
    Vocabulary* vocab = vocabTree->getVocabulary( term.getVocabId() );
    if( vocab ) {
        QString absPath = controller->getResolvedImagePath( term.getImagePath(), *vocab );
        imageBox->setImage( absPath );
    }
}

void QuizFrame::concludeQuiz() {
    setButtonsEnabled( false );
    QMessageBox::information( this, QObject::tr( "Information" ), tr( "WellDone" ).arg( controller->getInitialTermCount() ) );
}

Term* QuizFrame::askCurrentTerm() {
    restoreCommentField();

    Term* currTerm = controller->getCurrentTerm();
    if( currTerm )
        askTerm( *currTerm );

    return( currTerm );
}

Term* QuizFrame::askNextTerm() {
    Term* nextTerm = NULL;
    if( controller->hasNextTerm() )
        nextTerm = controller->getNextTerm(); // Can return NULL if the term has been deleted meanwhile.
       
    if( nextTerm ) {
        restoreCommentField();
        askTerm( *nextTerm );
        return( nextTerm );
    }
    else {
        concludeQuiz();
        return( NULL );
    }
}

void QuizFrame::askTerm( const Term& term ) {
    hideAnswers();
    controller->initRevealingSequence();
    setTerm( term );
    reveal();
    setButtonsEnabled( true );
}

void QuizFrame::updateLanguageLabels() {
    firstLangPanel->setTitle( QObject::tr( controller->getQuizFirstLanguage() ) );
    testLangPanel->setTitle( QObject::tr( controller->getQuizTestLanguage() ) );
}

void QuizFrame::setButtonsHidden( bool areHidden ) {
    if( areHidden ) {
        controlPanel->hide();
        maximizeCommentButton->hide();
        buttonsHidden = true;
    }
    else {
        // If the comment field is maximized, do not show the controlPanel yet.
        // It will be shown as soon as the comment field is restored.
        if( !maximizeCommentButton->isOn() )
            controlPanel->show();
        maximizeCommentButton->show();
        buttonsHidden = false;
    }
}

bool QuizFrame::areButtonsHidden() const {
    return( buttonsHidden );
}

void QuizFrame::showEvent( QShowEvent* showEvt ) {
    QWidget::showEvent( showEvt );
    // We set the state of maximize action because it VocabularyManagerFrame may have changed it.
    action[ ACTION_MAXIMIZE ]->setOn( maximizeCommentButton->isOn() );  
    emit( quizShown() );
}

void QuizFrame::hideEvent( QHideEvent* hideEvt ) {
    QWidget::hideEvent( hideEvt );
    emit( quizHidden() );
}

// Reimplemented to prevent resizing bug.
bool QuizFrame::event( QEvent* evt ) {
    //cout << "event evt=" << evt->type() << " b=" << b << " h=" << testLangPanel->height() << " sh->h=" << testLangPanel->sizeHint().height() << endl;
    if( evt->type() == QEvent::LayoutHint ) {
        if( firstLangPanel->height() != firstLangPanel->sizeHint().height() )
            firstLangPanel->setFixedHeight( firstLangPanel->sizeHint().height() );
        if( testLangPanel->height() != testLangPanel->sizeHint().height() )
            testLangPanel->setFixedHeight( testLangPanel->sizeHint().height() );
    }
    return( QWidget::event( evt ) );
}

void QuizFrame::rightAnswer() {
    if( controller->isQuizInProgress() ) {
        controller->rightAnswer();
        askNextTerm();
    }
}

void QuizFrame::wrongAnswer() {
    if( controller->isQuizInProgress() ) {
        controller->wrongAnswer();
        askNextTerm();
    }
}

void QuizFrame::reveal() {
    if( controller->isQuizInProgress() ) {
        Sequence seq = controller->getRevealingSequence();
        int groupCount = seq.getGroupCount();
        bool hasShownSomething = false;
        int i = controller->getRevealingSequenceStep();
        int stepCount = 0;
        //cerr << "currSeq=" << seq.toHumanReadableString() << " i=" << i << endl;
        for( ; i < groupCount && !hasShownSomething; i++, stepCount++ ) {
            Sequence::ItemList group = seq.getGroupAt( i );

            int itemCount = group.count();
            for( int j = 0; j < itemCount; j++ ) {
                Sequence::Item item = group[ j ];
                //cerr << "item=" << item << endl;
                switch( item ) {

                    case Sequence::FIRST_LANG_TERM :  
                        if( !isFirstLangTermRevealed() ) {
                            revealFirstLangTerm(); 
                            hasShownSomething = true;
                        }
                        break;

                    case Sequence::TEST_LANG_ALT :
                        if( !isAltTermRevealed() ) {
                            revealAltTerm(); 
                            hasShownSomething = true;
                        }
                        break;

                    case Sequence::TEST_LANG_TERM :
                        if( !isTestLangTermRevealed() ) {
                            revealTestLangTerm(); 
                            hasShownSomething = true;
                        }
                        break;

                    case Sequence::COMMENT :          
                        if( !isCommentRevealed() ) {
                            revealComment(); 
                            hasShownSomething = true;
                        }
                        break;

                    case Sequence::IMAGE :            
                        if( imageBox->containsValidImage() && !isImageRevealed() ) {
                            revealImage(); 
                            hasShownSomething = true;
                        }
                        break;
                    
                    default:
                        break;
                }
            }

        }
        if( hasShownSomething ) {
            for( int k = 0; k < stepCount; k++ )
                controller->incrementRevealingSequenceStep();
        }
        else if( i == groupCount ) {
            if( isCommentRevealed() )
                toggleMaximizeComment( !maximizeCommentButton->isOn() );
        }

        QApplication::flushX();
    }
}

void QuizFrame::revealAll() {
    revealTestLangTerm();
    revealAltTerm();
    revealComment();
    revealImage();
    QApplication::flushX();
}

bool QuizFrame::eventFilter( QObject*, QEvent* evt ) {
    if( evt->type() == QEvent::KeyPress ) {
        QKeyEvent* keyEvt = (QKeyEvent*)evt;
        switch( keyEvt->key() ) {

            case Qt::Key_Up : 
                if( keyEvt->state() == Qt::ControlButton )
                    commentMultiLineEdit->scrollPageUp();
                else
                    commentMultiLineEdit->scrollUp();
                return( true );

            case Qt::Key_Down : 
                if( keyEvt->state() == Qt::ControlButton )
                    commentMultiLineEdit->scrollPageDown();
                else
                    commentMultiLineEdit->scrollDown();
                return( true );

            case Qt::Key_Left :  scrollLeft(); return( true );

            case Qt::Key_Right : scrollRight(); return( true );

        }
    }
    return( false );
}

void QuizFrame::scrollLeft() {
    firstLangTermLineEdit->scrollLeft();
    testLangTermLineEdit->scrollLeft();
    testLangTermAltLineEdit->scrollLeft();
}

void QuizFrame::scrollRight() {
    firstLangTermLineEdit->scrollRight();
    testLangTermLineEdit->scrollRight();
    testLangTermAltLineEdit->scrollRight();
}

void QuizFrame::updateFonts() {
    QFont largeFont( controller->getPreferences().getLargeFont() );
    QFont mediumFont( controller->getPreferences().getMediumFont() );
    QFont labelsFont( controller->getPreferences().getLabelsFont() );
    QString firstLang( controller->getQuizFirstLanguage() );
    QString testLang( controller->getQuizTestLanguage() );

    firstLangTermLabel->setFont( labelsFont );
    firstLangTermLineEdit->setFont( controller->getPreferences().getMediumFont( firstLang ) );
    firstLangTermButton->setFont( labelsFont );
    firstLangPanel->setFont( labelsFont );

    testLangTermAltLabel->setFont( labelsFont );
    testLangTermAltLineEdit->setFont( controller->getPreferences().getMediumFont( testLang ) );
    testLangTermLabel->setFont( labelsFont );

    testLangTermLineEdit->setFont( controller->getPreferences().getLargeFont( testLang ) );

    testLangPanel->setFont( labelsFont );

    commentLabel->setFont( labelsFont );
    commentMultiLineEdit->setFont( controller->getPreferences().getBestFont( firstLang, testLang ) );

    firstLangTermPanel->updateGeometry();
    firstLangTermPanel->layout()->invalidate();

    firstLangTermStack->updateGeometry();
   
    testLangLabelsPanel->updateGeometry();
    testLangLabelsPanel->layout()->invalidate();

    testLangFieldsPanel->updateGeometry();
    testLangFieldsPanel->layout()->invalidate();

    updateGeometry();
    layout()->invalidate();
}

void QuizFrame::retranslateUi() {
    updateLanguageLabels();
    firstLangTermLabel->setText( tr( "Word/Expr." ) );
    firstLangTermButton->setText( tr( "???" ) );
    testLangTermAltLabel->setText( tr( "Alt./Phon." ) );
    testLangTermLabel->setText( tr( "Word/Expr." ) );
    testLangTermAltButton->setText( tr( "???" ) );
    testLangTermButton->setText( tr( "???" ) );
    imageBox->setTitle( tr( "Image" ) );
    commentLabel->setText( tr( "Examples/Comments" ) );
    commentButton->setText( tr( "???" ) );
}

void QuizFrame::hideAnswers() {
    firstLangTermStack->raiseWidget( 1 );
    testLangTermAltStack->raiseWidget( 1 );
    testLangTermStack->raiseWidget( 1 );
    commentStack->raiseWidget( 1 );
    imageBox->hideImage();
}

void QuizFrame::setButtonsEnabled( bool isEnabled ) {
    revealAllDataButton->setEnabled( isEnabled );
    rightAnswerButton->setEnabled( isEnabled );
    wrongAnswerButton->setEnabled( isEnabled );
    editionButton->setEnabled( isEnabled );
}

void QuizFrame::revealFirstLangTerm() {
    firstLangTermStack->raiseWidget( 0 );
}

void QuizFrame::revealAltTerm() {
    testLangTermAltStack->raiseWidget( 0 );
}

void QuizFrame::revealTestLangTerm() {
    testLangTermStack->raiseWidget( 0 );
}

void QuizFrame::revealComment() {
    commentStack->raiseWidget( 0 );
}

void QuizFrame::revealImage() {
    imageBox->revealImage();
}

void QuizFrame::toggleMaximizeComment( bool isOn ) {
    if( isOn )
        maximizeCommentField();
    else
        restoreCommentField();
}

bool QuizFrame::isFirstLangTermRevealed() const {
    return( firstLangTermStack->visibleWidget() == firstLangTermStack->widget( 0 ) );
}

bool QuizFrame::isAltTermRevealed() const {
    return( testLangTermAltStack->visibleWidget() == testLangTermAltStack->widget( 0 ) );
}

bool QuizFrame::isTestLangTermRevealed() const {
    return( testLangTermStack->visibleWidget() == testLangTermStack->widget( 0 ) );
}

bool QuizFrame::isCommentRevealed() const {
    return( commentStack->visibleWidget() == commentStack->widget( 0 ) );
}

bool QuizFrame::isImageRevealed() const {
    return( imageBox->isImageRevealed() );
}

void QuizFrame::maximizeCommentField() {
    setUpdatesEnabled( false );
    action[ ACTION_MAXIMIZE ]->setOn( true );
    maximizeCommentButton->setOn( true );
    firstLangPanel->hide();
    testLangPanel->hide();
    imageBox->hide();
    controlPanel->hide();
    revealComment();
    setUpdatesEnabled( true );
}

void QuizFrame::restoreCommentField() {
    setUpdatesEnabled( false );
    action[ ACTION_MAXIMIZE ]->setOn( false );
    maximizeCommentButton->setOn( false );
    firstLangPanel->show();
    testLangPanel->show();

    if( imageBox->containsValidImage() )
        imageBox->show();

    if( !buttonsHidden )
        controlPanel->show();
    setUpdatesEnabled( true );
}

void QuizFrame::editCurrentTerm() {
    if( controller->isQuizInProgress() ) {
        Folder* vocabTree = controller->getVocabTree();
        Term* term = controller->getCurrentTerm();
        if( !term ) {
            QMessageBox::warning( this, QObject::tr( "Information" ), tr( "DissociatedWord" ) );
            return;
        }

        Vocabulary* vocab = vocabTree->getVocabulary( term->getVocabId() );
        if( vocab == NULL || !vocab->isTermExists( term->getId() ) ) {
            QMessageBox::warning( this, QObject::tr( "Information" ), tr( "DissociatedWord" ) );
            return;
        }

        TermDialog dialog( *vocab, controller, this, *term );
        dialog.showMaximized();
        int result = dialog.exec();
        if( result ) { 
            QString firstLang( controller->getQuizFirstLanguage() );
            QString testLang( controller->getQuizTestLanguage() );
            Term newTerm = dialog.getTerm();
            Translation firstLangTrans = newTerm.getTranslation( firstLang );
            Translation testLangTrans = newTerm.getTranslation( testLang );
            term->addTranslation( firstLangTrans );
            term->addTranslation( testLangTrans );
            BilingualKey commentKey( firstLang, testLang );
            term->addComment( commentKey, newTerm.getComment( commentKey ) );
            term->setImagePath( newTerm.getImagePath() );
            vocab->setModificationDate( QDateTime::currentDateTime() );
            vocab->setDirty( true );
            setTerm( newTerm );
        }
    }
}
