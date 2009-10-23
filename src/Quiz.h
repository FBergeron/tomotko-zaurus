#ifndef QUIZ_H
#define QUIZ_H

#include <qwidget.h>
#include "Base.h"
#include "Folder.h"
#include "TermKey.h"

class Quiz : public Base {

public:

    //Quiz() {} // Do we need a default constructor?
    Quiz( const QString& applDir ) { this->applDir = applDir; };
    Quiz( const Quiz& quiz );
    virtual ~Quiz() {};
 
    virtual bool isResumable() const = 0;
    virtual bool load() = 0;
    virtual bool save() = 0;

    virtual void conclude() = 0;

    virtual QString getFirstLanguage() const { return( firstLang ); };
    virtual QString getTestLanguage() const { return( testLang ); };
    virtual bool isInProgress() const = 0;
    virtual void init( const QString& firstLanguage, const QString& testLanguage, Folder* rootFolder ) = 0;
    virtual void reinit() = 0;
    virtual void discardCurrentTerm() = 0;
    virtual const TermKey getCurrentTerm() const = 0;
    virtual bool hasNextTerm() const = 0;
    virtual const TermKey getNextTerm() = 0;
    virtual void gradeAnswer( int grade ) = 0;
    virtual int getProgress() const = 0;
    virtual int getInitialTermCount() const { return( initTermCount ); };    
    virtual int getAnswerCount() const = 0;

    virtual float getCurrentTermEasinessFactor();
    virtual int getCurrentTermRepetition();
    virtual int getCurrentTermNextRepetition();

protected:

    QString firstLang;
    QString testLang;
    Folder* rootFolder;

    int     initTermCount;

    QString applDir;

//    friend QDataStream& operator<<( QDataStream& out, const Quiz& quiz );
//    friend QDataStream& operator>>( QDataStream& in, Quiz& quiz );

};

#endif
