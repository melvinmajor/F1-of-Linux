#ifndef RACESTEP_H
#define RACESTEP_H

// struct containing informations about a formula 1 step
typedef struct RaceStep {
    int allowed;

    int lap;

    int stand;
    int withdrawal;

    int time[48][3];

    int done;
} RaceStep;


#endif //RACESTEP_H
