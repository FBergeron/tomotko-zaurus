#include "TermDialog.h"
#include "icons/editcut.xpm"
#include "icons/editcopy.xpm"
#include "icons/editpaste.xpm"
#include "icons/void.xpm"

TermDialog::TermDialog( Vocabulary& vocab, Controller* controller, QWidget* parent ) 
    : QDialog( parent, 0, true ), vocab( vocab ), controller( controller ), editedTerm( new Term( 0, vocab.getUid(), Util::createUuid() ) ) {
    init();
}

TermDialog::TermDialog( Vocabulary& vocab, Controller* controller, QWidget* parent, const Term& term ) 
    : QDialog( parent, 0, true ), vocab( vocab ), controller( controller ), editedTerm( new Term( term ) ) {
    init();
}

void TermDialog::init() {
    Preferences& prefs = controller->getPreferences();

    QString firstLang( prefs.getFirstLanguage() );
    QString testLang( prefs.getTestLanguage() );
    bool isDigraphEnabled( prefs.isDigraphEnabled() );

    menuBar = new QMenuBar( this );

    QPopupMenu* edition = new QPopupMenu( this );
    menuBar->insertItem( QObject::tr( "Edition" ), edition );

    cutAction = new QAction( QObject::tr( "Cut" ), ZPIXMAP( editcut_xpm ), QString::null, 0, this, 0 );
    connect( cutAction, SIGNAL( activated() ), this, SLOT( cut() ) );
    cutAction->addTo( edition );

    copyAction = new QAction( QObject::tr( "Copy" ), ZPIXMAP( editcopy_xpm ), QString::null, 0, this, 0 );
    connect( copyAction, SIGNAL( activated() ), this, SLOT( copy() ) );
    copyAction->addTo( edition );

    pasteAction = new QAction( QObject::tr( "Paste" ), ZPIXMAP( editpaste_xpm ), QString::null, 0, this, 0 );
    connect( pasteAction, SIGNAL( activated() ), this, SLOT( paste() ) );
    pasteAction->addTo( edition );

    topPanel = new QHBox( this, "TopPanel" ); 
    topLeftPanel = new QVBox( topPanel, "TopLeftPanel" );

    firstLangPanel = new QHGroupBox( QObject::tr( firstLang ), topLeftPanel, "FirstLangPanel" );
    testLangPanel = new QVGroupBox( QObject::tr( testLang ), topLeftPanel, "TestLangPanel" );

    firstLangTermPanel = new QHBox( firstLangPanel, "FirstLangTermPanel" ); 
    firstLangTermLabel = new QLabel( tr( "Word/Expr." ), firstLangTermPanel, "FirstLangTermLabel" );
    firstLangTermLineEdit = new DigraphLineEdit( firstLangTermPanel, "FirstLangTermLineEdit" );
    firstLangTermLineEdit->setFont( prefs.getMediumFont( firstLang ) );
    firstLangTermLineEdit->setDigraphEnabled( isDigraphEnabled );
    testLangTopPanel = new QWidget( testLangPanel, "TestLangTopPanel" );
    testLangTopPanelLayout = new QHBoxLayout( testLangTopPanel );

    testLangLabelsPanel = new QVBox( testLangTopPanel, "TestLangLabelsPanel" );
    testLangTermAltLabel = new QLabel( tr( "Alt./Phon." ), testLangLabelsPanel, "TestLangTermAltLabel" );
    testLangTermLabel = new QLabel( tr( "Word/Expr." ), testLangLabelsPanel, "TestLangTermLabel" );

    testLangFieldsPanel = new QVBox( testLangTopPanel, "TestLangFieldsPanel" );
    testLangTermAltLineEdit = new DigraphLineEdit( testLangFieldsPanel, "TestLangTermAltLineEdit" );
    testLangTermAltLineEdit->setFont( prefs.getMediumFont( testLang ) );
    testLangTermAltLineEdit->setDigraphEnabled( isDigraphEnabled );
    testLangTermLineEdit = new DigraphLineEdit( testLangFieldsPanel, "TestLangTermLineEdit" );
    testLangTermLineEdit->setFont( prefs.getLargeFont( testLang ) );
    testLangTermLineEdit->setDigraphEnabled( isDigraphEnabled );
   
    testLangTopPanelLayout->addWidget( testLangLabelsPanel );
    testLangTopPanelLayout->addWidget( testLangFieldsPanel, 1 );

    commentLabel = new QLabel( tr( "Examples/Comments" ), this, "TestLangCommentLabel" );
    commentMultiLineEdit = new DigraphMultiLineEdit( this, "TestLangCommentMultiLineEdit" );
    commentMultiLineEdit->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
    commentMultiLineEdit->setWrapPolicy( QMultiLineEdit::Anywhere );
    commentMultiLineEdit->setWordWrap( QMultiLineEdit::WidgetWidth );
    commentMultiLineEdit->setFont( prefs.getBestFont( firstLang, testLang ) );
    commentMultiLineEdit->setDigraphEnabled( isDigraphEnabled );

    imageBox = new QVGroupBox( tr( "Image" ), topPanel, "ImageBox" );

    imagePanel = new QWidget( imageBox, "ImagePanel" );
    imagePanelLayout = new QVBoxLayout( imagePanel );
    imagePanelLayout->setSpacing( 2 );
    imageWrapper = new QVBox( imagePanel, "ImageWrapper" );
    image = new QLabel( imageWrapper, "Image" );
    image->setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Expanding ) );
    image->setAlignment( AlignHCenter | AlignVCenter );
    image->setScaledContents( true );

    imageBox->setMaximumHeight( topLeftPanel->sizeHint().height() );

    imageButtonsPanel = new QHBox( imagePanel, "ImageButtonsPanel" );
    setImageButton = new QPushButton( tr( "setImage" ), imageButtonsPanel, "SetImageButton" );
    QToolTip::add( setImageButton, tr( "setImageTooltip" ) );
    connect( setImageButton, SIGNAL( clicked() ), this, SLOT( setImage() ) );
    clearImageButton = new QPushButton( tr( "clearImage" ), imageButtonsPanel, "clearImageButton" );
    QToolTip::add( clearImageButton, tr( "clearImageTooltip" ) );
    connect( clearImageButton, SIGNAL( clicked() ), this, SLOT( clearImage() ) );
    imagePanelLayout->addWidget( imageWrapper, 1 );
    imagePanelLayout->addWidget( imageButtonsPanel );

    mainLayout = new QVBoxLayout( this );
    mainLayout->setSpacing( 2 );
    mainLayout->setMenuBar( menuBar );
    mainLayout->addWidget( topPanel );
    mainLayout->addWidget( commentLabel );
    mainLayout->addWidget( commentMultiLineEdit, 1 );
    mainLayout->activate();

    setCaption( tr( "EditTerm" ) );

    updateUi();
}

