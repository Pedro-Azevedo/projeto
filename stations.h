#ifndef STATIONS_H_INCLUDED
#define STATIONS_H_INCLUDED
typedef struct{
    int stationID;
    char terminal[7];
    char* station;
    char* municipal;
    double lat;
    double lng;
    int status;
}station_data;

void stationlisting(void);
void routelisting(void);


#endif // STATIONS_H_INCLUDED
