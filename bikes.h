#ifndef BIKES_H_INCLUDED
#define BIKES_H_INCLUDED

#define MAX 10


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


#endif // BIKES_H_INCLUDED
