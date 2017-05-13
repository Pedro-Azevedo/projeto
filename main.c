#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//We include our header file
#include "trips.h"
#include "argtest.h"

int main(int argc, char*argv[])
{
    arguments_test(argc, argv);
    
    ///lists are created and the pointers to the last items are assigned to triptail and stationtail
    triptail=tripfile_read();
    stationtail=stationfile_read();
    
    printf("%d %d", triptail->prev->trip_file.timespan, stationtail->prev->station_file.stationID); //para testar

    if(strcmp(argv[1], "-t")==0)
        textmode();

    return EXIT_SUCCESS;
}
