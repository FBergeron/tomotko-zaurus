#include "CharacterDialog.h"

CharacterDialog::CharacterDialog( QWidget* parent, const QString& imageFile, bool isAnimated /* = false */ ) 
    : QDialog( parent, 0, true ) {

    setSizeGripEnabled( true );

    mainLayout = new QVBoxLayout( this );

    if( isAnimated ) {
        moviePlayer = new MoviePlayer( this, imageFile );
        mainLayout->addWidget( moviePlayer );
    }
    else {
        QPixmap image( imageFile );
        imageViewer = new QLabel( this );
        imageViewer->setPixmap( image );
        mainLayout->addWidget( imageViewer );
    }

    mainLayout->activate();
}

CharacterDialog::~CharacterDialog() {
}
