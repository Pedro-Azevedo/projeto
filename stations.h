#ifndef STATIONS_H_INCLUDED
#define STATIONS_H_INCLUDED

typedef struct
{
    int stationID;
    char terminal[7];
    char *station;
    char *municipal;
    double lat;
    double lng;
    int status;
} station_data;

typedef struct station
{
    station_data station_file;
    struct station *next;
} stationnode;


void stationlisting(void);
void routelisting(void);
void load_stationfile(stationnode**, FILE*);
stationnode* NewStationNode(station_data);
void InsertStationList(stationnode**, station_data);


#endif // STATIONS_H_INCLUDED
