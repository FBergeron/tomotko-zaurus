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
    //controller->loadTermData();
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

//int main( int /*argc*/, char ** /*argv*/ ) {
//    Controller* controller = new Controller();
//    controller->init();
//    //QString termUid = "{00000000-0000-0000-0000-000000000000}";
//    //QString termUid = "{zzzzzzzz-zzzz-zzzz-zzzz-zzzzzzzzzzzz}";
//    //QString termUid = "{007597ea-28af-4ef6-8bea-60a05fe36c6e}";
//    //QString termUid = "{ffd34b57-dc0b-4cdf-bacf-a7a41d7a4944}";
//
//    //QString termUid = "{f86fce9a-5490-4cd8-b393-12640ea8ca5d}";
//    //cerr << "termUid=" << termUid << endl;
//    //TermData termData = Statistics::instance()->loadTermData( termUid, QString( "de" ), QString( "fr" ) );
//    //cerr << "interval=" << termData.interval << " repetition=" << termData.repetition << " EF=" << termData.easinessFactor << endl;
//
//    
//
//    //termData.repetition++;
//    //termData.easinessFactor = 3.0f;
//    //bool wasWritten = controller->saveTermData( termUid, termData );
//    //cerr << "wasWritten=" << wasWritten << endl;
//    //termData = controller->loadTermData( termUid );
//    //cerr << "interval=" << termData.interval << " repetition=" << termData.repetition << " EF=" << termData.easinessFactor << endl;
//
//    QString firstLang = QString( "es" );
//    QString testLang = QString( "fr" );
//    bool isLoaded = Statistics::instance()->loadTermData( firstLang, testLang );
//    cerr << "loaded=" << isLoaded << endl;
//    if( isLoaded ) {
//        bool isSaved = Statistics::instance()->saveUncompressedTermData( firstLang, testLang );
//        cerr << "saved=" << isSaved << endl;
//    }
//    //controller->saveTermData();
//    return( 0 );
//}

//int main( int /*argc*/, char ** /*argv*/ ) {
//    Controller* controller = new Controller();
//    controller->init();
//    
//    //QString firstLang = QString( "de" );
//    //QString testLang = QString( "fr" );
//    //QString firstLang = QString( "en" );
//    //QString testLang = QString( "fr" );
//    //QString firstLang = QString( "en" );
//    //QString testLang = QString( "ja" );
//    QString firstLang = QString( "es" );
//    QString testLang = QString( "fr" );
//    bool isLoaded = Statistics::instance()->loadTermData( firstLang, testLang );
//    cerr << "loaded=" << isLoaded << endl;
//    if( isLoaded ) {
//        bool isSaved = Statistics::instance()->saveTermData( firstLang, testLang );
//        cerr << "saved=" << isSaved << endl;
//    }
//    //controller->saveTermData();
//    return( 0 );
//}

//int main( int /*argc*/, char ** /*argv*/ ) {
//    Controller* controller = new Controller();
//    controller->init();
//    
//    QString firstLang = QString( "en" );
//    QString testLang = QString( "ja" );
//    BiUidKey key( "{5da6f446-557b-43fe-bb90-559384631029}|{bdbebae9-7e83-4edb-86f0-853a8df30de5}" );
//    cerr << "my looking key=" << key.toString() << endl;
//    TermData termData = Statistics::instance()->loadTermData( key, firstLang, testLang );
//    cerr << "ef=" << termData.easinessFactor << " success=" << termData.successCount << " miss=" << termData.missCount << endl;
//    //if( isLoaded ) {
//    //    bool isSaved = Statistics::instance()->saveTermData( firstLang, testLang );
//    //    cerr << "saved=" << isSaved << endl;
//    //}
//    //controller->saveTermData();
//    return( 0 );
//}

//int main( int /*argc*/, char ** /*argv*/ ) {
//    Controller* controller = new Controller();
//    controller->init();
//    
//    QString firstLang = QString( "en" );
//    QString testLang = QString( "ja" );
//    bool isLoaded = Statistics::instance()->loadTermData( firstLang, testLang );
//    cerr << "loaded=" << isLoaded << " BEFORE=" <<endl;
//
//    QStringList str = Statistics::instance()->toString();
//    for( QStringList::ConstIterator it = str.begin(); it != str.end(); it++ ) {
//        cerr << (*it).latin1() << endl;
//    }
//
////    // Remove 3 first entries.
////    QStringList uidToRemove;
////    uidToRemove += "{10e2d7da-6060-4345-b773-da1abd8dbf98}";
////    uidToRemove += "{11517bc0-2443-42d9-9fe7-7702daf9f6cd}";
////    uidToRemove += "{205f60e4-32f2-4095-bc60-21e9593cdb13}";
////
////    Statistics::instance()->removeTermDataFromFile( uidToRemove, "/home/fred/.toMOTko/termData_en-ja.dat" );
////
////    cerr << "AFTER=" << endl;
////    str = Statistics::instance()->toString();
////    for( QStringList::ConstIterator it = str.begin(); it != str.end(); it++ ) {
////        cerr << (*it).latin1() << endl;
////    }
//
//    return( 0 );
//}