TermDialog::~TermDialog() {
}

void TermDialog::show() {
    QDialog::show();
    resizeImage();
}

void TermDialog::updateModel() {
    if( !editedTerm->isTranslationExists( controller->getPreferences().getFirstLanguage() ) )
        editedTerm->addTranslation( controller->getPreferences().getFirstLanguage() );

    Translation& firstLangTranslation = editedTerm->getTranslation( controller->getPreferences().getFirstLanguage() );
    firstLangTranslation.setWord( firstLangTermLineEdit->text() );

    if( !editedTerm->isTranslationExists( controller->getPreferences().getTestLanguage() ) )
        editedTerm->addTranslation( controller->getPreferences().getTestLanguage() );

    Translation& testLangTranslation = editedTerm->getTranslation( controller->getPreferences().getTestLanguage() );
    testLangTranslation.setWord( testLangTermLineEdit->text() );
    testLangTranslation.setAlt( testLangTermAltLineEdit->text() );

    BilingualKey commentKey( controller->getPreferences().getFirstLanguage(), controller->getPreferences().getTestLanguage() );
    editedTerm->addComment( commentKey, commentMultiLineEdit->text() );

    // If the path refers to an image in toMOTko's vocabulary, we use a relative path instead.
    QString vocabLocation = controller->getApplicationDirName() + "/" + vocab.getParent()->getPath() +
        "/v-" + QString::number( vocab.getId() ) + "/";
    QString imagePath = tempImagePath.left( vocabLocation.length() ) == vocabLocation ? 
            tempImagePath.right( tempImagePath.length() - vocabLocation.length() ) : tempImagePath;
    editedTerm->setImagePath( imagePath );
}

void TermDialog::cut() {
    QWidget* widget = qApp->focusWidget();
    if( widget != NULL ) {
        const char* className = widget->className();
        if( strcmp( className, "DigraphLineEdit" ) == 0 )
            ((DigraphLineEdit*)widget)->cut();
        else if( strcmp( className, "DigraphMultiLineEdit" ) == 0 )
            ((DigraphMultiLineEdit*)widget)->cut();
    }
}

