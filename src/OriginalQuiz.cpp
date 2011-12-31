#include "OriginalQuiz.h"

OriginalQuiz::OriginalQuiz( const QString& applDir, const uint quizLength /* = 3 */ ) : Quiz( applDir ), initQuizLength( quizLength ) {
    srand( time( NULL ) );
}

OriginalQuiz::OriginalQuiz( const OriginalQuiz& quiz ) 
    : Quiz( quiz ), /*firstLang( quiz.firstLang ), testLang( quiz.testLang ), */
        standbyPool( quiz.standbyPool ), allTerms( quiz.allTerms ), 
            initQuizLength( quiz.initQuizLength ), 
                currTermPool( quiz.currTermPool ), currTerm( quiz.currTerm ) {
    for( uint i = 0; i < poolCount; i++ )
        termPool[ i ] = quiz.termPool[ i ];
}

OriginalQuiz::~OriginalQuiz() {
}

bool OriginalQuiz::isResumable() const {
    QFile dataFile( getFilename() );
    return( dataFile.exists() );
}

bool OriginalQuiz::load() {
    QFile dataFile( getFilename() );
    if( !dataFile.open( IO_ReadOnly ) )
        return( false );
    
    QByteArray compressedData( dataFile.readAll() );
    dataFile.close();
    QByteArray data( Util::qUncompress( compressedData ) );

    QDataStream in( data, IO_ReadOnly );

    Q_UINT32 tempMagicNumber;
    Q_UINT16 tempVersion;

    in >> tempMagicNumber >> tempVersion;
cerr << "quiz ver=" << tempVersion << endl;
    if( tempMagicNumber != magicNumber ) {
        cerr << "Wrong magic number: Incompatible data file." << endl;
        return( false );
    }
    if( tempVersion > 0x000a ) {
        cerr << "Data file is from a more recent version.  Upgrade toMOTko." << endl;
        return( false );
    }
    if( tempVersion < 0x000a ) {
        cerr << "Data format too old.  You must use an anterior version of toMOTko." << endl;
        return( false );
    }

    in.setVersion( 3 );

    OriginalQuiz tempQuiz( applDir );
    in >> tempQuiz;

    firstLang = tempQuiz.firstLang;
    testLang = tempQuiz.testLang;
    for( uint i = 0; i < poolCount; i++ )
        termPool[ i ] = tempQuiz.termPool[ i ];
    standbyPool = tempQuiz.standbyPool;
    allTerms = tempQuiz.allTerms;
    initQuizLength = tempQuiz.initQuizLength;
    initTermCount = tempQuiz.initTermCount;
    currTermPool = tempQuiz.currTermPool;
    currTerm = tempQuiz.currTerm;

    return( true );
}

bool OriginalQuiz::save() {
//    QByteArray data;
//
//    QDataStream out( data, IO_WriteOnly );
//    out.setVersion( 3 /* QDataStream::Qt_3 ? */ );
//
//    // 0x0009 means 0.9.x version.  
//    out << Q_UINT32( OriginalQuiz::magicNumber ) << Q_UINT16( 0x000a ) << *this;
//    
//    QByteArray compressedData( Util::qCompress( data ) ); 
//
//    QFile dataFile( getFilename() );
//    QFileInfo dataFileInfo( dataFile );
//
//    if( !Util::makeDirectory( dataFileInfo.dirPath() ) )
//        return( false );
//
//    if( !dataFile.open( IO_WriteOnly ) )
//        return( false );
//
//    int ret = dataFile.writeBlock( compressedData );
//    dataFile.close();
//
//    if( ret == -1 || dataFile.status() != IO_Ok ) {
//        dataFile.resetStatus();
//        return( false );
//    }
//
//    return( true );
    return( false );
}

void OriginalQuiz::conclude() {
    if( isInProgress() )
        save();
    else {
        QString filename( getFilename() );
        QFile dataFile( filename );
        if( dataFile.exists() ) {
            if( !dataFile.remove() )
                cerr << "Could not remove file " << filename << ". Status=" << dataFile.status() << endl;
        }
    }
}

bool OriginalQuiz::isInProgress() const {
    for( uint i = 0; i < poolCount; i++ ) {
        if( termPool[ i ].count() > 0 )
            return( true );
    }
    return( standbyPool.count() > 0 );
}

void OriginalQuiz::init( const QString& firstLang, const QString& testLang, Folder* rootFolder ) {
    Quiz::init( firstLang, testLang, rootFolder );

    for( uint i = 0; i < poolCount; i++ )
        termPool[ i ].clear();
    allTerms.clear();
    initRec( firstLang, testLang, rootFolder );
    standbyPool.clear();
    currTermPool = 0;
}

