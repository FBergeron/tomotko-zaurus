#ifndef PROGRESS_H
#define PROGRESS_H

#include <qdatetime.h>
#include <qmap.h>

struct TermData {

    TermData() : interval( 0 ), repetition( 0 ), easinessFactor( 2.5 ), nextRepetitionDate( QDate::currentDate() ) {
    }

    TermData( const TermData& termData ) : interval( termData.interval ), repetition( termData.repetition ), easinessFactor( termData.easinessFactor ), nextRepetitionDate( termData.nextRepetitionDate ) {
    }

    int interval;
    uint repetition;
    float easinessFactor; // EF
    QDate nextRepetitionDate;
};

struct CurrTermProgressData {

    CurrTermProgressData() {
        repetition = 0;
        easinessFactor = 0.0f;
        daysToNextRepetition = 0;
    }

    CurrTermProgressData( const CurrTermProgressData& data ) 
        : repetition( data.repetition ), easinessFactor( data.easinessFactor ), daysToNextRepetition( data.daysToNextRepetition ) {
    }

    int     repetition;
    float   easinessFactor;
    int     daysToNextRepetition;
};

const int scheduleLength = 28; // In days.

struct ProgressData {
    
    ProgressData() : averageEF( 0.0f ), efStandardDeviation( 0.0f ) {
        for( int i = 0; i < scheduleLength; i++ )
            scheduleForDay[ i ] = 0;

        efDistribution = QMap<int,int>();
    }

    ProgressData( const ProgressData& data ) {
        for( int i = 0; i < scheduleLength; i++ )
            scheduleForDay[ i ] = data.scheduleForDay[ i ];
        for( QMap<int,int>::ConstIterator it = data.efDistribution.begin(); it != data.efDistribution.end(); it++ ) {
            int key = it.key();
            int value = it.data();
            efDistribution.insert( key, value );
        }
        currTerm = data.currTerm;
        averageEF = data.averageEF;
        efStandardDeviation = data.efStandardDeviation;
    }

    int                     scheduleForDay[ scheduleLength ];
    QMap<int,int>           efDistribution;
    CurrTermProgressData    currTerm;
    float                   averageEF;
    float                   efStandardDeviation;

};

#endif

