#ifndef IMAGE_BOX_H
#define IMAGE_BOX_H

#include <iostream.h>
#include <qfileinfo.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qmovie.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <qvbox.h>
#include <qvgroupbox.h>
#include <qwidgetstack.h>
#include "Util.h"

class ImageBox : public QVGroupBox {

    Q_OBJECT

public:

    ImageBox( const QString& title, const QString& hiddenLabel, const QString& hiddenTooltip, QWidget* parent = 0, const char* name = 0 );
    ~ImageBox();

    void setImage( const QString& path );
    bool isImageRevealed() const;
    bool containsValidImage() const;

    virtual QSize sizeHint() const;
    virtual void show();

protected:

    virtual void resizeEvent( QResizeEvent* evt );

public slots:

    void revealImage();
    void hideImage();

private slots:

    void restartMovie() const;

private:

    int imageWidth;
    int imageHeight;

    QWidgetStack*               imageStack;
    QVBox*                      imageWrapper;
    QLabel*                     image;
    QPushButton*                imageButton;

};

#endif

