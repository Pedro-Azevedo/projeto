#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trips.h"

/**
 *  Reads values from trip file and saves them in a list of structures.
 *  Returns a pointer do the list tail
 */
tripnode* tripfile_read()
{
    char string1[BUFSIZE], string2[BUFSIZE], string3[BUFSIZE];
    int testread, seconds;
    trip_data tripread;
    tripnode* prev_node=NULL;
    FILE *tripsfptr;
    tripsfptr=fopen( "hubway_trips_v2.csv", "r");
    if (tripsfptr)
    {
        do
        {
            testread=fscanf(tripsfptr,"%s %s %s",string1,string2,string3);
            if(testread==3)
            {
                sscanf(string1,"%d,%d,%d/%d/%d", &tripread.tripID, &tripread.timespan, &tripread.datebegin.day,
                       &tripread.datebegin.month, &tripread.datebegin.year);

                sscanf(string2,"%d:%d:%d,%d,%d/%d/%d", &tripread.timebegin.hour, &tripread.timebegin.minute, &seconds,
                       &tripread.startstationID,&tripread.dateend.day,&tripread.dateend.month,&tripread.dateend.year);

                sscanf(string3,"%d:%d:%d,%d,%[^,],%[^,],%d,%s", &tripread.timeend.hour, &tripread.timeend.minute, &seconds,
                       &tripread.stopstationID, tripread.bikeID, tripread.user.type, &tripread.user.birthyear, tripread.user.gender);

                prev_node=create_triplist(tripread,prev_node);
            }
        }
        while(testread==3);
        fclose(tripsfptr);
    }
    return prev_node;
}

/**
 *  Reads station from stations file and saves them in a structure and returns it
 */
stationnode* stationfile_read()
{
    int testread;
    station_data stationread;
    stationnode* prev_node=NULL;

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
                prev_node=create_stationlist(stationread,prev_node);
            }

        }
        while(testread==7);
        fclose(stationsfptr);
    }
    return prev_node;
}

/**
 *  Fills each trip structure station start and stop pointer with the pointers that match the correspondent structures
 */
void fillstation_triplist(tripnode* lasttrip,stationnode* laststation)
{
    tripnode* curtrip;
    stationnode* curstation;
    int startstation_done, stopstation_done;

    ///goes through the trip list from tail to head
    for(curtrip=lasttrip; curtrip!=NULL; curtrip=curtrip->prev)
    {
        startstation_done=stopstation_done=0;
        ///goes through the station list from tail to head until both start and stop stations are found
        for(curstation=laststation; curstation!=NULL && (startstation_done!=1  || stopstation_done!=1); curstation=curstation->prev)
        {
            ///if the current station has the same ID as the starting one a pointer to its structure is assigned
            if(curstation->station_file.stationID==curtrip->trip_file.startstationID)
            {
                curtrip->trip_file.start=&(curstation->station_file);
                startstation_done=1;
            }
            ///if the current station has the same ID as the stopping one a pointer to its structure is assigned
            if(curstation->station_file.stationID==curtrip->trip_file.stopstationID)
            {
                curtrip->trip_file.stop=&(curstation->station_file);
                stopstation_done=1;
            }
        }
    }
}
