#include "ImageBox.h"
#include "icons/void.xpm"

ImageBox::ImageBox( const QString& title, const QString& hiddenLabel, const QString& hiddenTooltip, QWidget *parent = 0, const char* name = 0 )
    : QVGroupBox( title, parent, name ), imageWidth( -1 ), imageHeight( -1 ) {
    imageStack = new QWidgetStack( this, "ImageStack" );
    imageStack->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );

    imageWrapper = new QVBox( imageStack, "ImageWrapper" );
    image = new QLabel( imageWrapper, "Image" );
    image->setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Expanding ) );
    image->setAlignment( AlignHCenter | AlignVCenter );
    image->setScaledContents( true );

    imageButton = new QPushButton( hiddenLabel, imageStack, "ImageButton" );
    imageButton->installEventFilter( this );
    QToolTip::add( imageButton, hiddenTooltip );
    connect( imageButton, SIGNAL( clicked() ), this, SLOT( revealImage() ) );  
    imageStack->addWidget( imageWrapper, 0 );
    imageStack->addWidget( imageButton, 1 );
}

ImageBox::~ImageBox() {
}

QSize ImageBox::sizeHint() const {
    int width = ( imageWidth == -1 ? 200 : height() * imageWidth / imageHeight );
    return( QSize( width, height() ) );
}

void ImageBox::show() {
    QVGroupBox::show();
    restartMovie();
}

bool ImageBox::isImageRevealed() const {
    return( imageStack->visibleWidget() == imageStack->widget( 0 ) );
}

bool ImageBox::containsValidImage() const {
    // If imageWidth could not be properly initialized in setImage(),
    // we consider that the imageBox is not initialized. 
    return( imageWidth != -1 );
}

void ImageBox::revealImage() {
    imageStack->raiseWidget( 0 );
    restartMovie();
}

void ImageBox::hideImage() {
    imageStack->raiseWidget( 1 );
}

void ImageBox::restartMovie() const {
    QMovie* movie = image->movie();
    if( movie )
        movie->restart();
}

void ImageBox::setImage( const QString& path ) {
    imageWidth = imageHeight = -1;
    if( !path.isNull() ) {
        QFileInfo info( path );
        if( info.exists() ) {
            const QString& format = QPixmap::imageFormat( path );
            if( format == "GIF" || format == "PNG" ) {
                const QPixmap& pixmap( path );
                imageWidth = pixmap.width();
                imageHeight = pixmap.height();
                if( format == "GIF" ) {
                    const QMovie& movie( path );
                    image->setMovie( movie );
                }
                else if( format == "PNG" ) {
                    image->setPixmap( pixmap );
                }
                show();
                setFixedWidth( sizeHint().width() );
            }
        }
    }
    
    if( imageWidth == -1 ) {
        image->setPixmap( ZPIXMAP( void_xpm ) );
        hide();
    }
}

void ImageBox::resizeEvent( QResizeEvent* evt ) {
    QVGroupBox::resizeEvent( evt );
    if( evt->size().width() != sizeHint().width() )
        setFixedWidth( sizeHint().width() );
}
