#include "SequenceDialog.h"

SequenceDialog::SequenceDialog( Preferences* prefs, QWidget* parent ) 
    : QDialog( parent, 0, true ), prefs( prefs ) {
    init();
}

SequenceDialog::~SequenceDialog() {
}

void SequenceDialog::init() {
    sequencePanel = new QHBox( this );
    sequencePanel->setSpacing( 10 );

    quizPanelWrapper = new QVGroupBox( tr( "Quiz" ), sequencePanel, "QuizPanelWrapper" );
    quizPanel = new QVBox( quizPanelWrapper );

    quizTopPanel = new QHBox( quizPanel );

    quizTopLeftPanel = new QVBox( quizTopPanel );
    quizTopLeftPanel->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Preferred ) );

    quizFirstLangPanel = new QHGroupBox( tr( "FirstLanguage" ), quizTopLeftPanel, "QuizFirstLangPanel" );
    quizFirstLangPanelWrapper = new QHBox( quizFirstLangPanel );
    quizFirstLangPanelWrapper->setSpacing( 2 );
    quizFirstLangLabel = new QLabel( tr( "Word/Expr." ), quizFirstLangPanelWrapper, "QuizFirstLangLabel" );
    quizFirstLangTermButton = new QPushButton( "a", quizFirstLangPanelWrapper, "QuizFirstLangTermButton" ); 
    quizFirstLangTermButton->installEventFilter( this );
    quizFirstLangPanelWrapper->setStretchFactor( quizFirstLangTermButton, 1 );
    quizFirstLangPanel->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );

    quizTopPanel->setStretchFactor( quizTopLeftPanel, 1 );

    quizTestLangPanel = new QHGroupBox( tr( "TestLanguage" ), quizTopLeftPanel, "QuizTestLangPanel" );
    quizTestLangPanelWrapper = new QHBox( quizTestLangPanel );
    quizTestLangPanelWrapper->setSpacing( 2 );
    quizTestLangLabelsPanel = new QVBox( quizTestLangPanelWrapper );
    quizTestLangLabelsPanel->setSpacing( 2 );
    quizTestLangButtonsPanel = new QVBox( quizTestLangPanelWrapper );
    quizTestLangButtonsPanel->setSpacing( 2 );
    quizTestLangAltLabel = new QLabel( tr( "Alt./Phon." ), quizTestLangLabelsPanel, "QuizTestLangAltLabel" );
    quizTestLangTermLabel = new QLabel( tr( "Word/Expr." ), quizTestLangLabelsPanel, "QuizTestLangTermLabel" );
    quizTestLangAltButton = new QPushButton( "b", quizTestLangButtonsPanel, "QuizTestLangAltButton" );
    quizTestLangAltButton->installEventFilter( this );
    quizTestLangTermButton = new QPushButton( "c", quizTestLangButtonsPanel, "QuizTestLangTermButton" );
    quizTestLangTermButton->installEventFilter( this );
    quizTestLangPanelWrapper->setStretchFactor( quizTestLangButtonsPanel, 1 );

    quizImagePanel = new QVGroupBox( tr( "Image" ), quizTopPanel, "QuizImagePanel" );
    quizImagePanel->setFixedWidth( 130 );
    quizImageButton = new QPushButton( "e", quizImagePanel, "QuizImageButton" );
    quizImageButton->installEventFilter( this );
    quizImageButton->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );

    quizCommentBox = new QVBox( quizPanel );
    quizCommentLabel = new QLabel( tr( "CommentLabelPanel" ), quizCommentBox );
    quizCommentButton = new QPushButton( "d", quizCommentBox, "QuizCommentButton" ); 
    quizCommentButton->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
    quizCommentButton->installEventFilter( this );
    quizCommentBox->setStretchFactor( quizCommentButton, 1 );
    quizPanel->setStretchFactor( quizCommentBox, 1 );

    sequencePanelButtons = new QVBox( sequencePanel );
    addSequenceMarkButton = new QPushButton( ">", sequencePanelButtons, "AddSequenceMarkButton" ); 
    addSequenceMarkButton->setEnabled( false );
    addSequenceMarkButton->setMinimumWidth( 50 );
    connect( addSequenceMarkButton, SIGNAL( clicked() ), this, SLOT( addSequenceMark() ) );
    addGroupMarkButton = new QPushButton( "+", sequencePanelButtons, "AddGroupMarkButton" ); 
    addGroupMarkButton->setEnabled( false );
    addGroupMarkButton->setMinimumWidth( addSequenceMarkButton->sizeHint().width() );
    connect( addGroupMarkButton, SIGNAL( clicked() ), this, SLOT( addGroupMark() ) );
    removeLastMarkButton = new QPushButton( "X", sequencePanelButtons, "RemoveLastMarkButton" ); 
    removeLastMarkButton->setEnabled( false );
    removeLastMarkButton->setMinimumWidth( addSequenceMarkButton->sizeHint().width() );
    connect( removeLastMarkButton, SIGNAL( clicked() ), this, SLOT( removeLastToken() ) );

    sequenceLinePanel = new QHBox( this );
    sequenceLinePanel->setSpacing( 10 );
    sequenceLineLabel = new QLabel( tr( "Sequence" ), sequenceLinePanel, "SequenceLineLabel" );
    sequenceLineLineEdit = new QLineEdit( sequenceLinePanel, "SequenceLineLineEdit" );
    sequenceLineLineEdit->setReadOnly( true );

    mainLayout = new QVBoxLayout( this );
    mainLayout->setSpacing( 6 );
    mainLayout->setMargin( 6 );
    mainLayout->addWidget( sequenceLinePanel );
    mainLayout->addWidget( sequencePanel, 1 );
    mainLayout->activate();

    setCaption( tr( "AddRevealingSequence" ) );
}

