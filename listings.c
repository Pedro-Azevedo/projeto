#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//We include our header file
#include "trips.h"
#include "argtest.h"
#include "listings.h"


/**
* triplisting function: to print the list of trips
* \param _triplist --> the list of trips (passed by reference)
*/

void triplisting(tripnode** _triplist)
{
    int num=0, i=0; //variables to 1-save the number of trips to show by page and 2-use in the cycle to print
    char c='\0'; //character to leave the cycle of printing or print more
    char buffer[BUFSIZE]= {'\0'}; //string to get the what the user types on the screen
    int validchoice=0; //test variable to evaluate what the user typed
    tripnode* current=NULL; //auxiliar pointer to iterate over the list of trips
    do
    {
        printf("How many trips do you want the program to show?\n");
        fgets(buffer, BUFSIZE, stdin);
        validchoice=sscanf(buffer, "%d", &num);
        if(num<=0)
        {
            printf("Invalid choice\n");
            validchoice=0;
        }
    }
    while(validchoice!=1);

    i=num;
    // we set the auxiliar node to the head of the list
    current=*_triplist;


    // The infinite cycle serves our purpose of printing pages of num elements
    while(1)
    {
        //We'll print the the number of trips chosen by the user and until we reach the end of the list
        while(i>0 && current!=NULL)
        {
            printf("TripID: %d\n", current->trip_file.tripID);
            printf("Timespan: %d\n", current->trip_file.timespan);
            printf("Date_Begin: %d/%d/%d\n", current->trip_file.datebegin.month, current->trip_file.datebegin.day, current->trip_file.datebegin.year);
            printf("Time_Begin:%d:%.2d:00\n", current->trip_file.timebegin.hour, current->trip_file.timebegin.minute);
            printf("Station_Start ID:%d\n",current->trip_file.stationstartID);
            printf("Date_End:%d/%d/%d\n", current->trip_file.dateend.month, current->trip_file.dateend.day, current->trip_file.dateend.year);
            printf("Time_End:%d:%.2d:00\n", current->trip_file.timeend.hour, current->trip_file.timeend.minute);
            printf("Station_Stop ID:%d\n",current->trip_file.stationstopID);
            if(strcmp(current->trip_file.bikeID,"-1")!=0)
                printf("BikeID: %s\n", current->trip_file.bikeID);
            else
                printf("Doesn't have a BikeID associated\n");

            if(current->trip_file.user.type==1)
            {
                printf("Type: Registered\n");
                printf("Birthyear: %d\n", current->trip_file.user.birthyear);
                if(current->trip_file.user.gender==1)
                    printf("Gender: Male\n");
                else
                    printf("Gender Female\n");
            }

            else
            {
                printf("Type: Casual\n");
            }

            i--;
            current=current->next;//interate over the list
            printf("\n\n");
        }

        //when  we reach the end, we break the cycle
        if(current==NULL)
            break;

        printf("\nPress a key to continue. Press 1 to leave\n");
        c=getchar();
        if(c=='1')
            break;
        i=num; //Reset i so that we can print more num trips
    }

}


