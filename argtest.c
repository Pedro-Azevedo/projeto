#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "argtest.h"

void arguments_test(int argc, char **argv)
{
    //Test the number of arguments(argc)
    if (argc != 4)
    {
        printf("Error: Invalid number of arguments.\n");
        exit(EXIT_FAILURE);
    }

    //Test if the mode entered is valid
    if(strcmp(argv[1],"-g")!=0 && strcmp(argv[1],"-t")!=0)
    {
        printf("Error: Not a valid mode\n");
        exit(EXIT_FAILURE);
    }


    //Test if the files entered are correct and in the correct positions
    if(strcmp(argv[2],"hubway_stations.csv")!=0 || strcmp(argv[3],"hubway_trips.csv")!=0 || strcmp(argv[2],"hubway_trips.csv")==0 || strcmp(argv[3],"hubway_stations.csv")==0)
    {
        printf("Error: Incompatible files entered\n");
        exit(EXIT_FAILURE);
    }

    //Test if the files exist or can be accessed
    for(int i=2; i<4; i++)
    {
        if(access(argv[i],F_OK)==-1)
        {
            printf("Error: The entered files do not exist or cannot be accessed\n");
            exit(EXIT_FAILURE);
        }
    }
    return;
}
