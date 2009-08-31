#ifndef MOVIE_PLAYER_H
#define MOVIE_PLAYER_H 

#include <iostream.h>
#include <qhbox.h>
#include <qlayout.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qwidget.h>
#include <qwidgetstack.h>
#include "MovieScreen.h"
#include "PixMap.h"

class MoviePlayer : public QWidget {

    Q_OBJECT
    
public:

    MoviePlayer( QWidget* parent, const QString& imageFile );
    ~MoviePlayer(); 

private slots:

    void togglePause();

private:

    MovieScreen*    movieScreen;

    QPushButton*    backwardButton;
    QPushButton*    forwardButton;
    QWidgetStack*   playPauseStack;
    QPushButton*    playButton;
    QPushButton*    pauseButton;

    QHBox*          console;

    QVBoxLayout*    mainLayout;

};

#endif
