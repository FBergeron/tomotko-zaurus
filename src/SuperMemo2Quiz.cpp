#include "SuperMemo2Quiz.h"

SuperMemo2Quiz::SuperMemo2Quiz( const QString& applDir ) : Quiz( applDir ) {
    srand( time( NULL ) );
}

SuperMemo2Quiz::SuperMemo2Quiz( const SuperMemo2Quiz& quiz ) 
    : Quiz( quiz ) {
}

SuperMemo2Quiz::~SuperMemo2Quiz() {
}

bool SuperMemo2Quiz::isResumable() const {
    return( false );
}

bool SuperMemo2Quiz::load() {
    // Nothing to do here as SM-2 quizzes are not resumable.
    return( true );
}

bool SuperMemo2Quiz::save() {
    // Nothing to do here as SM-2 quizzes are not resumable.
    return( true );
}

void SuperMemo2Quiz::conclude() {
    // Notthing to do here.  Data has already been saved.
}

bool SuperMemo2Quiz::isInProgress() const {
    return( terms.count() - termsToRemove.count() > 0 );
}

void SuperMemo2Quiz::init( const QString& firstLang, const QString& testLang, Folder* rootFolder ) {
    Quiz::init( firstLang, testLang, rootFolder );

    terms.clear();
    termsToRemove.clear();

    initRec( firstLang, testLang, rootFolder );
    shuffleTerms();
    currTermIndex = 0;
    cerr << "initTermCount=" << initTermCount << endl;
}

void SuperMemo2Quiz::reinit() {
    // Not applicable.
}

void SuperMemo2Quiz::initRec( const QString& firstLang, const QString& testLang, Folder* folder ) {
    if( !folder->isMarkedForDeletion() && folder->isMarkedForStudy() ) {
        for( Base* child = folder->first(); child; child = folder->next() ) {
            if( strcmp( child->className(), "Folder" ) == 0 )
                initRec( firstLang, testLang, (Folder*)child );
            else if( strcmp( child->className(), "Vocabulary" ) == 0 )
                initRec( firstLang, testLang, (Vocabulary*)child );
        }
    }
}

void SuperMemo2Quiz::initRec( const QString& firstLang, const QString& testLang, Vocabulary* vocab ) {
    if( !vocab->isMarkedForDeletion() && vocab->isMarkedForStudy() ) {
        for( Vocabulary::TermMap::ConstIterator it = vocab->begin(); it != vocab->end(); it++ ) {
            const Term& term = it.data();
            if( !term.isMarkedForDeletion() && term.isMarkedForStudy() && 
                term.isTranslationExists( firstLang ) && term.isTranslationExists( testLang ) ) {
                Translation firstLangTrans = term.getTranslation( firstLang );
                Translation testLangTrans = term.getTranslation( testLang );
                TermKey termKey( term.getUid(), term.getVocabUid() );
                TermData termData = Statistics::instance()->loadTermData( termKey.getTermUid().toString(), firstLang, testLang );
                cerr << "uid=" << term.getUid().toString();
                if( termData.nextRepetitionDate.isNull() )
                    cerr << "nr date is null" << endl;
                else {
                    cerr << "currDate=" << QDate::currentDate().toString() << " nr date=" << termData.nextRepetitionDate.toString() << endl;
                }
                if( termData.nextRepetitionDate.isNull() || QDate::currentDate().daysTo( termData.nextRepetitionDate ) <= 0 ) {
                    terms.append( termKey );
                    initTermCount++;
                }
            }
        }
    }
}

void SuperMemo2Quiz::shuffleTerms() {
    QValueList<TermKey> shuffledTerms;
    while( !terms.isEmpty() ) {
        const uint index = rand() % terms.count();
        TermKey key = terms[ index ];
        shuffledTerms.append( key );
        terms.remove( key );
    }
    for( QValueList<TermKey>::Iterator it = shuffledTerms.begin(); it != shuffledTerms.end(); it++ )
        terms.append( *it );
    shuffledTerms.clear();
}

void SuperMemo2Quiz::discardCurrentTerm() {
    termsToRemove.append( terms[ currTermIndex ] );
}

bool SuperMemo2Quiz::hasNextTerm() const {
    if( currTermIndex < terms.count() - 1 )
        return( true );
    return( terms.count() - termsToRemove.count() > 0 );
}

