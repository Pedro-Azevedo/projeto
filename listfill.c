#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trips.h"
#include "listings.h"

/**
* ĺoad_fromfiles function: to load the files into the lists
* \param argv --> parameters of main function
* \param _triplist --> the list of trips (passed by reference)
* \param _stationlist --> the list of stations (passed by reference)
*/

void load_fromfiles(char* argv[], tripnode** _triplist, stationnode** _stationlist)
{
    FILE* tripsfptr=NULL; //pointer to the file of trips
    FILE* stationsfptr=NULL; //pointer to the file of stations

    //open the files and verify if all went well
    stationsfptr=fopen(argv[2], "r");
    tripsfptr=fopen(argv[3], "r");
    if(tripsfptr==NULL || stationsfptr==NULL)
    {
        printf("Error: File could not be opened\n");
        exit(EXIT_FAILURE);
    }

    load_tripfile(_triplist, tripsfptr);
    load_stationfile(_stationlist, stationsfptr);
    fill_tripstations(_triplist, _stationlist);

    //Close the files
    fclose(tripsfptr);
    fclose(stationsfptr);
}


/**
* ĺoad_tripfile function: to create the list of trips
* \param _triplist --> the list of trips (passed by reference)
* \param _fp --> the file
*/

void load_tripfile(tripnode** _triplist, FILE* _fp)
{
    trip_data triptoread; //data for each node of the list
    char separator[]=",/:"; //separator for strtok
    char buffer[BUFSIZE]={'\0'}; // string to save the lines of the file
    char string1[BUFSIZE]={'\0'}; // auxiliar string to break buffer
    char string2[BUFSIZE]={'\0'}; // auxiliar string to break buffer
    char string3[BUFSIZE]={'\0'}; // auxiliar string to break buffer
    char* token=NULL; //auxiliar pointer to apply strtok
    char* help=NULL; //auxiliar pointer to apply strtol

    while(fgets(buffer, BUFSIZE, _fp)!=NULL)
    {
        //Break the string
        sscanf(buffer, "%s %s %s", string1, string2, string3);
        token=strtok(string1, separator);
        triptoread.tripID= strtol(token,&help,10);
        token=strtok(NULL, separator);
        triptoread.timespan= strtol(token,&help,10);
        token=strtok(NULL, separator);
        triptoread.datebegin.month=strtol(token,&help,10);
        token=strtok(NULL, separator);
        triptoread.datebegin.day=strtol(token,&help,10);
        token=strtok(NULL, separator);
        triptoread.datebegin.year=strtol(token,&help,10);
        token=strtok(string2, separator);
        triptoread.timebegin.hour=strtol(token,&help,10);
        token=strtok(NULL, separator);
        triptoread.timebegin.minute=strtol(token,&help,10);
        token=strtok(NULL, separator);
        token=strtok(NULL, separator);
        triptoread.stationstartID=strtol(token,&help,10);
        token=strtok(NULL, separator);
        triptoread.dateend.month=strtol(token,&help,10);
        token=strtok(NULL, separator);
        triptoread.dateend.day=strtol(token,&help,10);
        token=strtok(NULL, separator);
        triptoread.dateend.year=strtol(token,&help,10);
        token=strtok(string3, separator);
        triptoread.timeend.hour=strtol(token,&help,10);
        token=strtok(NULL, separator);
        triptoread.timeend.minute=strtol(token,&help,10);
        token=strtok(NULL, separator);
        //Skip the seconds
        token=strtok(NULL, separator);
        triptoread.stationstopID=strtol(token,&help,10);
        token=strtok(NULL, separator);
        //If the Bike ID is defined
        if(strcmp(token,"Casual")!=0 && strcmp(token,"Registered")!=0)
        {
            strcpy(triptoread.bikeID, token);
            token=strtok(NULL, separator);

        }

        //If it's not, we set an invalid value to it
        else
        {
            strcpy(triptoread.bikeID, "-1");
        }

        //If the type is Casual we set invalid data to birthyear and gender
        if(strcmp(token, "Casual")==0)
        {
            triptoread.user.type=0;
            triptoread.user.birthyear=-1;
            triptoread.user.gender=-1;
        }

        else
        {
            triptoread.user.type=1;
            token=strtok(NULL, separator);
            triptoread.user.birthyear=strtol(token,&help,10);
            token=strtok(NULL, separator);
            if(strcmp(token, "Male")==0)
                triptoread.user.gender=1;
            else
                triptoread.user.gender=0;
        }

        //Now that we have the structure we insert the node
        InsertTripList(_triplist, triptoread);
    }

}