void stationlisting(tripnode** _triplist, stationnode** _stationlist)
{
    int start[STATIONMAX][HOURS]={'\0'};
    int end[STATIONMAX][HOURS]={'\0'};
    int j=0;
    int maxbikesstart=0;
    int minbikesstart=0;
    int maxbikesstop=0;
    int minbikesstop=0;
    int sumbikesstart=0;
    int sumbikesstop=0;
    float averbikesstart=0.0f;
    float averbikesstop=0.0f;
    stationnode* current=NULL;
    tripnode* it=NULL;

    it=*_triplist;

    while(it!=NULL)
    {
        start[it->trip_file.stationstartID][it->trip_file.timebegin.hour]++;
        end[it->trip_file.stationstopID][it->trip_file.timeend.hour]++;
        it=it->next;
    }


    current=*_stationlist;

    while(current!=NULL && current->station_file.stationID!=0)
    {
        j=0;
        while(j<HOURS+1)
        {
            if(start[current->station_file.stationID][j]!=0 || end[current->station_file.stationID][j]!=0)
                break;
            j++;
        }

        if(j!=HOURS)
        {
            printf("\n");
            printf("Station ID: %d\n", current->station_file.stationID);
            printf("Terminal: %s\n", current->station_file.terminal);
            printf("Station: %s\n", current->station_file.station);
            printf("Municipal: %s\n", current->station_file.municipal);
            printf("Latitude: %f\n", current->station_file.lat);
            printf("Longitude: %f\n", current->station_file.lng);

            maxbikesstart=start[current->station_file.stationID][0];
            minbikesstart=start[current->station_file.stationID][0];
            sumbikesstart=0;
            maxbikesstop=end[current->station_file.stationID][0];
            minbikesstop=end[current->station_file.stationID][0];
            sumbikesstop=0;

            for(j=1; j<HOURS+1; j++)
            {
                if(maxbikesstart<start[current->station_file.stationID][j])
                    maxbikesstart=start[current->station_file.stationID][j];

                if(minbikesstart>start[current->station_file.stationID][j])
                    minbikesstart=start[current->station_file.stationID][j];

                if(maxbikesstop<end[current->station_file.stationID][j])
                    maxbikesstop=end[current->station_file.stationID][j];

                if(minbikesstop>end[current->station_file.stationID][j])
                    minbikesstop=end[current->station_file.stationID][j];

                sumbikesstart+=start[current->station_file.stationID][j];
                sumbikesstop+=end[current->station_file.stationID][j];
            }

            averbikesstart=(float)sumbikesstart/HOURS;
            averbikesstop=(float)sumbikesstop/HOURS;


            printf("Maximum number of bikes leaving: %d\n", maxbikesstart);
            printf("Minimum number of bikes leaving: %d\n", minbikesstart);
            printf("Average number of bikes leaving: %f\n", averbikesstart);
            printf("Maximum number of bikes arriving: %d\n", maxbikesstop);
            printf("Minimum number of bikes arriving: %d\n", minbikesstop);
            printf("Average number of bikes arriving: %f\n", averbikesstop);

        }

        current=current->next;

    }

    printf("\n \n \n");

/*
    for(i=0; i<70; i++)
    {
        printf("STATION:%d\n", i);
        for(j=0; j<HOURS; j++)
        {
            printf("%d\t", start[i][j]);
        }

        printf("\n");
    }*/

}

void routelisting(tripnode** _triplist)
{
    int num=0, ins=0; //variables to 1-save the station ID, 2-save the number of trips to show
    int station=0;//variable to save a station ID. Zero if there is no station.
    char buffer[BUFSIZE]= {'\0'}; //string to get the what the user types on the screen
    int stationarrive[STATIONMAX]= {0}, stationdepart[STATIONMAX]= {0};// vectors that contain the number of trips to a station (vector position=stationID)
    int tempstation[STATIONMAX]= {0}, tempstation2[STATIONMAX]= {0}; //temporary vectors that also contain that so that we can manipulate data without losing it
    int validchoice=0; //test variable to evaluate what the user typed
    tripnode* current=NULL; //auxiliar pointer to iterate over the list
    tripnode* prev=NULL; //auxiliar pointer to iterate over the list (one pointer behind the current one)
    tripnode* stopbegin=NULL; //auxiliar pointer that will help us distinguish in the sorted list when the stationID chosen is correspondent to a start station or a stop station
    int i=0; //variable for cycles

    do
    {   //menu for route choosing and number of routes per page
        printf("Choose:\n  -One of the stations ID's\n");
        printf("  -How many trips you want the program to show\n");
        fgets(buffer, BUFSIZE, stdin);
        validchoice=sscanf(buffer, "%d %d", &num, &ins);
        if(num<3 || ins<=0)
        {
            printf("Invalid choice\n");
            validchoice=0;
        }
    }
    while(validchoice!=2);

    //We start by organizing our trip list to make it easier to print data later

    current=*_triplist;
    prev=current;

    while(current!=NULL)
    {
        //If the current trip does not contain the chosen station, the node is removed
        if(current->trip_file.stationstartID!=num && current->trip_file.stationstopID!=num)
        {
            RemoveNode(&current,&prev,_triplist);
        }

        //If the current trip contains the chosen station as start
        else if(current->trip_file.stationstartID==num)
        {
            //If it matched the start station of the current trip, then we
            // (1) Increment the respective position of the stop station in the tempstation vector;
            // (2) Increment the respective position of the stop station in the stationdepart vector;
            tempstation[current->trip_file.stationstopID]++;
            stationdepart[current->trip_file.stationstopID]++;

            //If the station has already been incremented (already exists, the trip is removed from the list)
            //This will avoid repetitions in printing
            if(stationdepart[current->trip_file.stationstopID] > 1)
                RemoveNode(&current,&prev,_triplist);


            else
            {
                prev=current; //We save the current node
                current=current->next; //We go to the next node
            }

        }

        else
        {
            //If it matched the stop station of the current trip, then we
            // (1) Increment the respective position of the start station in the tempstation2 vector;
            // (2) Increment the respective position of the start stationin the stationarrive vector;
            tempstation2[current->trip_file.stationstartID]++;
            stationarrive[current->trip_file.stationstartID]++;

            //if the station has already been incremented (already exists, the trip is removed from the list)
            if(stationarrive[current->trip_file.stationstartID] > 1)
                RemoveNode(&current,&prev,_triplist);

            //if the station has not been analyzed yet
            else
            {
                prev=current; //We save the current node
                current=current->next; //We use the pointer to the next
            }

        }

    }


    //What remains of our trip list will now be sorted to differentiate the trips that our station as start station or as stop station
    stopbegin=SortTripList(_triplist,num);


    //Now we will sort the list again, ordering it by number of trips to the stop station
    for(i=0; i<STATIONMAX; i++)
    {

        station=order_bynumber(_triplist,stopbegin,tempstation);
        //the station value in the temporary vector is erased so it is not re-ordered
        tempstation[station]=0;

        //if the station value is 0 there are no more stations to be ordered
        if(station==0)
            break;
    }

    //We do the same now ordering the start stations by number of trips
    for(i=0; i<STATIONMAX; i++)
    {
        station=order_bynumber(&stopbegin,NULL,tempstation2);
        tempstation2[station]=0;

        if(station==0)
            break;
    }

    //We finally print the routes
    print_routelist(_triplist,stopbegin,ins,num,stationdepart,stationarrive);
}

