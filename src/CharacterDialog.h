#ifndef CHARACTER_DIALOG_H
#define CHARACTER_DIALOG_H 

#include <iostream.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpixmap.h>
#include <qwidget.h>
#include <qwidgetstack.h>
#include "MoviePlayer.h"

class CharacterDialog : public QDialog {

    Q_OBJECT
    
public:

    CharacterDialog( QWidget* parent, const QString& imageFile, bool isAnimated = false );
    ~CharacterDialog(); 

private:

    QLabel*         imageViewer;
    MoviePlayer*    moviePlayer;

    QVBoxLayout*    mainLayout;
    
};

#endif
