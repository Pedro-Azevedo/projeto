#ifndef TRIPS_H_INCLUDED
#define TRIPS_H_INCLUDED
#define BUFSIZE 100
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
    char type[10];
    int birthyear;
    char gender[6];
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
    int startstationID;
    int stopstationID;
    station_data *start;
    station_data *stop;

} trip_data;

typedef struct trip
{
    trip_data trip_file;
    struct trip *prev;
    struct trip *next;
} tripnode;

void textmode(void);
void dataselection(int*,ttime*, ttime*, int*, int*);
void criterion(ttime*, ttime*, int*, int*);
int testchoice(int);
void criterion1(ttime*, ttime*);
int criterion2(void);
int criterion3(void);
void ClearData(ttime*, ttime*, int*, int*);
void statisticslisting(void);
void triplisting(void);

void tripfile_read(void);
void create_triplist(trip_data);
tripnode* tripnode_alloc(trip_data);
tripnode* triphead;
void fillstation_triplist();

#endif // TRIPS_H_INCLUDED
