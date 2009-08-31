#include "MoviePlayer.h"
#include "icons/play.xpm"
#include "icons/pause.xpm"
#include "icons/backward.xpm"
#include "icons/forward.xpm"

MoviePlayer::MoviePlayer( QWidget* parent, const QString& movieFile ) 
    : QWidget( parent, 0 ) {

    movieScreen = new MovieScreen( this, movieFile );

    console = new QHBox( this );

    playPauseStack = new QWidgetStack( console );

    playButton = new QPushButton( playPauseStack );
    playButton->setPixmap( ZPIXMAP( play_xpm ) );
    connect( playButton, SIGNAL( clicked() ), this, SLOT( togglePause() ) );

    pauseButton = new QPushButton( playPauseStack );
    pauseButton->setPixmap( ZPIXMAP( pause_xpm ) );
    connect( pauseButton, SIGNAL( clicked() ), this, SLOT( togglePause() ) );

    playPauseStack->addWidget( playButton, 0 );
    playPauseStack->addWidget( pauseButton, 1 );
    playPauseStack->raiseWidget( 1 );
    playPauseStack->setMaximumHeight( playButton->sizeHint().height() );

    backwardButton = new QPushButton( console );
    backwardButton->setPixmap( ZPIXMAP( backward_xpm ) );
    backwardButton->setEnabled( false );
    connect( backwardButton, SIGNAL( clicked() ), movieScreen, SLOT( backward() ) );

    forwardButton = new QPushButton( console );
    forwardButton->setPixmap( ZPIXMAP( forward_xpm ) );
    forwardButton->setEnabled( false );
    connect( forwardButton, SIGNAL( clicked() ), movieScreen, SLOT( forward() ) );

    mainLayout = new QVBoxLayout( this );
    mainLayout->addWidget( movieScreen, 1 );
    mainLayout->addWidget( console );
    mainLayout->activate();
}

MoviePlayer::~MoviePlayer() {
}

void MoviePlayer::togglePause() {
    if( movieScreen->isPaused() ) {
        playPauseStack->raiseWidget( 1 );
        backwardButton->setEnabled( false );
        forwardButton->setEnabled( false );
    }
    else {
        playPauseStack->raiseWidget( 0 );
        backwardButton->setEnabled( true );
        forwardButton->setEnabled( true );
    }
    movieScreen->togglePause();
}
