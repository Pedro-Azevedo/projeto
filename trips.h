#ifndef TRIPS_H_INCLUDED
#define TRIPS_H_INCLUDED
#define BUFSIZE 1000
#include "stations.h"

typedef struct
{
    int hour;
    int minute;
} ttime;


typedef struct
{
    int day;
    int month;
    int year;
} ddate;


typedef struct
{
    int type;
    int birthyear;
    int gender;
} person;


typedef struct
{
    int tripID;
    int timespan;
    ttime timebegin;
    ttime timeend;
    ddate datebegin;
    ddate dateend;
    char bikeID[7];
    person user;
    int stationstartID;
    int stationstopID;

} trip_data;

typedef struct trip
{
    trip_data trip_file;
    struct trip *next;
} tripnode;


void textmode(char**, tripnode**, stationnode**);
void dataselection(int*,ttime*, ttime*, int*, int*, tripnode**, stationnode**, char**);
void criterion(ttime*, ttime*, int*, int*, tripnode**, stationnode**, char**);
int testchoice(int);
void criterion1(ttime*, ttime*, tripnode**);
int criterion2(tripnode**);
int criterion3(tripnode**);
void ClearData(char**, tripnode**, stationnode**);
void statisticslisting(void);
void triplisting(tripnode**);
void load_fromfiles(char**, tripnode**, stationnode**);
void load_tripfile(tripnode**, FILE*);
tripnode* NewTripNode(trip_data);
void InsertTripList(tripnode**, trip_data);
tripnode* RemoveUsingHour(ttime*, ttime*, tripnode*);
tripnode* RemoveUsingWeekday(int, tripnode*);
tripnode* RemoveUsingMaxduration(int, tripnode*);
int ConvertDate(int, int, int);
int LeapYear(int);

#endif // TRIPS_H_INCLUDED