/**
* ĺoad_stationfile function: to create the list of stations
* \param _stationlist --> the list of stations (passed by reference)
* \param _fp --> the file
*/

void load_stationfile(stationnode** _stationlist, FILE* _fp)
{
    station_data stationtoread;//data for each node of the list
    char separator[]=",";//separator for strtok
    char buffer[BUFSIZE]={'\0'};// string to save the lines of the file
    char* token=NULL;//auxiliar pointer to apply strtok
    char* help=NULL;//auxiliar pointer to apply strtol

    while(fgets(buffer, BUFSIZE, _fp)!=NULL)
    {
        token=strtok(buffer, separator);
        stationtoread.stationID= strtol(token,&help,10);
        token=strtok(NULL, separator);
        strcpy(stationtoread.terminal,token);
        token=strtok(NULL, separator);
        stationtoread.station=(char*)malloc((strlen(token)+1)*sizeof(char));
        if(stationtoread.station==NULL)
        {
            printf("Error: Memory not correctly allocated\n");
            exit(EXIT_FAILURE);
        }
        strcpy(stationtoread.station, token);
        token=strtok(NULL, separator);
        stationtoread.municipal=(char*)malloc((strlen(token)+1)*sizeof(char));
        if(stationtoread.municipal==NULL)
        {
            printf("Error: Memory not correctly allocated\n");
            exit(EXIT_FAILURE);
        }
        strcpy(stationtoread.municipal, token);
        token=strtok(NULL, separator);
        sscanf(token, "%lf", &stationtoread.lat);
        token=strtok(NULL, separator);
        sscanf(token, "%lf", &stationtoread.lng);
        token=strtok(NULL, separator);
        if(strcmp(token,"Existing")==0)
            stationtoread.status=1;
        else
            stationtoread.status=0;

         //Now that we have the structure we insert the node
        InsertStationList(_stationlist, stationtoread);
    }
}

/**
* NewTripNode function: to create a new node for the list of trips
* \param _trip --> the trip structure of the node
* It returns the newnode
*/

tripnode* NewTripNode(trip_data _trip)
{
    tripnode* newnode=NULL; //the newnode to return
    //dinamically allocate memory for the new node and test if it went correctly
    newnode=(tripnode*)malloc(sizeof(tripnode));
    if(newnode==NULL)
    {
        printf("Error: Memory not correctly allocated\n");
        exit(EXIT_FAILURE);
    }

    newnode->trip_file=_trip; //attach the structure _trip to the node
    newnode->next=NULL; //make the next pointer of the node pointing to NULL

    return newnode;
}

/**
* NewStationNode function: to create a new node for the list of stations
* \param _station --> the station structure of the node
* It returns the newnode
*/

stationnode* NewStationNode(station_data _station)
{
    stationnode* newnode=NULL; //the newnode to return
    //dinamically allocate memory for the new node and test if it went correctly
    newnode=(stationnode*)malloc(sizeof(stationnode));
    if(newnode==NULL)
    {
        printf("Error: Memory not correctly allocated\n");
        exit(EXIT_FAILURE);
    }

    newnode->station_file=_station; //attach the structure _station to the node
    newnode->next=NULL; //make the next pointer of the node pointing to NULL

    return newnode;
}

/**
* InsertTripList function: to add nodes in the list of trips
* \param _headtrip --> the head node of the list (the list itself passed by reference)
* \param _trip --> the trip structure
*/

void InsertTripList(tripnode** _headtrip, trip_data _trip)
{
    /*Our strategy here is to always make the new nodes the head itself so our last node will be the first in the list*/
    tripnode *newnode=NULL; //the newnode to add

    //To create the new node
    newnode=NewTripNode(_trip);
    //We set the newnode next pointer to point to the head
    newnode->next= *_headtrip;
    //We make the newnode the head itself
    *_headtrip=newnode;
}

