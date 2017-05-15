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

void stationlisting(void);
void routelisting(void);

stationnode* stationhead;
void stationfile_read(void);
void create_stationlist(station_data);
stationnode* stationnode_alloc(station_data);

#endif // STATIONS_H_INCLUDED
