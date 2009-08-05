#ifndef PROGRESS_H
#define PROGRESS_H

struct CurrTermProgressData {
    int     repetition;
    float   easinessFactor;
    int     daysToNextRepetition;
};

const int scheduleLength = 7; // In days.

struct ProgressData {
    int                  scheduleForDay[ scheduleLength ];
    CurrTermProgressData currTerm;
};

#endif

