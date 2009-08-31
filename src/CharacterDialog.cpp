#include "CharacterDialog.h"

CharacterDialog::CharacterDialog( QWidget* parent, const QString& imageFile, bool isAnimated /* = false */ ) 
    : QDialog( parent, 0, true ) {

    setSizeGripEnabled( true );

    mainLayout = new QVBoxLayout( this );

    if( isAnimated ) {
        cerr << 1 << endl;
        moviePlayer = new MoviePlayer( this, imageFile );
        cerr << 2 << endl;
        mainLayout->addWidget( moviePlayer );
        cerr << 3 << endl;
    }
    else {
        QPixmap image( imageFile );
        imageViewer = new QLabel( this );
        imageViewer->setPixmap( image );
        mainLayout->addWidget( imageViewer );
    }

    mainLayout->activate();
    cerr << 4 << endl;
}

CharacterDialog::~CharacterDialog() {
}
