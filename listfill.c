#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trips.h"

/**
 *  Reads values from trip file and saves them in a list of structures.
 *  Returns a pointer do the list tail
 */
void tripfile_read()
{
    char string1[BUFSIZE], string2[BUFSIZE], string3[BUFSIZE];
    int testread, seconds;
    trip_data tripread;
    FILE *tripsfptr;
    tripsfptr=fopen( "hubway_trips_v2.csv", "r");
    if (tripsfptr)
    {
        do
        {
            testread=fscanf(tripsfptr,"%s %s %s",string1,string2,string3);
            if(testread==3)
            {
                sscanf(string1,"%d,%d,%d/%d/%d", &tripread.tripID, &tripread.timespan, &tripread.datebegin.month,
                       &tripread.datebegin.day, &tripread.datebegin.year);

                sscanf(string2,"%d:%d:%d,%d,%d/%d/%d", &tripread.timebegin.hour, &tripread.timebegin.minute, &seconds,
                       &tripread.startstationID,&tripread.dateend.month,&tripread.dateend.day,&tripread.dateend.year);

                sscanf(string3,"%d:%d:%d,%d,%[^,],%[^,],%d,%s", &tripread.timeend.hour, &tripread.timeend.minute, &seconds,
                       &tripread.stopstationID, tripread.bikeID, tripread.user.type, &tripread.user.birthyear, tripread.user.gender);

               create_triplist(tripread);
            }
        }
        while(testread==3);
        fclose(tripsfptr);
    }
    return;
}

/**
 *  Reads station from stations file and saves them in a structure and returns it
 */
void stationfile_read()
{
    int testread;
    station_data stationread;

    FILE *stationsfptr;
    stationsfptr=fopen( "hubway_stations.csv", "r");
    if (stationsfptr)
    {
        do
        {
            testread=fscanf(stationsfptr,"%d,%[^,],%[^,],%[^,],%lf,%lf,%s", &stationread.stationID, stationread.terminal, stationread.station,
                            stationread.municipal, &stationread.lat, &stationread.lng, stationread.status);
            if(testread==7)
            {
                create_stationlist(stationread);
            }

        }
        while(testread==7);
        fclose(stationsfptr);
    }
    return;
}

/**
 *  Fills each trip structure station start and stop pointer with the pointers that match the correspondent structures
 */
void fillstation_triplist()
{
    tripnode* temptrip=triphead;
    stationnode* tempstation=stationhead;

    while(temptrip!=NULL){
        while(tempstation!=NULL)
        {
            if(temptrip->trip_file.startstationID==tempstation->station_file.stationID)
            {
                temptrip->trip_file.start=&(tempstation->station_file);
            }
            if(temptrip->trip_file.stopstationID==tempstation->station_file.stationID)
            {
                temptrip->trip_file.stop=&(tempstation->station_file);
            }
            tempstation=tempstation->next;
        }
        temptrip=temptrip->next;
        tempstation=stationhead;
    }
}
