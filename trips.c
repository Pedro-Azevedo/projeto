#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//We include our header file
#include "trips.h"
#include "argtest.h"
#include "listings.h"


/**
 * dataselection function: it allows the user to select some parameters about the travels to list
 * \param _all --> to decide to display all the travels or just some that verify one or more criteria
 * \param _begin --> one criterion the user can choose to verify (time when the travel starts)
 * \param _end --> one criterion the user can choose to verify (time when the travel ends)
 * \param _weekday -->one criterion the user can choose to verify (weekday of the travel)
 * \param _maxduration --> one criterion the user can choose to verify (max duration of the travel)
 * \param _triplist --> the list of trips
 * \param _stationlist --> the list of stations
 * \param argv --> the parameters of main
 * All this parameters are passed to the function by reference
 */

void dataselection(int* _all, ttime* _begin, ttime* _end, int* _weekday, int* _maxduration, tripnode** _triplist, stationnode** _stationlist, char** argv)
{
    printf("\nYou chose \"1. Data Selection\" mode\n");
    printf("Do you want to:\n\n");
    printf("\t1. Print the listings or statistics about all the travels\n");
    printf("\t2. Print the listings or statistics that obey to certain criteria\n");
    printf("\t3. LEAVE MENU\n\n");

    (*_all)=testchoice(3);

    //If the user chooses the second option, it will be sent to another menu to define the criteria wanted
    if((*_all)==2)
    {
        system("clear");
        criterion(_begin, _end, _weekday, _maxduration, _triplist, _stationlist, argv);
    }


    //If it chooses option 1 or option 3 and when it returns from function "criterion" it will return to the main menu
}


/**
 * dataselection function: it allows the user to select some criteria about the travels he wants to list
 * \param _begin --> one criterion the user can choose to verify (time when the travel starts)
 * \param _end --> one criterion the user can choose to verify (time when the travel ends)
 * \param _weekday -->one criterion the user can choose to verify (weekday of the travel)
 * \param _maxduration --> one criterion the user can choose to verify (max duration of the travel)
 * \param _triplist --> the list of trips
 * \param _stationlist --> the list of stations
 * \param argv --> the parameters of main
 * All this parameters are passed to the function by reference
 */

void criterion (ttime* _begin, ttime* _end, int* _weekday, int* _maxduration, tripnode** _triplist, stationnode** _stationlist, char** argv)
{
    int userchoice=0; //variable that stores the user choice from the menu

    //We create an infinite cycle so that the user returns to this menu after defining a criterion
    while(1)
    {
        printf("Now we will need you to choose some criteria for printing the travels.\n");
        printf("\t1. Begin and end hour\n");
        printf("\t2. Day of the week\n");
        printf("\t3. Maximum duration\n");
        printf("\t4. CLEAR ALL PREVIOUS DATA\n");
        printf("\t5. LEAVE MENU\n\n");

        userchoice=testchoice(5);

        switch(userchoice)
        {
        case 1:
            system("clear");
            criterion1(_begin, _end, _triplist);
            system("clear");
            break;
        case 2:
            system("clear");
            (*_weekday)=criterion2(_triplist);
            system("clear");
            break;
        case 3:
            system("clear");
            (*_maxduration)=criterion3(_triplist);
            system("clear");
            break;
        case 4:
            system("clear");
            ClearData(argv, _triplist, _stationlist, 0);
            system("clear");
            break;
        case 5:
            system("clear");
            return; //breks the infinite cycle
        }
    }
}

/**
 * testchoice function: function to allow the user to choose an option from the various menus
 * \param _max --> maximum value of the range of options the user has
 * \return the choice of the user
 */


int testchoice(int _max)
{
    int userchoice=0; //variable to return
    char buffer[BUFSIZE]= {'\0'}; //string to get the what the user types on the screen
    int testchoice=0; //test variable to evaluate what the user typed
    // do-while cycle so that the program asks the user while the option is invalid
    do
    {
        printf("Please type the integer corresponding to your choice:\n");
        fgets(buffer, BUFSIZE, stdin);
        testchoice=sscanf(buffer, "%d", &userchoice);
        if(userchoice>_max || userchoice <1)
        {
            printf("Invalid choice\n");
            testchoice=0;
        }
    }
    while(testchoice!=1);

    return userchoice;
}