Sequence SequenceDialog::getSequence() const {
    return( sequence );
}

void SequenceDialog::accept() {
    if( sequenceLineLineEdit->text().isEmpty() ) {
        QMessageBox::warning( this, QObject::tr( "Warning" ), tr( "SequenceIsEmpty" ) );
        return;
    }

    QString lastChar = sequenceLineLineEdit->text().right( 1 );
    if( lastChar == " " || lastChar == "+" ) {
        QMessageBox::warning( this, QObject::tr( "Warning" ), tr( "SequenceNotTerminated" ) );
        return;
    }

    currGroup.append( items.top() );
    sequence.addGroup( currGroup );
    currGroup.clear();

    QDialog::accept();
}

bool SequenceDialog::eventFilter( QObject* obj, QEvent* evt ) {
    if( evt->type() == QEvent::MouseButtonRelease ) {
        QMouseEvent* mouseEvt = (QMouseEvent*)evt;
        if( mouseEvt->button() == LeftButton ) {
            if( obj && obj->inherits( "QPushButton" ) ) {
                QPushButton* button = (QPushButton*)obj;
                addSequenceItem( button->text() );
            }
        }
    }
    return( false );
}

void SequenceDialog::addSequenceItem( const QString& itemStr ) {
    sequenceLineLineEdit->setText( items.count() == 0 ? itemStr : sequenceLineLineEdit->text() + itemStr );
    Sequence::Item item = Sequence::stringToItem( itemStr );
    items.push( item );
    setItemButtonsEnabled( false );
    setUnionButtonsEnabled( true );
    removeLastMarkButton->setEnabled( true );
}

void SequenceDialog::addSequenceMark() {
    currGroup.append( items.top() );
    sequence.addGroup( currGroup );
    currGroup.clear();
    sequenceLineLineEdit->setText( sequenceLineLineEdit->text() + " > " );
    setItemButtonsEnabled( true );
    setUnionButtonsEnabled( false );
    removeLastMarkButton->setEnabled( true );
}

void SequenceDialog::addGroupMark() {
    currGroup.append( items.top() );
    sequenceLineLineEdit->setText( sequenceLineLineEdit->text() + "+" );
    setItemButtonsEnabled( true );
    setUnionButtonsEnabled( false );
    removeLastMarkButton->setEnabled( true );
}

void SequenceDialog::removeLastToken() {
    QString seqStr = sequenceLineLineEdit->text();
    QString lastChar = seqStr.right( 1 );
    if( lastChar == "+" ) {
        currGroup.remove( items.top() );
        setItemButtonsEnabled( false );
        setUnionButtonsEnabled( true );
        sequenceLineLineEdit->setText( seqStr.left( seqStr.length() - 1 ) );
    }
    else if( lastChar == " " ) { /* " > " */
        currGroup.remove( items.top() );
        Sequence::ItemList lastGroup = sequence.getGroupAt( sequence.getGroupCount() );
        sequence.removeLastGroup();
        currGroup = lastGroup;
        setItemButtonsEnabled( false );
        setUnionButtonsEnabled( true );
        sequenceLineLineEdit->setText( seqStr.left( seqStr.length() - 3 ) );
    }
    else { /* "a", "b", "c", "d" or "e" */
        items.pop();
        setItemButtonsEnabled( true );
        setUnionButtonsEnabled( false );
        sequenceLineLineEdit->setText( seqStr.left( seqStr.length() - 1 ) );
    }
    removeLastMarkButton->setEnabled( sequenceLineLineEdit->text().length() > 0 );
}

void SequenceDialog::setUnionButtonsEnabled( bool isEnabled ) {
    addSequenceMarkButton->setEnabled( isEnabled && areItemsLeft() );
    addGroupMarkButton->setEnabled( isEnabled && areItemsLeft() );
}

bool SequenceDialog::isItemUsed( const Sequence::Item& item ) const {
    return( items.contains( item ) );
}

bool SequenceDialog::areItemsLeft() const {
    return( !isItemUsed( Sequence::FIRST_LANG_TERM ) || !isItemUsed( Sequence::TEST_LANG_ALT ) || !isItemUsed( Sequence::TEST_LANG_TERM ) ||
        !isItemUsed( Sequence::COMMENT ) || !isItemUsed( Sequence::IMAGE ) );
}

void SequenceDialog::setItemButtonsEnabled( bool isEnabled ) {
    quizFirstLangTermButton->setEnabled( isEnabled && !isItemUsed( Sequence::FIRST_LANG_TERM ) );
    quizTestLangAltButton->setEnabled( isEnabled && !isItemUsed( Sequence::TEST_LANG_ALT ) );
    quizTestLangTermButton->setEnabled( isEnabled && !isItemUsed( Sequence::TEST_LANG_TERM ) );
    quizCommentButton->setEnabled( isEnabled && !isItemUsed( Sequence::COMMENT ) );
    quizImageButton->setEnabled( isEnabled && !isItemUsed( Sequence::IMAGE ) );
}
