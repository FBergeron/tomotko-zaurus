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
