#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//We include our header file
#include "trips.h"
#include "argtest.h"


/**
 * dataselection function: it allows the user to select some parameters about the travels to list
 * \param _all --> to decide to display all the travels or just some that verify one or more criteria
 * \param _begin --> one criterion the user can choose to verify (time when the travel starts)
 * \param _end --> one criterion the user can choose to verify (time when the travel ends)
 * \param _weekday -->one criterion the user can choose to verify (weekday of the travel)
 * \param _maxduration --> one criterion the user can choose to verify (max duration of the travel)
 * All this parameters are passed to the function by reference
 */

void dataselection(int* _all, ttime* _begin, ttime* _end, int* _weekday, int* _maxduration)
{
    printf("\nYou chose \"1. Data Selection\" mode\n");
    printf("Do you want to:\n\n");
    printf("\t1. Print the listings or statistics about all the travels\n");
    printf("\t2. Print the listings or statistics that obey to certain criteria\n");
    printf("\t3. LEAVE MENU\n\n");

    (*_all)=testchoice(3);

    //If the user chooses the second option, it will be sent to another menu to define the criteria wanted
    if((*_all)==2)
        criterion(_begin, _end, _weekday, _maxduration);

    //If it chooses option 1 or option 3 and when it returns from function "criterion" it will return to the main menu
}


/**
 * dataselection function: it allows the user to select some criteria about the travels he wants to list
 * \param _begin --> one criterion the user can choose to verify (time when the travel starts)
 * \param _end --> one criterion the user can choose to verify (time when the travel ends)
 * \param _weekday -->one criterion the user can choose to verify (weekday of the travel)
 * \param _maxduration --> one criterion the user can choose to verify (max duration of the travel)
 * All this parameters are passed to the function by reference
 */

void criterion (ttime* _begin, ttime* _end, int* _weekday, int* _maxduration)
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
            criterion1(_begin, _end);
            break;
        case 2:
            (*_weekday)=criterion2();
            break;
        case 3:
            (*_maxduration)=criterion3();
            break;
        case 4:
            ClearData(_begin, _end, _weekday, _maxduration);
            break;
        case 5:
            return; //brekas the infinite cycle
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
 * Both this parameters are passed to the function by reference
 */

void criterion1(ttime* _begin, ttime* _end)
{
    char buffer[BUFSIZE]= {'\0'}; //string to get the what the user types on the screen
    int validchoice=0; //test variable to evaluate what the user typed
    // do-while cycle so that the program asks the user while the option is invalid
    do
    {
        printf("Type by order the beginning and ending hour\n");
        fgets(buffer, BUFSIZE, stdin);
        validchoice=sscanf(buffer, "%d %d", &((*_begin).hour), &((*_end).hour));
        if((*_begin).hour >24 || (*_begin).hour<0 || (*_end).hour >24 || (*_end).hour<0)
        {
            printf("Invalid choice\n");
            validchoice=0;
        }
    }
    while(validchoice!=2);
}


/**
* criterion2 function: to evaluate the second criterion of the program (weekday of the travel)
* It simply returns an int to represent the chosen day of the week (monday=1, tuesday=2, wednesday=3, etc.)
*/

int criterion2(void)
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
        if(weekday>24 || weekday<1)
        {
            printf("Invalid choice\n");
            validchoice=0;
        }
    }
    while(validchoice!=1);

    return weekday;
}

/**
* criterion3 function: to evaluate the third criterion of the program (maximum duration of the travel)
* It simply returns an int that stores the maximum duration chosen
*/


int criterion3(void)
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

    return maxduration;
}


/**
* ClearData function: to clear all the data that was defined
* \param _begin --> one criterion the user can choose to verify (time when the travel starts)
* \param _end --> one criterion the user can choose to verify (time when the travel ends)
* \param _weekday -->one criterion the user can choose to verify (weekday of the travel)
* \param _maxduration --> one criterion the user can choose to verify (max duration of the travel)
*/

void ClearData(ttime* _begin, ttime* _end, int* _weekday, int* _maxduration)
{
    // The strategy is to set the parameters to invalid values
    // This will help us to define boundaries for cycle or conditions for if-clauses
    // when we0ll need to print only some travels
    _begin->hour=-1;
    _end->hour=-1;
    *_weekday=0;
    *_maxduration=-1;
}


void triplisting(void)
{

}

void stationlisting(void)
{

}

void routelisting(void)
{

}

void statisticslisting(void)
{

}

/**
 * Links the allocated node pointer to the read structure and doubly links it
 */
tripnode* create_triplist(trip_data struct_node, tripnode* prevnode)
{
    tripnode *nextnode=NULL;
    //allocating mem for the node
    prevnode = tripnode_alloc(prevnode,nextnode);
    prevnode->trip_file=struct_node;
    return prevnode;
}

/**
 * Links the allocated node pointer to the read structure and doubly links it
 */
stationnode* create_stationlist(station_data struct_node, stationnode* prevnode)
{
    stationnode *nextnode=NULL;
    //allocating mem for the node
    prevnode = stationnode_alloc(prevnode,nextnode);
    prevnode->station_file=struct_node;
    return prevnode;
}

/**
 *  Dinamically allocates memory for each node of the station list
 */
stationnode* stationnode_alloc(stationnode* prevnode, stationnode* nextnode)
{
    stationnode *stationnode_ptr=(stationnode*)malloc(sizeof(stationnode));
    if(stationnode_ptr==NULL)
    {
        printf("Error! It was not possible to allocate memory.");
        exit(EXIT_FAILURE);
    }
    else
    {
        stationnode_ptr->next=nextnode;
        stationnode_ptr->prev=prevnode;
    }
    return stationnode_ptr;
}

/**
 *  Dinamically allocates memory for each node of the trip list
 */
tripnode* tripnode_alloc(tripnode* prevnode, tripnode* nextnode)
{
    tripnode *tripnode_ptr=(tripnode*)malloc(sizeof(tripnode));
    if(tripnode_ptr==NULL)
    {
        printf("Error! It was not possible to allocate memory.");
        exit(EXIT_FAILURE);
    }
    else
    {
        tripnode_ptr->next=nextnode;
        tripnode_ptr->prev=prevnode;
    }
    return tripnode_ptr;
}