/**
* InsertSationList function: to add nodes in the list
* \param _headstation --> the head node of the list (the list itself passed by reference)
* \param _station --> the station structure
*/
void InsertStationList(stationnode** _headstation, station_data _station)
{
    /*Our strategy here is to always make the new nodes the head itself so our last node will be the first in the list*/
    stationnode *newnode=NULL; //the newnode to add
    //To create the new node
    newnode=NewStationNode(_station);
    //We set the newnode next pointer to point to the head
    newnode->next= *_headstation;
    //We make the newnode the head itself
    *_headstation=newnode;

}

/**
* fill_tripstations function: fills each trip structure with the corresponding stations
* \param _triplist, _stationlist --> the head nodes of the lists (the list itself passed by reference)
*/
void fill_tripstations(tripnode** _triplist, stationnode** _stationlist)
{
    tripnode* current_trip=NULL; //auxiliar pointer to iterate over the trip list
    stationnode* current_station=NULL; //auxiliar pointer to iterate over the station list
    int matched=0; //a flag to stop us from iterating over the stations if both of the start and stop ones are already attached to the
                   //respective trip

    //We set the auxiliar pointer to point to the top of the trip list
    current_trip=*_triplist;

    //We iterate over the list until the pointer points to NULL (the list just ended)
    while(current_trip!=NULL)
    {
        //resets the current station to the top of the list
        current_station=*_stationlist;
        matched=0;
        while(current_station!=NULL)
        {
           //Now if the stationID of the current station is equal to the station start ID of the current trip
           //We then make the pointer start of the current trip to point to the entire station structure (by passing its address)
            if(current_station->station_file.stationID==current_trip->trip_file.stationstartID)
            {
                current_trip->trip_file.start=&(current_station->station_file);
                matched++;
            }
            //Now if the stationID of the current station is equal to the station stop ID of the current trip
           //We then make the pointer stop of the current trip to point to the entire station structure (by passing its address)
            if(current_station->station_file.stationID==current_trip->trip_file.stationstopID)
            {
                current_trip->trip_file.stop=&(current_station->station_file);
                matched++;
            }

            if(matched==2)
                break;

            current_station=current_station->next;
        }

        current_trip=current_trip->next;
    }
}


/**
* RemoveUsingHour function: to remove form the list using the hour criterion
* \param _headstation --> the head node of the list (the list itself passed by reference)
* \param _station --> the station structure
*/

tripnode* RemoveUsingHour(ttime* _begin, ttime* _end, tripnode* _headtrip)
{
    tripnode* current=NULL; //auxiliar node to iterate over the list
    tripnode* prev=NULL; //auxiliar node to iterate over the list (one pinter behind the current one)

    //if the list is empty
    if(_headtrip==NULL)
    {
        //we simply return the head i.e. NULL
        return _headtrip;
    }

    current=_headtrip;
    prev=current;

    while(current!=NULL)
    {
        if(current->trip_file.timebegin.hour<_begin->hour || current->trip_file.timeend.hour>=_end->hour || current->trip_file.datebegin.day != current->trip_file.dateend.day)
        {
            RemoveNode(&current, &prev, &_headtrip);
        }

        else
        {
            prev=current; //We save the current node
            current=current->next; //We use the pointer to the next
        }

    }

    //We return the head
    return _headtrip;

}


tripnode* RemoveUsingWeekday(int _weekday, tripnode* _headtrip)
{
    tripnode* current=NULL; //auxiliar node to iterate over the list
    tripnode* prev=NULL; //auxiliar node to iterate over the list (one pinter behind the current one)

    int converteddate=0;//variable to store the converted date


    //if the list is empty
    if(_headtrip==NULL)
    {
        //we simply return the head i.e. NULL
        return _headtrip;
    }

    current=_headtrip;
    prev=current;

    while(current!=NULL)
    {
        //Convert the date
        converteddate=ConvertDate(current->trip_file.datebegin.day, current->trip_file.datebegin.month, current->trip_file.datebegin.year);

        if(converteddate!=_weekday)
        {
            RemoveNode(&current, &prev, &_headtrip);
        }

        else
        {
            prev=current; //We save the current node
            current=current->next; //We use the pointer to the next
        }


    }

    return _headtrip;

}

