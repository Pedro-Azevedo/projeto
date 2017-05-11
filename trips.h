#ifndef TRIPS_H_INCLUDED
#define TRIPS_H_INCLUDED

#include "stations.h"

typedef struct{
    int hour;
    int minute;
}ttime;


typedef struct{
    int day;
    int month;
    int year;
}ddate;


typedef struct{
    int type;
    int birthyear;
    int gender;
}person;



typedef struct{
    int tripID;
    int timespan;
    ttime timebegin;
    ttime timeend;
    ddate datebegin;
    ddate dateend;
    char bikeID[7];
    person user;
    station_data *start;
    station_data *stop;

}trip_data;

void textmode(void);
void dataselection(int*,ttime*, ttime*, int*, int*);
void criterion(ttime*, ttime*, int*, int*);
int testchoice(int);
void criterion1(ttime*, ttime*);
int criterion2(void);
int criterion3(void);
void ClearData(ttime*, ttime*, int*, int*);
void triplisting(void);
void statisticslisting(void);

#endif // TRIPS_H_INCLUDED