void statisticslisting(void)
{

}
/**
* SortTripList function: function that sorts the station list separating the ones that start in the station from the ones that end.
* Note: from this point items that start on station_ID and items that end on station_ID will be mostly threated as two sublists
* \param trip_head --> the head node of the list (the list itself passed by reference)
* \param stationID --> the inserted ID for the station
* It returns a pointer to the beginning of the end stations
*/
tripnode* SortTripList(tripnode** trip_head, int station_ID)
{
    int notSaved=0; //auxiliar flag to indicate the first node of the stopstations
    tripnode* current=*trip_head; //auxiliar pointer to iterate over the list
    tripnode* end=NULL; //node to save the begin of the second sublist
    tripnode* prev=NULL; //auxiliar pointer to iterate over the list (one node behind current)

    while(current!=NULL)
    {
        //if the node is not already the head of the list and it starts at the inserted station

        if(current!=*trip_head && current->trip_file.stationstartID==station_ID)
        {
            //the node is removed from the middle of the list
            prev->next=current->next;
            current->next=*trip_head;
            //We make the node starting at that station the head itself
            *trip_head=current;
            //we continue counting from where we were
            current=prev;
        }

        if(notSaved==0 && current->trip_file.stationstopID==station_ID)
        {
            //if it is the first node with the stop station its pointer is saved to end and the flag is turned "off"
            notSaved=1;
            end=current;
        }

        prev=current;
        current=current->next;
    }
    prev->next=NULL;
    return end;
}