/**
* ConvertDate function function: to convert a date into a day of the week using the Key Value Method
* \param _day
* \param _month
* \param _year
* It returns the converted value
*/
int ConvertDate(int _day, int _month, int _year)
{
    int convert=0; //variable to save the conversion
    int monthscale=0; //auxiliar variable to convert the month
    int yeardigits=0;//variable to save the last two digits of the year
    int leapyearflag=0; //to flag a leap year
    //We start by taking the last two digits of the year
    convert=_year%100;
    //We divide by 4
    convert/=4;
    //We sum it to the day
    convert+=_day;

    //We use the table of conversion of the months
    switch(_month)
    {
        case 1:
        case 10:
                monthscale=1;
                break;
        case 2:
        case 3:
        case 11:
                monthscale=4;
                break;
        case 4:
        case 7:
                monthscale=0;
                break;
        case 5:
                monthscale=2;
                break;
        case 6:
                monthscale=5;
                break;
        case 8:
                monthscale=3;
                break;
        case 9:
        case 12:
                monthscale=6;
    }


    //We sum the monthscale
    convert+=monthscale;
    //We verify if we have a leap year
    leapyearflag=LeapYear(_year);

    //If we have and the month is january or february we decrement convert
    if(leapyearflag==1 &&(_month==1 || _month==2))
    {
        convert--;
    }

    //We sum 6
    convert+=6;

    //We take the last two digits of the year
    yeardigits=_year%100;
    //We add it
    convert+=yeardigits;

    //We take the rest of the division by 7
    convert%=7;

    //If the result is zero or 1(Saturday or Sunday) we add 6 to convert to our scale (6=Saturday, 7=Sunday)
    if(convert==0 || convert==1)
        convert+=6;
    //If the result is other we decrement to have the same effect
    else
        convert--;

    //We returned the converted value
    return convert;
}

/**
* Leapyear function function: to test if a year is leap
* \param _year
* It returns a flag to tell if the the year is leap(1) or not(0)
*/

int LeapYear(int _year)
{
    int flag=0;
    //If the year can be divided by 400 the year is leap
    if(_year%400==0)
        flag=1;
    else
    {
        //if it can be divided by 100 (but not by 400) it is not a leap year
        if(_year%100==0)
            flag=0;
        else
        {
            //if it can't be divided neither by 400 nor 100, but it is by 4 it's a leap year
            if(_year%4==0)
                flag=1;
            //If every condition fails it is not a leap year
            else
                flag=0;
        }
    }

    return flag;

}



tripnode* RemoveUsingMaxduration(int _maxduration, tripnode* _headtrip)
{
    tripnode* current=NULL; //auxiliar node to iterate over the list
    tripnode* prev=NULL; //auxiliar node to iterate over the list (one pinter behind the current one)

    //if the list is empty
    if(_headtrip==NULL)
    {
        //we simply return the head i.e. NULL
        return _headtrip;
    }


    current=_headtrip;
    prev=current;

    while(current!=NULL)
    {
        if(current->trip_file.timespan>_maxduration)
        {
            RemoveNode(&current,&prev,&_headtrip);
        }

        else
        {
            prev=current; //We save the current node
            current=current->next; //We use the pointer to the next
        }

    }

    return _headtrip;

}



/**
* RemoveNode function: to remove nodes from the list
* \param _headstation --> the head node of the list (the list itself passed by reference)
* \param _current --> the node being removed from the list
* \param _prev --> the node previous to the one being removed
*/
void RemoveNode(tripnode** _current, tripnode** _prev, tripnode** _headtrip)
{
    tripnode* _temp=NULL; //auxiliar node to free memory
    //To remove the head
    if(*_current==*_headtrip)
    {
        //We set the current to temp
        _temp=*_current;
        //we advance the current
        *_current=(*_current)->next;
        //We set the current to prev
        *_prev=*_current;
        //the current is the new head
        *_headtrip=*_current;
        //we free the memory
        free(_temp);
    }
    //To remove in the middle of the list or in the tail
    else
    {
        //We set the current to temp
        _temp=*_current;
        //We set the next pointer of prev to the next pointer of current
        (*_prev)->next=(*_current)->next;
        //We advance current
        *_current=(*_current)->next;
        //We free the memory
        free(_temp);
    }
}

