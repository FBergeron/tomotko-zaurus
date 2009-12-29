#ifndef ORIGINAL_QUIZ_H
#define ORIGINAL_QUIZ_H

#include <qvaluelist.h>
#include <iostream.h>
#include <stdlib.h>
#include <time.h>
#include "BilingualKey.h"
#include "Folder.h"
#include "Quiz.h"
#include "StandbyTerm.h"
#include "Statistics.h"
#include "TermKey.h"
#include "Vocabulary.h"

class OriginalQuiz : public Quiz {

    static const Q_UINT32 magicNumber( Q_UINT32( 0x22446677 ) );
    static const uint standbyPoolSize = 5;

public:

    static const uint poolCount  = 5;

    OriginalQuiz( const QString& applDir, const uint quizLength = 3 );
    OriginalQuiz( const OriginalQuiz& quiz );
    ~OriginalQuiz();
 
    const char* className() const { return( "OriginalQuiz" ); }

    bool isResumable() const;
    bool load();
    bool save();

    void conclude();

    bool isInProgress() const;
    void init( const QString& firstLanguage, const QString& testLanguage, Folder* rootFolder );
    void reinit();
    void discardCurrentTerm();
    bool hasNextTerm() const;
    const TermKey getCurrentTerm() const;
    const TermKey getNextTerm();
    void gradeAnswer( int grade );
    int getProgress() const;
    int getAnswerCount() const;

private:

    void initRec( const QString& firstLanguage, const QString& testLanguage, Folder* folder );
    void initRec( const QString& firstLanguage, const QString& testLanguage, Vocabulary* vocab );
    void putCurrentTermOnStandby();
    void reintroduceStandbyTerm();
    QString getFilename() const;
    void addTerm( const TermKey& termKey, const int priority = 0 );
    void increaseTermPriority();
    void decreaseTermPriority();
    void rightAnswer();
    void wrongAnswer();

    QValueList<TermKey>     termPool[ poolCount ];
    QValueList<StandbyTerm> standbyPool; 

    QValueList<TermKey>     allTerms;

    uint                    initQuizLength;
    int                     currTermPool;
    TermKey                 currTerm;

    friend QDataStream& operator<<( QDataStream& out, const OriginalQuiz& quiz );
    friend QDataStream& operator>>( QDataStream& in, OriginalQuiz& quiz );

};

#endif
