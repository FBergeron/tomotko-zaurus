#ifndef SUPERMEMO2_QUIZ_H
#define SUPERMEMO2_QUIZ_H

#include <qdatetime.h>
#include <qvaluelist.h>
#include <iostream.h>
#include <stdlib.h>
#include <time.h>
#include "BilingualKey.h"
#include "Folder.h"
#include "Quiz.h"
#include "TermKey.h"
#include "Vocabulary.h"
#include "Progress.h"
#include "ProgressDialog.h"
#include "Statistics.h"

class SuperMemo2Quiz : public Quiz {

public:

    SuperMemo2Quiz( const QString& applDir );
    SuperMemo2Quiz( const SuperMemo2Quiz& quiz );
    ~SuperMemo2Quiz();
 
    const char* className() const { return( "SuperMemo2Quiz" ); }

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
    void wrongAnswer();
    int getProgress() const;
    int getAnswerCount() const;

    float getCurrentTermEasinessFactor();
    int getCurrentTermRepetition();
    int getCurrentTermNextRepetition();

private:

    void initRec( const QString& firstLanguage, const QString& testLanguage, Folder* folder );
    void initRec( const QString& firstLanguage, const QString& testLanguage, Vocabulary* vocab );
    void shuffleTerms();


    int getNextInterval( int interval, float easinessFactor, int repetition ) const;

    QValueList<TermKey>     terms;
    QValueList<TermKey>     termsToRemove;

    uint                    currTermIndex;

};

#endif
