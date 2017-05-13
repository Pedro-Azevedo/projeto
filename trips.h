#ifndef STATIONS_H_INCLUDED
#define STATIONS_H_INCLUDED

typedef struct
{
    int stationID;
    char terminal[7];
    char station[250];
    char municipal[10];
    double lat;
    double lng;
    char status[10];
} station_data;

typedef struct station
{
    station_data station_file;
    struct station *prev;
    struct station *next;
} stationnode;

stationnode* stationtail;

void stationlisting(void);
void routelisting(void);

stationnode* stationfile_read(void);
stationnode* create_stationlist(station_data,stationnode*);
stationnode* stationnode_alloc(stationnode*,stationnode*);


#endif // STATIONS_H_INCLUDED

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