/**
 * criterion1 function: to evaluate the first criterion of the program (begin/end time of the travel)
 * \param _begin --> one criterion the user can choose to verify (time when the travel starts)
 * \param _end --> one criterion the user can choose to verify (time when the travel ends)
 * \param triplist --> the list of trips
 * Both this parameters are passed to the function by reference
 */

void criterion1(ttime* _begin, ttime* _end, tripnode** _triplist)
{
    char buffer[BUFSIZE]= {'\0'}; //string to get the what the user types on the screen
    int validchoice=0; //test variable to evaluate what the user typed
    // do-while cycle so that the program asks the user while the option is invalid
    do
    {
        printf("Type by order the beginning hour and ending hour\n");
        fgets(buffer, BUFSIZE, stdin);
        validchoice=sscanf(buffer, "%d %d", &((*_begin).hour), &((*_end).hour));
        if((*_begin).hour >23 || (*_begin).hour<0 || (*_end).hour >23 || (*_end).hour<0)
        {
            printf("Invalid choice\n");
            validchoice=0;
        }
    }
    while(validchoice!=2);

    //Remove the undesirable elements of the list
    *_triplist=RemoveUsingHour(_begin,_end,*_triplist);
}


/**
* criterion2 function: to evaluate the second criterion of the program (weekday of the travel)
* \param triplist --> the list of trips (passed by reference)
* It simply returns an int to represent the chosen day of the week (monday=1, tuesday=2, wednesday=3, etc.)
*/

int criterion2(tripnode** _triplist)
{
    char buffer[BUFSIZE]= {'\0'}; //string to get the what the user types on the screen
    int validchoice=0; //test variable to evaluate what the user typed
    int weekday=0; //variable to return
    // do-while cycle so that the program asks the user while the option is invalid
    do
    {
        printf("Type the integer corresponding the day of the week (1=monday;...7=sunday)\n");
        fgets(buffer, BUFSIZE, stdin);
        validchoice=sscanf(buffer, "%d", &weekday);
        if(weekday>7 || weekday<1)
        {
            printf("Invalid choice\n");
            validchoice=0;
        }
    }
    while(validchoice!=1);

    //Remove the undesirable elements of the list
    *_triplist=RemoveUsingWeekday(weekday,*_triplist);

    return weekday;
}

/**
* criterion3 function: to evaluate the third criterion of the program (maximum duration of the travel)
* \param triplist --> the list of trips (passed by reference)
* It simply returns an int that stores the maximum duration chosen
*/


int criterion3(tripnode** _triplist)
{
    char buffer[BUFSIZE]= {'\0'}; //string to get the what the user types on the screen
    int validchoice=0; //test variable to evaluate what the user typed
    int maxduration=0; //variable to return
    // do-while cycle so that the program asks the user while the option is invalid
    do
    {
        validchoice=0;
        printf("Type the maximum duration (in seconds)\n");
        fgets(buffer, BUFSIZE, stdin);
        validchoice=sscanf(buffer, "%d", &maxduration);
        if(maxduration<0)
        {
            printf("Invalid choice\n");
            validchoice=0;
        }
    }
    while(validchoice!=1);

    //Remove the undesirable elements of the list
    *_triplist=RemoveUsingMaxduration(maxduration,*_triplist);

    return maxduration;
}


/**
* ClearData function: to clear all the data that was defined
* \param argv --> parameters of main function
* \param _triplist --> the list of trips (passed by reference)
* \param _stationlist --> the list of stations (passed by reference)
*/

void ClearData(char** argv, tripnode** _triplist, stationnode** _stationlist, int _ending)
{
    //We start by clearing the lists

    //Free the lists memory
    tripnode* aux1;
    tripnode* erase1;
    aux1=*_triplist;
    while(aux1!=NULL)
    {
        erase1=aux1;
        aux1=aux1->next;
        free(erase1);
    }

    stationnode* aux2;
    stationnode* erase2;
    aux2=*_stationlist;
    while(aux2!=NULL)
    {
        erase2=aux2;
        aux2=aux2->next;
        free(erase2);
    }

    *_stationlist=NULL;
    *_triplist=NULL;

    //Then we load them again if we're not ending the program
    if(_ending==0)
        load_fromfiles(argv, _triplist, _stationlist);

}
