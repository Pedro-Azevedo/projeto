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
    ///lists are created (the pointers to their heads are stationhead and triphead)
    tripfile_read();
    stationfile_read();
    ///stations data are assigned to respective trips
    fillstation_triplist();
    
    if(strcmp(argv[1], "-t")==0)
        textmode();

    return EXIT_SUCCESS;
}
