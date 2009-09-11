#include "SuperMemo2Quiz.h"

SuperMemo2Quiz::SuperMemo2Quiz( const QString& applDir ) : Quiz( applDir ) {
    srand( time( NULL ) );
}

SuperMemo2Quiz::SuperMemo2Quiz( const SuperMemo2Quiz& quiz ) 
    : Quiz( quiz ) /*firstLang( quiz.firstLang ), testLang( quiz.testLang ), */ {
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
    saveTermData();
}

bool SuperMemo2Quiz::isInProgress() const {
    return( terms.count() - termsToRemove.count() > 0 );
}

void SuperMemo2Quiz::init( const QString& firstLang, const QString& testLang, Folder* rootFolder ) {
    Quiz::init( firstLang, testLang, rootFolder );

    loadTermData();

    terms.clear();
    termsToRemove.clear();

    initRec( firstLang, testLang, rootFolder );
    shuffleTerms();
    currTermIndex = 0;
    cerr << "initTermCount=" << initTermCount << endl;
}

void SuperMemo2Quiz::getSchedule( int* schedule ) {
    for( int i = 0; i < scheduleLength; i++ )
        schedule[ i ] = 0;
    getScheduleRec( rootFolder, schedule );
}

void SuperMemo2Quiz::reinit() {
    // Not applicable.
}

void SuperMemo2Quiz::initRec( const QString& firstLang, const QString& testLang, Folder* folder ) {
    if( folder->isMarkedForStudy() ) {
        for( Base* child = folder->first(); child; child = folder->next() ) {
            if( strcmp( child->className(), "Folder" ) == 0 )
                initRec( firstLang, testLang, (Folder*)child );
            else if( strcmp( child->className(), "Vocabulary" ) == 0 )
                initRec( firstLang, testLang, (Vocabulary*)child );
        }
    }
}

void SuperMemo2Quiz::initRec( const QString& firstLang, const QString& testLang, Vocabulary* vocab ) {
    if( vocab->isMarkedForStudy() ) {
        for( Vocabulary::TermMap::ConstIterator it = vocab->begin(); it != vocab->end(); it++ ) {
            const Term& term = it.data();
            if( !term.isMarkedForDeletion() && term.isMarkedForStudy() && 
                term.isTranslationExists( firstLang ) && term.isTranslationExists( testLang ) ) {
                Translation firstLangTrans = term.getTranslation( firstLang );
                Translation testLangTrans = term.getTranslation( testLang );
                TermKey termKey( term.getUid(), term.getVocabUid() );
                TermData termData = getTermData( termKey.getTermUid().toString() );
                if( termData.nextRepetitionDate.isNull() || QDate::currentDate().daysTo( termData.nextRepetitionDate ) <= 0 ) {
                    terms.append( termKey );
                    initTermCount++;
                }
            }
        }
    }
}

void SuperMemo2Quiz::getScheduleRec( Folder* folder, int* schedule ) {
    if( folder->isMarkedForStudy() ) {
        for( Base* child = folder->first(); child; child = folder->next() ) {
            if( strcmp( child->className(), "Folder" ) == 0 )
                getScheduleRec( (Folder*)child, schedule );
            else if( strcmp( child->className(), "Vocabulary" ) == 0 )
                getScheduleRec( (Vocabulary*)child, schedule );
        }
    }
}

