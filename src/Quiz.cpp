#include "Quiz.h"

Quiz::Quiz( const Quiz& quiz ) 
    :  Base(), firstLang( quiz.firstLang ), testLang( quiz.testLang ), 
        initTermCount( quiz.initTermCount ), 
            applDir( quiz.applDir ) {
}

void Quiz::init( const QString& firstLanguage, const QString& testLanguage, Folder* rootFolder ) {
    this->firstLang = firstLanguage;
    this->testLang = testLanguage;
    this->rootFolder = rootFolder;
    this->initTermCount = 0;
}

void Quiz::showProgressData( QWidget* ) {
    // Do nothing by default.  Subclasses should override this if needed.
}

float Quiz::getCurrentTermEasinessFactor() {
    // Returns zero EF by default.  Subclasses should override this if needed.
    return( 0.0f );
}

int Quiz::getCurrentTermNextRepetition() {
    // Returns zero value by default.  Subclasses should override this if needed.
    return( 0 );
}