void TermDialog::copy() {
    QWidget* widget = qApp->focusWidget();
    if( widget != NULL ) {
        const char* className = widget->className();
        if( strcmp( className, "DigraphLineEdit" ) == 0 )
            ((DigraphLineEdit*)widget)->copy();
        else if( strcmp( className, "DigraphMultiLineEdit" ) == 0 )
            ((DigraphMultiLineEdit*)widget)->copy();
    }
}

void TermDialog::paste() {
    QWidget* widget = qApp->focusWidget();
    if( widget != NULL ) {
        const char* className = widget->className();
        if( strcmp( className, "DigraphLineEdit" ) == 0 )
            ((DigraphLineEdit*)widget)->paste();
        else if( strcmp( className, "DigraphMultiLineEdit" ) == 0 )
            ((DigraphMultiLineEdit*)widget)->paste();
    }
}

void TermDialog::setImage() {
    QDir dir = QPEApplication::documentDir();
    if( tempImagePath )
        dir = QFileInfo( tempImagePath ).dir();

    ZFileDialog dialog( tr( "SetImage..." ), dir.path(), ZFileDialog::ExistingFile, true, this );
    QStringList allowedExtensions = QStringList::split( QString( "," ), QString( ".gif,.png" ) );
    dialog.setFilters( allowedExtensions );

    int result = dialog.exec();
    if( result && !dialog.selectedFile().isEmpty() ) {
        QString fileExtension = dialog.selectedFile().right( 4 );
        if( allowedExtensions.contains( fileExtension ) )
            initImage( dialog.selectedFile() );
    }
}

void TermDialog::clearImage() {
    image->setPixmap( ZPIXMAP( void_xpm ) );
    tempImagePath = QString::null;
    imageFormat = QString::null;
}

void TermDialog::initImage( const QString& imagePath ) {
    clearImage();
    if( !imagePath.isNull() ) {
        QFileInfo info( imagePath );
        if( info.exists() ) {
            imageFormat = QPixmap::imageFormat( imagePath );
            if( imageFormat == "GIF" || imageFormat == "PNG" ) {
                tempImagePath = imagePath;
                if( imageFormat == "GIF" ) {
                    const QMovie& movie( imagePath );
                    image->setMovie( movie );
                }
                else if( imageFormat == "PNG" ) {
                    QPixmap pixmap( imagePath );
                    image->setPixmap( pixmap );
                    resizeImage();
                }
            }
        }
    }
}

void TermDialog::resizeImage() const {
    if( !tempImagePath.isNull() ) {
        if( imageFormat == "GIF" ) {
            // Nothing to do for this format.  The default behavior works fine.
        }
        else if( imageFormat == "PNG" ) {
            int proportionalWidth = image->height() * image->pixmap()->width() / image->pixmap()->height();
            image->setMaximumWidth( proportionalWidth );
        }
    }
}

const Term& TermDialog::getTerm() {
    updateModel();
    return( *editedTerm );
}

void TermDialog::updateUi() {
    if( editedTerm ) {
        if( editedTerm->isTranslationExists( controller->getPreferences().getFirstLanguage() ) ) {
            Translation& firstLangTranslation = editedTerm->getTranslation( controller->getPreferences().getFirstLanguage() );
            firstLangTermLineEdit->setText( firstLangTranslation.getWord() );
            firstLangTermLineEdit->setCursorPosition( 0 );
        }

        if( editedTerm->isTranslationExists( controller->getPreferences().getTestLanguage() ) ) {
            Translation& testLangTranslation = editedTerm->getTranslation( controller->getPreferences().getTestLanguage() );
            testLangTermLineEdit->setText( testLangTranslation.getWord() );
            testLangTermLineEdit->setCursorPosition( 0 );
            testLangTermAltLineEdit->setText( testLangTranslation.getAlt() );
            testLangTermAltLineEdit->setCursorPosition( 0 );
            BilingualKey commentKey( controller->getPreferences().getFirstLanguage(), controller->getPreferences().getTestLanguage() );
            if( editedTerm->isCommentExists( commentKey ) ) {
                commentMultiLineEdit->setText( editedTerm->getComment( commentKey ) );
                commentMultiLineEdit->setCursorPosition( 0, 0 );
            }
        }

        QString absPath = controller->getResolvedImagePath( editedTerm->getImagePath(), vocab );
        initImage( absPath );
    }
}
