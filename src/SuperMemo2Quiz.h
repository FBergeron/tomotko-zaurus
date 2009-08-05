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

class SuperMemo2Quiz : public Quiz {

    static const Q_UINT32 magicNumber( Q_UINT32( 0x22446680 ) );

public:

    SuperMemo2Quiz( const QString& applDir );
    SuperMemo2Quiz( const SuperMemo2Quiz& quiz );
    ~SuperMemo2Quiz();
 
    const char* className() const { return( "SuperMemo2Quiz" ); }

    struct TermData {
        int interval;
        uint repetition;
        float easinessFactor; // EF
        QDate nextRepetitionDate;
    };

    bool isResumable() const;
    bool load();
    bool save();

    void conclude();

    bool isInProgress() const;
    void init( const QString& firstLanguage, const QString& testLanguage, Folder* rootFolder );
    void getSchedule( Folder* rootFolder, int* schedule );
    void reinit();
    void discardCurrentTerm();
    bool hasNextTerm() const;
    const TermKey getCurrentTerm() const;
    const TermKey getNextTerm();
    void gradeAnswer( int grade );
    void wrongAnswer();
    int getProgress() const;
    int getAnswerCount() const;

private:

    void initRec( const QString& firstLanguage, const QString& testLanguage, Folder* folder );
    void initRec( const QString& firstLanguage, const QString& testLanguage, Vocabulary* vocab );
    void getScheduleRec( Folder* folder, int* schedule );
    void getScheduleRec( Vocabulary* vocab, int* schedule );
    void shuffleTerms();

    QString getTermDataFilename() const;

    TermData getTermData( const QString& termUid );
    void setTermData( const QString& termUid, const TermData& termData );

    bool loadTermData();
    bool saveTermData() const;

    QValueList<TermKey>     terms;
    QValueList<TermKey>     termsToRemove;

    uint                    currTermIndex;

    QMap<QString, TermData> allTermData;

};

#endif
