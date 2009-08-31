#ifndef MOVIE_SCREEN_H
#define MOVIE_SCREEN_H 

#include <iostream.h>
#include <qmovie.h>
#include <qimage.h>
#include <qpainter.h>
#include <qwidget.h>

class MovieScreen : public QWidget {

    Q_OBJECT
    
public:

    MovieScreen( QWidget* parent, const QString& movieFile );
    ~MovieScreen(); 

    bool isPaused() const;

public slots:

    QSize sizeHint() const;

    void togglePause();
    void forward();
    void backward();

protected slots:

protected:

    virtual void paintEvent( QPaintEvent* evt );

private slots:

    void movieUpdated( const QRect& );
    void movieResized( const QSize& );
    void movieStatus( int );

private:

    QString         movieFile;

    QMovie          movie;

    QSize           preferredSize;

};

#endif

