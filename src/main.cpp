/****************************************************************
**
** toMOTko
** 
** Small application to learn vocabulary on my Zaurus.
**
** Author : Frederic Bergeron (c) 2006
**
****************************************************************/

#include <qpe/qpeapplication.h>
#include <qfont.h>
#include "Controller.h"
#include "MainWindow.h"
#include "zlib.h"

QAction* action[ ACTION_COUNT ];

int main( int argc, char **argv ) {
    // Check if zlib is ok.  Taken from zlib's example.c file.
    static const char* myVersion = ZLIB_VERSION;

    if( zlibVersion()[ 0 ] != myVersion[ 0 ] ) {
        fprintf( stderr, "incompatible zlib version\n" );
        exit( 1 );
    } else if( strcmp( zlibVersion(), ZLIB_VERSION ) != 0 ) {
        fprintf( stderr, "warning: different zlib version\n" );
    }

    // printf( "zlib version %s = 0x%04x, compile flags = 0x%lx\n", ZLIB_VERSION, ZLIB_VERNUM, zlibCompileFlags() );
    // End of zlib check.

    QPEApplication a( argc, argv );
    Controller* controller = new Controller();
    if( !controller->init() ) {
        cerr << "Cannot initialize controller.  Check disk space and file permissions." << endl;
        exit( 2 );
    }

    QFont labelsFont( controller->getPreferences().getLabelsFont() ); 
    qApp->setFont( labelsFont );

    MainWindow* mainWindow = new MainWindow( controller );
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    a.showMainWidget( mainWindow );
    return a.exec();
}
