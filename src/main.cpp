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
#include "Progress.h"
#include "Statistics.h"
#include "zlib.h"

QAction* action[ ACTION_COUNT ];

//int main( int argc, char **argv ) {
//    // Check if zlib is ok.  Taken from zlib's example.c file.
//    static const char* myVersion = ZLIB_VERSION;
//
//    if( zlibVersion()[ 0 ] != myVersion[ 0 ] ) {
//        fprintf( stderr, "incompatible zlib version\n" );
//        exit( 1 );
//    } else if( strcmp( zlibVersion(), ZLIB_VERSION ) != 0 ) {
//        fprintf( stderr, "warning: different zlib version\n" );
//    }
//
//    // printf( "zlib version %s = 0x%04x, compile flags = 0x%lx\n", ZLIB_VERSION, ZLIB_VERNUM, zlibCompileFlags() );
//    // End of zlib check.
//
//    QPEApplication a( argc, argv );
//    Controller* controller = new Controller();
//    //controller->loadTermData();
//    if( !controller->init() ) {
//        cerr << "Cannot initialize controller.  Check disk space and file permissions." << endl;
//        exit( 2 );
//    }
//
//    QFont labelsFont( controller->getPreferences().getLabelsFont() ); 
//    qApp->setFont( labelsFont );
//
//    MainWindow* mainWindow = new MainWindow( controller );
//    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
//    a.showMainWidget( mainWindow );
//    return a.exec();
//}

int main( int /*argc*/, char ** /*argv*/ ) {
    Controller* controller = new Controller();
    controller->init();
    //QString termUid = "{00000000-0000-0000-0000-000000000000}";
    //QString termUid = "{zzzzzzzz-zzzz-zzzz-zzzz-zzzzzzzzzzzz}";
    //QString termUid = "{007597ea-28af-4ef6-8bea-60a05fe36c6e}";
    //QString termUid = "{ffd34b57-dc0b-4cdf-bacf-a7a41d7a4944}";

    QString termUid = "{f86fce9a-5490-4cd8-b393-12640ea8ca5d}";
    cerr << "termUid=" << termUid << endl;
    TermData termData = Statistics::instance()->loadTermData( termUid, QString( "de" ), QString( "fr" ) );
    cerr << "interval=" << termData.interval << " repetition=" << termData.repetition << " EF=" << termData.easinessFactor << endl;

    

    //termData.repetition++;
    //termData.easinessFactor = 3.0f;
    //bool wasWritten = controller->saveTermData( termUid, termData );
    //cerr << "wasWritten=" << wasWritten << endl;
    //termData = controller->loadTermData( termUid );
    //cerr << "interval=" << termData.interval << " repetition=" << termData.repetition << " EF=" << termData.easinessFactor << endl;

    //controller->saveTermData( 
    //controller->loadTermData();
    //controller->saveUncompressedTermData();
    //controller->saveTermData();
    return( 0 );
}