void OriginalQuiz::reinit() {
    for( uint i = 0; i < poolCount; i++ )
        termPool[ i ].clear();
    standbyPool.clear();
    for( QValueList<TermKey>::ConstIterator it = allTerms.begin(); it != allTerms.end(); it++ ) {
        TermKey termKey = *it;
        addTerm( termKey, ( poolCount - 1 ) - initQuizLength );
    }
    currTermPool = 0;
}

void OriginalQuiz::initRec( const QString& firstLang, const QString& testLang, Folder* folder ) {
    if( folder->isMarkedForStudy() ) {
        for( Base* child = folder->first(); child; child = folder->next() ) {
            if( strcmp( child->className(), "Folder" ) == 0 )
                initRec( firstLang, testLang, (Folder*)child );
            else if( strcmp( child->className(), "Vocabulary" ) == 0 )
                initRec( firstLang, testLang, (Vocabulary*)child );
        }
    }
}

void OriginalQuiz::initRec( const QString& firstLang, const QString& testLang, Vocabulary* vocab ) {
    if( vocab->isMarkedForStudy() ) {
        for( Vocabulary::TermMap::ConstIterator it = vocab->begin(); it != vocab->end(); it++ ) {
            const Term& term = it.data();
            if( !term.isMarkedForDeletion() && term.isMarkedForStudy() && 
                term.isTranslationExists( firstLang ) && term.isTranslationExists( testLang ) ) {
                Translation firstLangTrans = term.getTranslation( firstLang );
                Translation testLangTrans = term.getTranslation( testLang );
                //cerr << "NEW TERMKEY=" << term.getUid().toString() << " vocab=" << term.getVocabUid().toString() << endl;
                TermKey termKey( term.getUid(), term.getVocabUid(), firstLangTrans.getUid(), testLangTrans.getUid() );
                addTerm( termKey, ( poolCount - 1 ) - initQuizLength );
                allTerms.append( termKey );
                initTermCount++;
            }
        }
    }
}

void OriginalQuiz::addTerm( const TermKey& termKey, const int priority = 0 ) {
    termPool[ priority ].append( termKey );
}

void OriginalQuiz::discardCurrentTerm() {
    termPool[ currTermPool ].remove( currTerm );
}

bool OriginalQuiz::hasNextTerm() const {
    if( !standbyPool.isEmpty() )
        return( true );
   
    for( uint i = 0; i < poolCount; i++ ) {
        if( !termPool[ i ].isEmpty() )
            return( true );
    }

    return( false );
}

const TermKey OriginalQuiz::getCurrentTerm() const {
    return( currTerm );
}

const TermKey OriginalQuiz::getNextTerm() {
    if( standbyPool.count() == OriginalQuiz::standbyPoolSize )
        reintroduceStandbyTerm();
    
search:
    int startPool = ( currTermPool + 1 ) % OriginalQuiz::poolCount;
    int nextPool = startPool; 
    do {
        const int poolSize = termPool[ nextPool ].count();
        cerr << "poolSize="<< poolSize << endl;
        if( poolSize > 0 ) {
            const int index = rand() % poolSize;
            currTermPool = nextPool;
            currTerm = termPool[ nextPool ][ index ];
cerr << "currTerm = " << currTerm.getTermUid().toString() << endl;
            return( currTerm );
        }
        nextPool = ( nextPool + 1 ) % OriginalQuiz::poolCount; 
    } while( nextPool != startPool );
    if( standbyPool.count() > 0 ) {
        reintroduceStandbyTerm();
        goto search;
    }
    else {
        currTerm = TermKey();
        currTermPool = -1;
        return( currTerm );
    }
}

void OriginalQuiz::increaseTermPriority() {
    if( currTermPool > 0 ) {
        termPool[ currTermPool ].remove( currTerm );
        currTermPool--;
        termPool[ currTermPool ].append( currTerm );
    }
}

void OriginalQuiz::decreaseTermPriority() {
    if( currTermPool < (int)( OriginalQuiz::poolCount - 1 ) ) {
        termPool[ currTermPool ].remove( currTerm );
        currTermPool++;
        termPool[ currTermPool ].append( currTerm );
    }
}

void OriginalQuiz::gradeAnswer( int grade ) {
    if( grade == 0 )
        wrongAnswer();
    else
        rightAnswer();
}

void OriginalQuiz::rightAnswer() {
    //TermData termData = Statistics::instance()->getTermData( currTerm.getTermUid().toString() );
    BiUidKey key( currTerm.getFirstLanguageTranslationUid().toString(), currTerm.getTestLanguageTranslationUid().toString() );
    TermData termData = Statistics::instance()->getTermData( key );
    termData.successCount += 1;
    //Statistics::instance()->setTermData( currTerm.getTermUid().toString(), firstLang, testLang, termData ); 
    Statistics::instance()->setTermData( key, firstLang, testLang, termData ); 

    if( currTermPool == OriginalQuiz::poolCount - 1 )
        discardCurrentTerm();
    else {
        decreaseTermPriority();
        putCurrentTermOnStandby();
    }
}

