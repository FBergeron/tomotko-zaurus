#ifndef PROGRESS_H
#define PROGRESS_H

#include <qdatetime.h>
#include <qmap.h>

/**
 * Record of statistics data for a term.
 */
struct TermData {

    TermData() : interval( 0 ), repetition( 0 ), easinessFactor( 2.5 ), nextRepetitionDate( QDate::currentDate() ), 
        lastRepetitionDate( QDate() ), successCount( 0 ), missCount( 0 ) {
    }

    TermData( const TermData& termData ) : interval( termData.interval ), repetition( termData.repetition ), easinessFactor( termData.easinessFactor ), 
        nextRepetitionDate( termData.nextRepetitionDate ), lastRepetitionDate( termData.lastRepetitionDate ), 
            successCount( termData.successCount ), missCount( termData.missCount ) {
    }

    int interval;
    uint repetition;
    float easinessFactor; // EF
    QDate nextRepetitionDate;
    QDate lastRepetitionDate;
    uint successCount;
    uint missCount;

};

/**
 * Record of statistics data for the current terms (either in the quiz or the selected one in Vocabulary Manager).
 */
struct CurrTermProgressData {

    CurrTermProgressData() {
        repetition = 0;
        easinessFactor = 0.0f;
        daysToNextRepetition = 0;
        daysToLastRepetition = 0;
        successCount = 0;
        missCount = 0;
    }

    CurrTermProgressData( const CurrTermProgressData& data ) 
        : repetition( data.repetition ), easinessFactor( data.easinessFactor ), daysToNextRepetition( data.daysToNextRepetition ),
            daysToLastRepetition( data.daysToLastRepetition ), successCount( data.successCount ), missCount( data.missCount ) {
    }

    int     repetition;
    float   easinessFactor;
    int     daysToNextRepetition;
    int     daysToLastRepetition;
    uint    successCount;
    uint    missCount;

};

/**
 * Number of days that we consider when building a schedule.
 */
const int scheduleLength = 28; // In days.

/**
 * Structure storing all the progress information.
 */
struct ProgressData {
    
    ProgressData() : efValueCount( 0 ), efAverage( 0.0f ), efStandardDeviation( 0.0f ), 
        successRateValueCount( 0 ), successRateAverage( 0.0f ), successRateStandardDeviation( 0.0f ) {
        for( int i = 0; i < scheduleLength; i++ )
            scheduleForDay[ i ] = 0;

        efDistribution = QMap<int,int>();
        successRateDistribution = QMap<int,int>();
    }

    ProgressData( const ProgressData& data ) {
        for( int i = 0; i < scheduleLength; i++ )
            scheduleForDay[ i ] = data.scheduleForDay[ i ];

        for( QMap<int,int>::ConstIterator it = data.efDistribution.begin(); it != data.efDistribution.end(); it++ ) {
            int key = it.key();
            int value = it.data();
            efDistribution.insert( key, value );
        }
        efValueCount = data.efValueCount;
        efAverage = data.efAverage;
        efStandardDeviation = data.efStandardDeviation;

        for( QMap<int,int>::ConstIterator it = data.successRateDistribution.begin(); it != data.successRateDistribution.end(); it++ ) {
            int key = it.key();
            int value = it.data();
            successRateDistribution.insert( key, value );
        }
        successRateValueCount = data.successRateValueCount;
        successRateAverage = data.successRateAverage;
        successRateStandardDeviation = data.successRateStandardDeviation;

        currTerm = data.currTerm;
    }

    int                     scheduleForDay[ scheduleLength ];

    QMap<int,int>           efDistribution;
    int                     efValueCount;
    float                   efAverage;
    float                   efStandardDeviation;

    QMap<int,int>           successRateDistribution;
    int                     successRateValueCount;
    float                   successRateAverage;
    float                   successRateStandardDeviation;

    CurrTermProgressData    currTerm;

};

#endif

