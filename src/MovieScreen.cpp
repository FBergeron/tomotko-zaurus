#include "MovieScreen.h"

MovieScreen::MovieScreen( QWidget* parent, const QString& movieFile ) 
    : QWidget( parent, 0, Qt::WResizeNoErase | Qt::WRepaintNoErase ), movieFile( movieFile ), preferredSize( 160, 160 ), previousFrameNumber( -1 ) {

    setBackgroundMode( NoBackground ); // Prevents flickering.

    movie = QMovie( movieFile );
    movie.connectUpdate( this, SLOT( movieUpdated( const QRect& ) ) );
    movie.connectResize( this, SLOT( movieResized( const QSize& ) ) );
    movie.connectStatus( this, SLOT( movieStatus( int ) ) );

    setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
}

MovieScreen::~MovieScreen() {
}

bool MovieScreen::isPaused() const {
    return( movie.paused() );
}

QSize MovieScreen::sizeHint() const {
    return( preferredSize );
}

void MovieScreen::togglePause() {
    if( movie.paused() )
        movie.unpause();
    else
        movie.pause();
    repaint();
}

void MovieScreen::forward() {
    movie.step();
    repaint();
}

void MovieScreen::backward() {
    if( movie.frameNumber() >= 2 ) {
        previousFrameNumber = movie.frameNumber() - 2;
        movie.setSpeed( 6400 ); // Increase the speed to reach the previousFrame as quickly as possible.
        movie.unpause();
    }
}

void MovieScreen::paintEvent( QPaintEvent* /*evt*/ ) {
    QPainter p( this );
    if( !movie.isNull() ) {
        QPixmap pm = movie.framePixmap();
        QImage scaledImage = pm.convertToImage().smoothScale( size().width(), size().height() );
        pm.convertFromImage( scaledImage );
        QPixmap pmScreen( pm );
        p.drawPixmap( 0, 0, pmScreen );
    }
}

void MovieScreen::movieUpdated( const QRect& /*rect*/ ) {
    //// The given area of the movie has changed.
    //QRect r = contentsRect();

    //if ( r.size() != movie.framePixmap().size() ) {
    //    // Need to scale - redraw whole frame.
    //    repaint( r );
    //} else {
    //    // Only redraw the changed area of the frame
    //    repaint( area.x()+r.x(), area.y()+r.x(),
    //             area.width(), area.height() );
    //}
    if( previousFrameNumber == -1 )
        repaint();
    if( movie.frameNumber() == previousFrameNumber ) {
        previousFrameNumber = -1;
        movie.setSpeed( 100 );
        movie.pause();
        repaint();
    }
}

void MovieScreen::movieResized( const QSize& size ) {
    QSize currSize( this->size().width(), this->size().height() );
    preferredSize = QSize( size.width() * 2, size.height() * 2 );
//cout << "currSize=" << currSize.width() << "x" << currSize.height() << " size=" << size.width() << "x" << size.height() << endl;
    updateGeometry();
}

void MovieScreen::movieStatus( const int status ) {
    // Prevents repaints() when reaching previousFrameNumber.
    if( previousFrameNumber == -1 ) {
        if( status == QMovie::Paused || status == QMovie::EndOfMovie )
            repaint();
    }
}
