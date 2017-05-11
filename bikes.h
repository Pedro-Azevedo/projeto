#ifndef BIKES_H_INCLUDED
#define BIKES_H_INCLUDED


//Definition of some constants(macros). This is basically the size of some strings.
#define MAX 10
#define BUFSIZE 1000

//Definition of the data structures needed

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
    int stationID;
    char terminal[MAX];
    char* station;
    char municipal[MAX];
    double lat;
    double lng;
    int status;
}station_data;

typedef struct{
    int travelID;
    int timespan;
    ttime timebegin;
    ttime timeend;
    ddate datebegin;
    ddate dateend;
    char bikeID[MAX];
    person user;
    station_data *start;
    station_data *stop;

}travel_data;


//Definition of all the functions needed
void textmode(void);
void dataselection(int*,ttime*, ttime*, int*, int*);
void criterion(ttime*, ttime*, int*, int*);
int testchoice(int);
void criterion1(ttime*, ttime*);
int criterion2(void);
int criterion3(void);
void ClearData(ttime*, ttime*, int*, int*);
void graphicmode(void);
void travellisting(void);
void stationlisting(void);
void routelisting(void);
void statisticslisting(void);


#endif // BIKES_H_INCLUDED
