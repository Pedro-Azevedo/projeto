#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//We include our header files
#include "trips.h"
#include "argtest.h"

int main(int argc, char*argv[])
{
    //Test the arguments entered
    arguments_test(argc, argv);

    //Define the pointers to our lists
    tripnode* triplist=NULL;
    stationnode* stationlist=NULL;

    //Load the files into the lists
    printf("Loading the files...\n");
    load_fromfiles(argv, &triplist, &stationlist);

    //Start the text mode
    if(strcmp(argv[1], "-t")==0)
        textmode(argv, &triplist, &stationlist);

    //Free the lists memory
    tripnode* aux1;
    aux1=triplist;
    while(aux1!=NULL)
    {
        aux1=aux1->next;
        free(aux1);
    }

    stationnode* aux2;
    aux2=stationlist;
    while(aux2!=NULL)
    {
        aux2=aux2->next;
        free(aux2);
    }

    return EXIT_SUCCESS;
}