void SuperMemo2Quiz::getScheduleRec( Vocabulary* vocab, int* schedule ) {
    if( vocab->isMarkedForStudy() ) {
        for( Vocabulary::TermMap::ConstIterator it = vocab->begin(); it != vocab->end(); it++ ) {
            const Term& term = it.data();
            if( !term.isMarkedForDeletion() && term.isMarkedForStudy() && 
                term.isTranslationExists( firstLang ) && term.isTranslationExists( testLang ) ) {
                Translation firstLangTrans = term.getTranslation( firstLang );
                Translation testLangTrans = term.getTranslation( testLang );
                TermKey termKey( term.getUid(), term.getVocabUid() );
                TermData termData = getTermData( termKey.getTermUid().toString() );

                QDate today = QDate::currentDate();
                for( int i = 0; i < scheduleLength; i++ ) {
                    QDate date = today.addDays( i );
                    if( termData.nextRepetitionDate.isNull() ) {
                        if( i == 0 )
                            schedule[ i ]++;
                    }
                    else {
                        if( termData.nextRepetitionDate == date || ( i == 0 && termData.nextRepetitionDate < date ) )
                            schedule[ i ]++;
                    }
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
    TermData termData = getTermData( currTerm.getTermUid().toString() );
    cerr << "TermData before: repetition=" << termData.repetition << " interval=" << termData.interval << " EF=" << termData.easinessFactor << " nextRepDate=" << ( termData.nextRepetitionDate.isNull() ? QString( "null" ) : termData.nextRepetitionDate.toString() ) << endl;
    if( grade >= 3 ) {
        termData.interval = getNextInterval( termData.interval, termData.easinessFactor, termData.repetition );
        termData.repetition++;
        termData.nextRepetitionDate = QDate::currentDate().addDays( termData.interval );

        termsToRemove.append( terms[ currTermIndex ] ); // Stop asking the term for this quiz.
    }
    else {
        termData.repetition = 0;
        termData.interval = 1;
        termData.nextRepetitionDate = QDate();
    }
    termData.easinessFactor = termData.easinessFactor + ( 0.1 - ( 5 - grade ) * ( 0.08 + ( 5 - grade ) * 0.02 ) );
    if( termData.easinessFactor < 1.3 )
        termData.easinessFactor = 1.3;

    cerr << "TermData after : repetition=" << termData.repetition << " interval=" << termData.interval << " EF=" << termData.easinessFactor << " nextRepDate=" << termData.nextRepetitionDate.toString() << endl;
    setTermData( currTerm.getTermUid().toString(), termData ); 
}

int SuperMemo2Quiz::getProgress() const {
    return( initTermCount == 0 ? 0 : initTermCount - terms.count() + termsToRemove.count() );
}

int SuperMemo2Quiz::getAnswerCount() const {
    return( 6 );
}

void SuperMemo2Quiz::showProgressData( QWidget* parent ) {
    ProgressData progressData;

    TermKey currTermKey = getCurrentTerm();
    if( !currTermKey.isNull() ) {
        TermData termData = getTermData( currTermKey.getTermUid().toString() );

        progressData.currTerm.repetition = termData.repetition;
        progressData.currTerm.easinessFactor = termData.easinessFactor;
        progressData.currTerm.daysToNextRepetition = ( termData.nextRepetitionDate.isNull() ? 0 : QDate::currentDate().daysTo( termData.nextRepetitionDate ) );
    }

    getSchedule( progressData.scheduleForDay );

    ProgressDialog dialog( parent, progressData );
    dialog.resize( 440, 330 ); 
    dialog.show();
    dialog.exec();
}

float SuperMemo2Quiz::getCurrentTermEasinessFactor() {
    TermKey currTermKey = getCurrentTerm();
    if( !currTermKey.isNull() ) {
        TermData termData = getTermData( currTermKey.getTermUid().toString() );
        return( termData.easinessFactor );
    }

    return( Quiz::getCurrentTermEasinessFactor() );
}

int SuperMemo2Quiz::getCurrentTermRepetition() {
    TermKey currTermKey = getCurrentTerm();
    if( !currTermKey.isNull() ) {
        TermData termData = getTermData( currTermKey.getTermUid().toString() );
        return( termData.repetition );
    }

    return( Quiz::getCurrentTermRepetition() );
}

int SuperMemo2Quiz::getCurrentTermNextRepetition() {
    TermKey currTermKey = getCurrentTerm();
    if( !currTermKey.isNull() ) {
        TermData termData = getTermData( currTermKey.getTermUid().toString() );
        int nextInterval = getNextInterval( termData.interval, termData.easinessFactor, termData.repetition );
        return( nextInterval );
    }

    return( Quiz::getCurrentTermNextRepetition() );
}

QString SuperMemo2Quiz::getTermDataFilename() const {
    BilingualKey key( firstLang, testLang );
    return( applDir + "/quiz_sm2_termData_" + key.toString() + ".dat.z" );
}

SuperMemo2Quiz::TermData SuperMemo2Quiz::getTermData( const QString& termUid ) {
    if( allTermData.contains( termUid ) )
        return( allTermData[ termUid ] );
    else {
        TermData termData;
        termData.interval = 0;
        termData.repetition = 0;
        termData.easinessFactor = 2.5;
        termData.nextRepetitionDate = QDate();
        allTermData[ termUid ] = termData;
        return( termData );
    }
}

void SuperMemo2Quiz::setTermData( const QString& termUid, const TermData& data ) {
    allTermData[ termUid ] = data;
}

bool SuperMemo2Quiz::loadTermData() {
    QFile dataFile( getTermDataFilename() );
    if( !dataFile.exists() )
        return( true );

    if( !dataFile.open( IO_ReadOnly ) ) {
        cerr << "Cannot open metadata file: " << dataFile.name() << endl;
        return( false );
    }
    
    QByteArray compressedData( dataFile.readAll() );
    dataFile.close();

    QByteArray data( Util::qUncompress( compressedData ) );

    QDataStream in( data, IO_ReadOnly );

    Q_UINT32 tempMagicNumber;
    Q_UINT16 tempVersion;
    QString tempTermUid;
    int tempInterval;
    uint tempRepetition;
    float tempEasinessFactor;
    QDate tempNextRepetitionDate;
   
    in >> tempMagicNumber >> tempVersion;
    if( tempMagicNumber != SuperMemo2Quiz::magicNumber ) {
        cerr << "Wrong magic number: Incompatible SuperMemo2Quiz data file." << endl;
        return( false );
    }
    if( tempVersion > 0x0001 ) {
        cerr << "SuperMemo2Quiz data file is from a more recent version.  Upgrade toMOTko." << endl;
        return( false );
    }
    if( tempVersion < 0x0001 ) {
        cerr << "SuperMemo2Quiz data format too old.  You must use an anterior version of toMOTko." << endl;
        return( false );
    }

    in.setVersion( 3 );
    while( !in.atEnd() ) {
        in >> tempTermUid >> tempInterval >> tempRepetition >> tempEasinessFactor >> tempNextRepetitionDate;

        TermData termData;
        termData.interval = tempInterval;
        termData.repetition = tempRepetition;
        termData.easinessFactor = tempEasinessFactor;
        termData.nextRepetitionDate = tempNextRepetitionDate;

        allTermData[ tempTermUid ] = termData;

        cerr << tempTermUid << ": interval=" << tempInterval << " repetition=" << tempRepetition << " EF=" << tempEasinessFactor << " nextRepDate=" << tempNextRepetitionDate.toString() << endl;
    }

    return( true );
}

bool SuperMemo2Quiz::saveTermData() const {
    QByteArray data;

    QDataStream out( data, IO_WriteOnly );
    out.setVersion( 3 /* QDataStream::Qt_3 ? */ );

    out << Q_UINT32( SuperMemo2Quiz::magicNumber ) << Q_UINT16( 0x0001 );
    for( QMap<QString, TermData>::ConstIterator it = allTermData.begin(); it != allTermData.end(); it++ ) {
        QString termUid = it.key();
        TermData termData = it.data();
        out << termUid << termData.interval << termData.repetition << termData.easinessFactor << termData.nextRepetitionDate;
    }

    QByteArray compressedData( Util::qCompress( data ) ); 

    QFile dataFile( getTermDataFilename() );
    QFileInfo dataFileInfo( dataFile );

    if( !Util::makeDirectory( dataFileInfo.dirPath() ) )
        return( false );

    if( !dataFile.open( IO_WriteOnly ) )
        return( false );

    dataFile.writeBlock( compressedData );
    dataFile.close();

    return( true );
}
