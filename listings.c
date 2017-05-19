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
    tripnode* current=NULL; //auxiliar node to iterate over the list of trips
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

void stationlisting(void)
{

}

void routelisting(tripnode** _triplist)
{
    int num=0, ins=0, station=0; //variables to 1-save the number of trips to show by page and 2-use in the cycle to print;
    char buffer[BUFSIZE]= {'\0'}; //string to get the what the user types on the screen
    int stationarrive[STATIONMAX]= {0}, stationdepart[STATIONMAX]= {0};// vectors that contain the number of trips to a station (vector position=stationID)
    int tempstation[STATIONMAX]= {0}, tempstation2[STATIONMAX]= {0}; //temporary vectors are used so that stations already ordered can be erased
    int validchoice=0; //test variable to evaluate what the user typed
    tripnode* current=*_triplist;
    tripnode* prev=NULL; //auxiliar node to iterate over the list (one pinter behind the current one)
    tripnode* end=NULL;

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

    //initial selection erases repeated routes (same origin and destination) and routes that dont contain the station
    prev=current;
    while(current!=NULL)
    {   //if it does not contain the station the node is removed
        if(current->trip_file.stationstartID!=num && current->trip_file.stationstopID!=num)
        {
            RemoveNode(&current,&prev,_triplist);
        }
        else if(current->trip_file.stationstartID==num)
        {   //if it contains the station as origin the vector position of the destination station is incremented
            tempstation[current->trip_file.stationstopID]++;
            stationdepart[current->trip_file.stationstopID]++;
            if(stationdepart[current->trip_file.stationstopID] > 1)
                RemoveNode(&current,&prev,_triplist); //if the station has already been incremented (already exists its repetition is removed)
            else
            {   //if the station has not been analyzed yet
                prev=current; //We save the current node
                current=current->next; //We use the pointer to the next
            }
        }
        else
        {   //if it contains the station as dest the vector position of the origin station is incremented
            tempstation2[current->trip_file.stationstartID]++;
            stationarrive[current->trip_file.stationstartID]++;
            if(stationarrive[current->trip_file.stationstartID] > 1)
                RemoveNode(&current,&prev,_triplist); //if the station has already been incremented (already exists its repetition is removed)
            else
            {   //if the station has not been analyzed yet
                prev=current; //We save the current node
                current=current->next; //We use the pointer to the next
            }
        }
    }
    end=SortTripList(_triplist,num);
    for(int i=0; i<STATIONMAX; i++)
    {   //this loops sends (from the highest to the lowest) stations to the end so that in the end they are ordered
        station=order_bynumber(_triplist,end,tempstation);
        tempstation[station]=0; //its value in the temporary vector is erased so it is not re-ordered
        if(station==0) //if the station value is 0 there are no more stations to be ordered
            break;
    }
    for(int i=0; i<STATIONMAX; i++)
    {   //the same loop is repeated now for the 2nd group (contain the inserted station as destination)
        station=order_bynumber(&end,NULL,tempstation2);
        tempstation2[station]=0;
        if(station==0)
            break;
    }
    //station values are printed
    print_routelist(_triplist,end,ins,num,stationdepart,stationarrive);
}

void statisticslisting(void)
{

}
/**
* SortTripList function: function that sorts the station list separating the ones that start in the station from the ones that end.
* Note: from this point items that start on station_ID and items that end on station_ID will be mostly threated as two sublists
* \param trip_head --> the head node of the list (the list itself passed by reference)
* \param stationID --> the inserted ID for the station
* It returns a pointer to the beggining of the end stations
*/
tripnode* SortTripList(tripnode** trip_head, int station_ID)
{
    int notSaved=0; //auxiliar flag to indicate the first node of the stopstations
    tripnode* current=*trip_head;
    tripnode* end=NULL;
    tripnode* prev=NULL;

    while(current!=NULL)
    {   //if the node is not already the head of the list and it starts at the inserted station
        if(current!=*trip_head && current->trip_file.stationstartID==station_ID)
        {   //the node is removed from the middle of the list
            prev->next=current->next;
            current->next=*trip_head;
            //We make the node starting at that station the head itself
            *trip_head=current;
            //we continue counting from where we were
            current=prev;
        }
        if(notSaved==0 && current->trip_file.stationstopID==station_ID)
        {   //if it is the first node with the stop station its pointer is saved to end and the flag is turned "off"
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
    int i=0, hightrip=0, highstation=0 ;
    tripnode* cur=*depart_head;
    tripnode* prev=cur;
    tripnode* depart_tail=NULL;
    //search for the pointer to the last member of the group (start/ending stations)
    while(cur!=arrive_head)
    {   //while we dont get to the first pointer of the other group or the NULL pointer we save the cur
        depart_tail=cur;
        cur=cur->next;
    }
    //search for the station with most trips number in the vector
    for(i=0; i<STATIONMAX; i++)
    {   //stationmax is set to 100 in case new stations are added
        if(depart_val[i]>hightrip)
        {   //in the case where there are more trips than the ones in the highstation'th position
            //we save their values for further comparisons and their position
            hightrip=depart_val[i];
            highstation=i;
        }
    }
    //in case there are no more stations highstation will be 0 so we return
    if(highstation==0)
        return 0;
    //we re-assign the head pointer to cur
    cur=*depart_head;
    //in the case where arrive_head is NULL we are attending the group that stops at the station
    if(arrive_head==NULL)
        i=cur->trip_file.stationstartID;
    //in the other case we are attending the group that starts at the station
    else
        i=cur->trip_file.stationstopID;

    //we now go through the list to find the route that corresponds to the found station
    while(i!=highstation)
    {   //we save the prev pointer for further use and itinerate over the list
        prev=cur;
        cur=cur->next;
        //i var is always reset for the new station
        if(arrive_head==NULL)
            i=cur->trip_file.stationstartID;
        else
            i=cur->trip_file.stationstopID;
    }
    //once we get the item with the most stations we want to send it to the end of the group
    if(cur!=*depart_head)
    {   //if it is not the group head of the group we simply make the list jump over it and set it to the end
        prev->next=cur->next;
        depart_tail->next=cur;
        cur->next=arrive_head;
    }
    else
    {   //if it is the head of the group we assign a new head and send it to the end
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
    tripnode* current=*_triplist;
    int i;
    char c;
    // The infinite cycle serves our purpose of printing pages of num elements
    while(1)
    {
        i=ins; //Reset i so that we can print more num trips
        //We'll print the the number of trips chosen by the user and until we reach the end of the list
        printf("\nStarting at stationID %d:\n",num);
        while(i>0 && current!=end)
        {

            printf("%d: %s",current->trip_file.stationstopID, current->trip_file.stop->station);
            printf(" (%d trips)\n",stationdepart[current->trip_file.stationstopID]);
            i--;
            current=current->next;//interate over the list
        }
        i=ins; //Reset i so that we can print more num trips

        printf("\nEnding at stationID %d:\n",num);
        while(i>0 && end!=NULL)
        {
            printf("%d: %s",end->trip_file.stationstartID, end->trip_file.start->station);
            printf(" (%d trips)\n",stationarrive[end->trip_file.stationstartID]);
            i--;
            end=end->next;//interate over the list
        }
        //when  we reach the end, we break the cycle
        if(current==end || end==NULL)
            break;

        printf("\n\nPress ENTER to continue. Press 1 to leave\n\n");
        c=getchar();
        if(c=='1')
            break;

        i=ins; //Reset i so that we can print more num trips
    }
}