const TermKey SuperMemo2Quiz::getCurrentTerm() const {
    return( terms[ currTermIndex ] );
}

const TermKey SuperMemo2Quiz::getNextTerm() {
    TermKey previousTerm = terms[ currTermIndex ];

    //cerr << "getNextTerm currTermIndex=" << currTermIndex << " terms.count()=" << terms.count() << endl;
    if( currTermIndex < terms.count() - 1 ) {
        currTermIndex++;
        return( terms[ currTermIndex ] );
    }

    for( QValueList<TermKey>::Iterator it = termsToRemove.begin(); it != termsToRemove.end(); it++ )
        terms.remove( *it );
    termsToRemove.clear();

    shuffleTerms();

    // To prevent asking the same term twice in a row, we remove the first term 
    // if it's equal to the previous one and append it at the end of the list.
    if( terms[ 0 ] == previousTerm ) {
        terms.remove( previousTerm );
        terms.append( previousTerm ); 
    }

    currTermIndex = 0;
    return( terms[ currTermIndex ] );
}

int SuperMemo2Quiz::getNextInterval( int interval, float easinessFactor, int repetition ) const {
    if( repetition == 0 )
        return( 1 );
    else if( repetition == 1 )
        return( 6 );
    else 
        return( (int)( ( interval * easinessFactor ) + 0.5 ) ); // (int) ( val + 0.5) to round up the value.
}

void SuperMemo2Quiz::gradeAnswer( int grade ) {
    TermKey currTerm = terms[ currTermIndex ];
    TermData termData = Statistics::instance()->getTermData( currTerm.getTermUid().toString() );
    cerr << "TermData before: repetition=" << termData.repetition << " interval=" << termData.interval << " EF=" << termData.easinessFactor << " nextRepDate=" << ( termData.nextRepetitionDate.isNull() ? QString( "null" ) : termData.nextRepetitionDate.toString() ) << endl;
    if( grade >= 3 ) {
        termData.successCount += 1;
        termData.interval = getNextInterval( termData.interval, termData.easinessFactor, termData.repetition );
        termData.repetition++;
        termData.nextRepetitionDate = QDate::currentDate().addDays( termData.interval );

        termsToRemove.append( terms[ currTermIndex ] ); // Stop asking the term for this quiz.
    }
    else {
        termData.missCount += 1;
        termData.repetition = 0;
        termData.interval = 1;
        termData.nextRepetitionDate = QDate::currentDate();
    }
    termData.easinessFactor = termData.easinessFactor + ( 0.1 - ( 5 - grade ) * ( 0.08 + ( 5 - grade ) * 0.02 ) );
    termData.lastRepetitionDate = QDate::currentDate();
    if( termData.easinessFactor < 1.3 )
        termData.easinessFactor = 1.3;

    cerr << "TermData after : repetition=" << termData.repetition << " interval=" << termData.interval << " EF=" << termData.easinessFactor << " nextRepDate=" << termData.nextRepetitionDate.toString() << endl;
    Statistics::instance()->setTermData( currTerm.getTermUid().toString(), firstLang, testLang, termData ); 
}

int SuperMemo2Quiz::getProgress() const {
    return( initTermCount == 0 ? 0 : initTermCount - terms.count() + termsToRemove.count() );
}

int SuperMemo2Quiz::getAnswerCount() const {
    return( 6 );
}

//void SuperMemo2Quiz::showProgressData( QWidget* /*parent*/ ) {
    //ProgressData progressData;

    //TermKey currTermKey = getCurrentTerm();
    //if( !currTermKey.isNull() ) {
    //    TermData termData = getTermData( currTermKey.getTermUid().toString() );

    //    progressData.currTerm.repetition = termData.repetition;
    //    progressData.currTerm.easinessFactor = termData.easinessFactor;
    //    progressData.currTerm.daysToNextRepetition = ( termData.nextRepetitionDate.isNull() ? 0 : QDate::currentDate().daysTo( termData.nextRepetitionDate ) );
    //}

    //getSchedule( progressData.scheduleForDay );
    //getEFDistribution( progressData.efDistribution );

    //ProgressDialog dialog( parent, progressData );
    //dialog.resize( 440, 330 ); 
    //dialog.show();
    //dialog.exec();
//}