/**
* order_bynumber function: function that sorts all items between depart_head and arrive_head(not included) by how many trips they represent
* \param depart_head --> the head node of the sublist (the list itself passed by reference)
* \param arrive_head --> the end of the sublist
* Returns an integer to the "vector position"-->"list item" moved to the tail
*/
int order_bynumber(tripnode** depart_head, tripnode* arrive_head, int depart_val[STATIONMAX])
{
    int i=0; //variable to use in cycles
    int hightrip=0, highstation=0; //variables to save data (1) the maximum number of trips and (2) the index of the trip to put on top
    tripnode* cur=NULL; //auxiliar pointer to iterate over the list
    tripnode* prev=NULL;//auxiliar pointer to iterate over the list (one node behind the current node)
    tripnode* depart_tail=NULL; //auxiliar pointer to save the cur node

    cur=*depart_head;
    prev=cur;

    //We go all over the list until we find the end of the sublist we're considering
    while(cur!=arrive_head)
    {
         //while we don't get to the first pointer of the other sublist or to the NULL pointer, we save the cur
        depart_tail=cur;
        cur=cur->next;
    }

    //search for the station with most trips number in the vector
    for(i=0; i<STATIONMAX; i++)
    {

        if(depart_val[i]>hightrip)
        {
            //in the case where there are more trips than the ones in the highstation'th position
            //we save their values for further comparisons and we also save their positions
            hightrip=depart_val[i];
            highstation=i;
        }

    }

    //in case there are no more stations highstation will be 0 so we return'immediately
    if(highstation==0)
        return 0;

    //we reassign cur to the the begin of the sublist
    cur=*depart_head;

    //in the case where arrive_head is NULL we are considering the second sublist (that considers the chosen station as a stop station)
    //then we attribute i to the stationstartID of the begin of the sublist
    if(arrive_head==NULL)
        i=cur->trip_file.stationstartID;

    //in the other case we are considering the group that has the chosen station as a start station
    //then we attribute i to the stationstartID of the begin of the sublist
    else
        i=cur->trip_file.stationstopID;


    //we now go through the list to find the route that corresponds to the found station
    while(i!=highstation)
    {
        //we save the prev pointer for further use and iterate over the list
        prev=cur;
        cur=cur->next;

        //i var is always reset for the new station
        if(arrive_head==NULL)
            i=cur->trip_file.stationstartID;

        else
            i=cur->trip_file.stationstopID;

    }

    //once we get the item with the most stations we want to send it to the end of the group

    //if it is not the head of the sublist we simply make the list jump over it and set it to the end
    if(cur!=*depart_head)
    {
        prev->next=cur->next;
        depart_tail->next=cur;
        cur->next=arrive_head;
    }

    //if it is the head of the sublist we assign a new head and send it to the end
    else
    {
        depart_tail->next=cur;
        *depart_head=cur->next;
        cur->next=arrive_head;
    }

    //highstation is returned so we can set its value to 0 and continue to the following higher value
    return highstation;
}

/**
 * print_routelist function: prints the route listing stations according to the inserted parameters
 * _triplist --> the head node of the list
 * end --> the head node of the "sublist" (part of the list that contains the stationID as destination)
 * ins --> number of stations per page
 * num --> station ID number
 * stationdepart --> vector with the number of trips between station ID and the respective value pos (as destination)
 * stationdepart --> vector with the number of trips between station ID and the respective value pos (as origin)
 */
void print_routelist(tripnode** _triplist, tripnode* end, int ins, int num, int stationdepart[STATIONMAX], int stationarrive[STATIONMAX])
{
    tripnode* current=NULL; //auxiliar pointer to iterate over the list
    int i;//auxiliar variable for printing routes
    char c;

    current=*_triplist;

    // The infinite cycle serves our purpose of printing pages of ins elements
    while(1)
    {
        //Reset i so that we can print more ins trips
        i=ins;

        //We'll print the the number of trips chosen by the user and until we reach the end of the list
        printf("\nStarting at stationID %d:\n",num);

        while(i>0 && current!=end)
        {

            printf("%d: %s",current->trip_file.stationstopID, current->trip_file.stop->station);
            printf(" (%d trips)\n",stationdepart[current->trip_file.stationstopID]);
            i--;
            current=current->next;//iterate over the list
        }

        //Reset i so that we can print more ins trips
        i=ins;

        printf("\nStopping at stationID %d:\n",num);

        while(i>0 && end!=NULL)
        {

            printf("%d: %s",end->trip_file.stationstartID, end->trip_file.start->station);
            printf(" (%d trips)\n",stationarrive[end->trip_file.stationstartID]);
            i--;
            end=end->next;//iterate over the list

        }

        //when  we reach the end, we break the cycle

        if(current==end || end==NULL)
            break;


        printf("\n\nPress ENTER to continue. Press 1 to leave\n\n");

        c=getchar();

        if(c=='1')
            break;

        i=ins; //Reset i so that we can print more ins trips
    }
}
