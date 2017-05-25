#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//We include our header file
#include "trips.h"
#include "argtest.h"
#include "listings.h"

/**
 * Text mode function
 * It shows us the menu with options for Data Selection or various Listings
 */

void textmode(char** argv, tripnode** _triplist, stationnode** _stationlist)
{
    int userchoice=0; //the choice from the given menu
    int all=0;  // parameter that will serve to check if the user wants to see all the travels or just
                // some that verify one or more criteria
    int maxduration=0; //one criterion the user can choose to verify (max duration of the travel)
    int weekday=0; //one criterion the user can choose to verify (weekday of the travel)
    ttime begin; // one criterion the user can choose to verify (time when the travel starts)
    ttime end; //one criterion the user can choose to verify (time when the travel ends)

    printf("\t\t\tWelcome to BikeStation program!\n\n");
    printf("You chose text mode.\n");

    // We set an infinite cycle to show us the menu so that everytime the user returns from
    // the various options functions, it will still display the menu for him to choose
    while(1)
    {
        printf("Consider the following options\n\n");
        printf("\t1. Data Selection\n");
        printf("\t2. Travel Listing\n");
        printf("\t3. Station Listing\n");
        printf("\t4. Route Listing\n");
        printf("\t5. Statistics Listing\n");
        printf("\t6. LEAVE MENU\n\n");

        userchoice=testchoice(6);

        switch(userchoice)
        {
            case 1:
                system("clear");
                dataselection(&all, &begin, &end, &maxduration, &weekday, _triplist, _stationlist, argv);
                system("clear");
                break;
            case 2:
                system("clear");
                triplisting(_triplist);
                system("clear");
                break;
            case 3:
                system("clear");
                stationlisting(_triplist,_stationlist);
                break;
            case 4:
                system("clear");
                routelisting(_triplist);
                system("clear");
                break;
            case 5:
                system("clear");
                statisticslisting(_triplist);
                system("clear");
                break;
            case 6:
                system("clear");
                return;//breaks the infinite cycle
        }
    }
    return;
}