void OriginalQuiz::wrongAnswer() {
    cerr << "wrongAnswer" << endl;
    //TermData termData = Statistics::instance()->getTermData( currTerm.getTermUid().toString() );
    BiUidKey key( currTerm.getFirstLanguageTranslationUid().toString(), currTerm.getTestLanguageTranslationUid().toString() );
    TermData termData = Statistics::instance()->getTermData( key );
    cerr << "miss=" << termData.missCount << endl;
    termData.missCount += 1;
    //Statistics::instance()->setTermData( currTerm.getTermUid().toString(), firstLang, testLang, termData ); 
    Statistics::instance()->setTermData( key, firstLang, testLang, termData ); 
    cerr << "ok" << endl;

    increaseTermPriority();
    putCurrentTermOnStandby();
}

int OriginalQuiz::getProgress() const {
    if( initTermCount == 0 )
        return( 0 );

    int currTermCount = 0;
    for( uint i = 0; i < poolCount; i++ )
        currTermCount += termPool[ i ].count();
    for( uint i = 0; i < standbyPool.count(); i++ )
        currTermCount += 1;
    return( initTermCount - currTermCount );
}

int OriginalQuiz::getAnswerCount() const {
    return( 2 );
}

void OriginalQuiz::putCurrentTermOnStandby() {
    StandbyTerm term( currTerm, currTermPool );
    standbyPool.prepend( term ); 
    termPool[ currTermPool ].remove( currTerm );
}

void OriginalQuiz::reintroduceStandbyTerm() {
    const StandbyTerm& reintroducedTerm = standbyPool.last();
    termPool[ reintroducedTerm.getPool() ].append( reintroducedTerm.getKey() ); 
    standbyPool.remove( reintroducedTerm );
}

QString OriginalQuiz::getFilename() const {
    BilingualKey key( firstLang, testLang );
    return( applDir + "/quiz_orig_" + key.toString() + ".dat.z" );
}

QDataStream& operator<<( QDataStream& out, const OriginalQuiz& quiz ) {
    out << quiz.firstLang << quiz.testLang;

    for( uint i = 0; i < OriginalQuiz::poolCount; i++ ) {
        for( QValueList<TermKey>::ConstIterator it = quiz.termPool[ i ].begin(); it != quiz.termPool[ i ].end(); it++ )
            out << *it;
        out << TermKey(); // To mark the end of the pool.
    }

    for( QValueList<StandbyTerm>::ConstIterator it = quiz.standbyPool.begin(); it != quiz.standbyPool.end(); it++ )
        out << *it;
    out << StandbyTerm( TermKey(), 0 ); // To mark the end of the standbyPool.
    
    out << quiz.allTerms; 
    out << quiz.initQuizLength << quiz.initTermCount << quiz.currTermPool << quiz.currTerm;

    return( out );
}

QDataStream& operator>>( QDataStream& in, OriginalQuiz& quiz ) {
    QString tempFirstLanguage;
    QString tempTestLanguage;
    QValueList<TermKey> tempTermPool[ OriginalQuiz::poolCount ];
    QValueList<StandbyTerm> tempStandbyPool;
    QValueList<TermKey> tempAllTerms;
    uint tempInitQuizLength;
    int tempInitTermCount;
    int tempCurrTermPool;
    TermKey tempCurrTerm;

    in >> tempFirstLanguage >> tempTestLanguage;
    for( uint i = 0; i < OriginalQuiz::poolCount; i++ ) {
        for( ;; ) {
            TermKey tempTermKey;
            in >> tempTermKey;
            if( tempTermKey.getTermUid().isNull() && tempTermKey.getVocabUid().isNull() )
                break;
            tempTermPool[ i ].append( tempTermKey );
        }
    }

    for( ;; ) {
        StandbyTerm tempStandbyTerm;
        in >> tempStandbyTerm;
        if( tempStandbyTerm.getPool() == 0 && tempStandbyTerm.getKey().getTermUid().isNull() && tempStandbyTerm.getKey().getVocabUid().isNull() )
            break;
        tempStandbyPool.append( tempStandbyTerm );
    }

    in >> tempAllTerms;
    in >> tempInitQuizLength >> tempInitTermCount >> tempCurrTermPool >> tempCurrTerm;

    quiz.firstLang = tempFirstLanguage;
    quiz.testLang = tempTestLanguage;
    quiz.termPool = tempTermPool;
    quiz.standbyPool = tempStandbyPool;
    quiz.allTerms = tempAllTerms;
    quiz.initQuizLength = tempInitQuizLength;
    quiz.initTermCount = tempInitTermCount;
    quiz.currTermPool = tempCurrTermPool;
    quiz.currTerm = tempCurrTerm;

    return( in );
}
