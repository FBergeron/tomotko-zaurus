#ifndef PROGRESS_H
#define PROGRESS_H

struct CurrTermProgressData {

    CurrTermProgressData() {
        repetition = 0;
        easinessFactor = 0.0f;
        daysToNextRepetition = 0;
    }

    int     repetition;
    float   easinessFactor;
    int     daysToNextRepetition;
};

const int scheduleLength = 28; // In days.

struct ProgressData {
    
    ProgressData() {
        for( int i = 0; i < scheduleLength; i++ )
            scheduleForDay[ i ] = 0;
    }

    int                  scheduleForDay[ scheduleLength ];
    CurrTermProgressData currTerm;
};

#endif

