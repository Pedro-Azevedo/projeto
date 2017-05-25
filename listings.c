#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//We include our header file
#include "trips.h"
#include "argtest.h"
#include "listings.h"
#include "graphicmode.h"


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
        system("clear");
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

    do{
        //menu for route choosing and number of routes per page
        printf("Choose:\n  -One of the stations ID's\n");
        printf("  -How many trips you want the program to show\n");
        fgets(buffer, BUFSIZE, stdin);
        validchoice=sscanf(buffer, "%d %d", &num, &ins);
        if(num<3 || ins<=0)
        {
            printf("Invalid choice\n");
            validchoice=0;
        }
    }while(validchoice!=2);


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
     system("clear");
    //We finally print the routes
    print_routelist(_triplist,stopbegin,ins,num,stationdepart,stationarrive);


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
    tripnode* current=NULL; //auxiliar pointer to iterate over the list
    tripnode* end=NULL; //node to save the begin of the second sublist
    tripnode* prev=NULL; //auxiliar pointer to iterate over the list (one node behind current)

    current=*trip_head;

    if(current==NULL)
    {
        return NULL;
    }

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
        system("clear");
        i=ins; //Reset i so that we can print more ins trips
    }
}
void statisticslisting(tripnode** _triplist)
{
    int userchoice=0; //the choice from the given menu
    // We set an infinite cycle to show us the menu so that everytime the user returns from
    // the various options functions, it will still display the menu for him to choose
    while(1)
    {
        printf("Consider the following options\n\n");
        printf("\t1. Type & Gender\n");
        printf("\t2. Age (Registered Only!)\n");
        printf("\t3. Trip duration\n");
        printf("\t4. Average Speed\n");
        printf("\t5. LEAVE MENU\n\n");
        userchoice=testchoice(5);

        switch(userchoice)
        {
        case 1:
            system("clear");
            GenderStats(_triplist);
            break;
        case 2:
            system("clear");
            AgeStats(_triplist);
            break;
        case 3:
            system("clear");
            DurationStats(_triplist);
            break;
        case 4:
            system("clear");
            AvgSpeedStats(_triplist);
            break;
        case 5:
            system("clear");
            return; //breaks the infinite cycle
        }
    }
    return;
}

/**
* GenderStats function: function that calculates the number of registered/casual members and in case of the first their sex
* \param _triplist --> the head node of the list (the list itself passed by reference)
*/
void GenderStats (tripnode** _triplist)
{
    tripnode* cur=*_triplist;
    double casual=0, regfem=0, regmale=0, total=0; //values counted during the list itineration
    double percent_v[3];

    while(cur!=NULL)
    {
        //type=0 --> casual     type=1 --> Registered (only registered present gender)
        if(cur->trip_file.user.type==0)
            casual++;
        else if(cur->trip_file.user.gender==1) //for the registered if gender=1 --> male
            regmale++;
        else //for the registered if gender=0 -->female
            regfem++;
        //itinerate over the list
        cur=cur->next;
    }
    total=(regfem+regmale+casual); //total of entries
    percent_v[0]= (regfem/total)*100;
    percent_v[1]= (regmale/total)*100;
    percent_v[2]= (casual/total)*100;
    print_gstats(percent_v);
    write_gstats(percent_v,total);
}

/**
* print_gstats function: function prints the results of GenderStats on the screen with some illustrative bars
* \param percent_v --> vector with the  needed values from GenderStats
*/
void print_gstats (double percent_v[2])
{   //vector to print the correct number of bars
    int bar_count[3]= {0};
    //top of the page
    printf("======================================================================\n");
    printf("\t\t\tTYPE & GENDER STATS\n");
    printf("======================================================================\n");
    //cycle to print each gender and type and respective data
    for(int i=0; i<=2; i++)
    {   //text selection
        if(i==0)
            printf("Registered Females\t");
        else if(i==1)
            printf("Registered Males\t");
        else
            printf("Casual\t\t\t");
        //because each bar represents 3% we divide de %/3  and decrease each time a bar is printed
        for(bar_count[i]=percent_v[i]/3; bar_count[i]>0; bar_count[i]--)
        {
            printf("|");
        }
        printf("  %.2f %%\n",percent_v[i]); //print the value with 2 decimal cases
        printf("----------------------------------------------------------------------\n");
    }
    //bottom of the page
    printf("\t\t\t\t\t\tPress ENTER to leave\n");
    printf("======================================================================\n");
    getchar(); //wait for enter to be pressed
    system("clear"); //screen clear
    return;
}

/**
* write_gstats function: function that writes the results of GenderStats on a .txt file
* \param percent_v --> vector with the  needed percentage values from GenderStats
* \param total --> number of trips being analyzed
*/
void write_gstats(double percent_v[2], double total)
{
    //creates a pointer to overwrite (w) on the .txt file
    FILE* fptr=fopen("type_gender_stats.txt","w");
    //text to be written and correspondent file close
    fprintf(fptr,"\t\t--Type & Gender Stats--\n\n");
    fprintf(fptr,"1. Casual: %.0f out of %.0f (%.2f%%)\n",((percent_v[2]/100)*total),total, percent_v[2]);
    fprintf(fptr,"2. Registered: %.0f out of %.0f (%.2f%%)\n",(((percent_v[0]+percent_v[1])/100)*total),total,(percent_v[0]+percent_v[1]));
    fprintf(fptr,"\t.1 Female: %.0f out of %.0f (%.2f%%)\n",((percent_v[0]/100)*total),total,percent_v[0]);
    fprintf(fptr,"\t.2 Male: %.0f out of %.0f (%.2f%%)\n",((percent_v[1]/100)*total),total,percent_v[1]);
    fclose(fptr);
}

/**
* AgeStats function: function that counts all members age in a range of 2 years
* \param _triplist --> the head node of the list (the list itself passed by reference)
*/
void AgeStats(tripnode** _triplist)
{
    tripnode* cur=*_triplist; //pointer to the beginning of the list
    int temp_birthyear, pos=0, agerangesize=0, total=0;
    double age[80]= {0}, percent_vect[80]= {0}; //vector to save the number of users that belong to each age
    //cycle to itinerate over the list
    while(cur!=NULL)
    {
        //if there was no birth year, it was set to -1 when it was read
        if(cur->trip_file.user.birthyear!=-1)
        {
            //if it exists its age is equal to the current year-birthyear
            temp_birthyear=2017-cur->trip_file.user.birthyear;
            //we divide by 2 because it is organized in 2 years range
            pos=temp_birthyear/2;
            if(pos>agerangesize)
            {
                //agerange saves the highest 2 year range with members
                agerangesize=pos;
            }
            age[pos]+=1;//every time a birth year is read its correspondent value is incremented
            total++;
        }
        cur=cur->next;
    }
    for(int i=0; i<agerangesize; i++)
    {
        percent_vect[i]=(age[i]/total)*100;
    }
    age[agerangesize]=percent_vect[agerangesize]=-1;
    print_astats(percent_vect);
    write_astats(percent_vect,total);
}

/**
* print_astats function: function that prints the age stats along with the illustrative bar graphic
* \param percent_v --> vector with the  needed percentage values from AgeStats
*/
void print_astats (double percent_v[80])
{
    int bar_count[80]= {0}; //vector to allow the bar printing
    int age=0, no_data=0;   //flag for empty lists and age
    //top of the page
    printf("======================================================================\n");
    printf("\t\t\t\tAGE STATS\n");
    printf("======================================================================\n");
    // loop for data printing (all age ranges)
    for(int i=0; i<=80; i++)
    {   //in case the value is invalid - NaN - (condition 2) because of some exaggerated numbers or too low (condition 1)
        if(percent_v[i]>=0.01 && percent_v[i]== percent_v[i])
        {   //prints age range
            printf("%d-%d     \t",age,(age+2));
            //because each bar represents 1%
            for(bar_count[i]=percent_v[i]; bar_count[i]>0; bar_count[i]--)
            {
                printf("|");
                no_data=1; // if a bar is printed it means that atleast 1% of the values exist
            }
            printf("  %.2f %%\n",percent_v[i]);
            printf("----------------------------------------------------------------------\n");
        }
        age=age+2; //"increment" age range
    }
    //if no data is available to be printed
    if(no_data==0)
        printf("\t\tNO DATA FITS YOUR SEARCH!!!\n");
    //bottom of the page
    printf("\t\t\t\t\t\tPress ENTER to leave\n");
    printf("======================================================================\n");
    getchar(); //wait for ENTER to be pressed and screen cleanse
    system("clear");
    return;
}

/**
* write_astats function: function that writes the results of AgeStats on a .txt file
* \param percent_v --> vector with the  needed percentage values from AgeStats
* \param total --> number of trips being analyzed
*/
void write_astats (double percent_v[80], double total)
{
    int age=0;
    //file .txt is opened and overwritten or created
    FILE* fptr=fopen("age_stats.txt","w");
    //top of the page
    fprintf(fptr,"\t\t--Age Stats--\n\n");
    for(int i=0; i<=80; i++)
    {   //conditions 1 and 2 from print_astats
        if(percent_v[i]>=0.01 && percent_v[i]== percent_v[i])
        {   //age range is followed by % and actual numbers
            fprintf(fptr,"%d yrs->%d yrs:\t",age,(age+2));
            fprintf(fptr,"%.2f %%   (%.0f out of %.0f)\n",percent_v[i],(percent_v[i]/100)*total, total);

        }
        age=age+2; //age is "incremented"
    }
    fclose(fptr); //file is closed
}

/**
* DurationStats function: function that counts all members trip duration
* \param _triplist --> the head node of the list (the list itself passed by reference)
*/
void DurationStats(tripnode** _triplist)
{
    int temp_timespan, pos;
    double duration[24], durationcount; //vector to save each 15 min interval up to 6 hours
    tripnode* cur=*_triplist;
    //cycle to itinerate over the list
    while(cur!=NULL)
    {
        //convertion of the timespan from seconds to minutes
        temp_timespan=(cur->trip_file.timespan/60);
        //due to the fat that each 15 minute interval corresponds to a position we divide the time by 15
        pos=temp_timespan/15;
        //if the timespan goes beyond the maximum it is ignored (if it does not its respective pos and the overall count are increased)
        if(pos<24)
        {
            duration[pos]+=1;
            durationcount++;
        }
        cur=cur->next;
    }
    //loop that calculates every number of duration as a percentage of the overall trips
    for(int i=0; i<24; i++)
    {
        duration[i]=((duration[i])/durationcount)*100;
    }
    print_dstats(duration);
    write_dstats(duration,durationcount);
}

/**
* print_dstats function: function that prints the results of DurationStats on the screen
* \param percent_v --> vector with the  needed percentage values from DurationStats
*/
void print_dstats(double percent_v[23])
{   //vector used to print illustrative bars
    int bar_count[23]= {0};
    int durationmin=0,durationhrs=0; //vars to where the minutes and hrs of the duration are stored/converted
    //top of the screen
    printf("======================================================================\n");
    printf("\t\t\t\tDURATION STATS\n");
    printf("======================================================================\n");
    //loop that prints all 23 time intervals
    for(int i=0; i<=23; i++)
    {   //however, they are only printed if their value is significant enough to the counting
        if(percent_v[i]>=0.01)
        {   //the min var goes beyond 60 it means that it can be converted to entire hours
            if(durationmin>=60)
            {   //hour var is increased and the correspondent in min is subtracted
                durationhrs++;
                durationmin-=60;
            } //prints the hours and mins of duration range
            printf("%dh %dmin - %dh %dmin\t",durationhrs,durationmin,durationhrs,durationmin+14);
            //because each bar represents 3%
            for(bar_count[i]=percent_v[i]/3; bar_count[i]>0; bar_count[i]--)
            {
                printf("|");
            }
            printf("  %.2f %%  \n",percent_v[i]);
            printf("----------------------------------------------------------------------\n");
        }
        durationmin=durationmin+15;
    }
    //bottom of the page
    printf("\t\t\t\t\t\tPress ENTER to leave\n");
    printf("======================================================================\n");
    getchar(); //wait for a key to be pressed and clears the screen
    system("clear");
    return;
}

/**
* write_dstats function: function that writes the results of DurationStats on .txt file
* \param percent_v --> vector with the  needed percentage values from DurationStats
* \param durationcount --> number of trips counted
*/
void write_dstats (double percent_v[23], double durationcount)
{
    int durationmin=0, durationhrs=0;
    //.txt file is created/opened and overwritten
    FILE* fptr=fopen("duration_stats.txt","w");
    //top of the page
    fprintf(fptr,"\t\t--Duration Stats--\n\n");
    //loop for all 23 time ranges (similar to the one in print_dstats
    for(int i=0; i<=23; i++)
    {
        if(percent_v[i]>=0.01)
        {
            if(durationmin>=60)
            {
                durationhrs++;
                durationmin-=60;
            }
            fprintf(fptr,"%dh %dmin - %dh %dmin:\t",durationhrs,durationmin,durationhrs,durationmin+14);
            fprintf(fptr,"%.2f %% (%.0f out of %.0f)\n",percent_v[i], (percent_v[i]/100)*durationcount, durationcount);
        }
        durationmin=durationmin+15;
    }
    //file is closed
    fclose(fptr);
}

/**
* AverageSpeedStats function: function that calculates the average of the average speeds and separates them by type, gender and age
* \param _triplist --> head node of the list (the list itself passed by reference)
*/
void AvgSpeedStats (tripnode** _triplist)
{
    tripnode* cur=*_triplist;
    double registered[1][80]= {{0}}, casual=0; //doubles used to count the sum of all average speeds depending on [a][b],c: a.gender b.age c.type
    double avgspeed_val, distance, time,temp; //temp avrg speed, distance, time and temp vars used to calculate each trip avg. speed
    double registeredcount[2][100]= {{0}}, casualcount=0; //registered count has a bigger size to prevent segmentation fault and both are used to count the number of items added to the previous vars
    int line,column;
    //loop to itinerate over the list to find lat and long values
    while(cur!=NULL)
    {   //distance is calculated through the haversine formula
        distance=GetDistance(cur->trip_file.start->lat,cur->trip_file.start->lng,cur->trip_file.stop->lat,cur->trip_file.stop->lng);
        //time is converted from m/s to km/h
        time=(cur->trip_file.timespan/3.6);
        //Avg.Speed=deltax/deltat
        avgspeed_val=distance/time;
        //in the case where the avgspeed value is valid and not 0 (i.e. nor time nor distance are 0)
        if(avgspeed_val!=0 && avgspeed_val==avgspeed_val )
        {   //if the user is Casual type casual variable is increased and so is its' count
            if(cur->trip_file.user.type==0)
            {
                casual+=avgspeed_val;
                casualcount+=1;
            }
            else
            {   //for registered users we use deltayears/2 to calculate every 2 year range and use it as 2nd var of the array
                line=(2017-cur->trip_file.user.birthyear)/2;
                column=cur->trip_file.user.gender; //we then use its gender to define the 1st var of the array
                temp=registered[column][line];      //avg speed and its count are updated
                registered[column][line]=avgspeed_val+temp;
                registeredcount[column][line]+=1;
            }
        }
        cur=cur->next;
    }
    casual=casual/casualcount; //calculates the average of avg. speeds in casual type

    for(int i=0; i<=1; i++)
        for(int j=0; j<80; j++)
            registered[i][j]=registered[i][j]/registeredcount[i][j]; //for every array entry the percentage is calculated

    print_avgsstats(registered,casual);
    write_avgsstats(registered,casual);
    return;
}

/**
* print_avgsstats function: function that prints the results of AvgSpeedStats on the screen
* \param avgspeed_v --> vector with the some of the needed avg speed values from AvgSpeedStats
* \param casual --> double with the casual avg speed
*/
void print_avgsstats (double avgspeed_v[][80], double casual)
{
    int age=0;
    //top of the screen
    printf("======================================================================\n");
    printf("\t\t\tAVERAGE SPEED STATS(km/h)\n");
    printf("======================================================================\n\n");
    //top of the table
    printf("\t\t|\t   Registered\t\t|\t\t|\n\t\t|    Female\t|\tMale\t|    Casual\t|\n");
    printf("----------------------------------------------------------------------\n");
    //used to itinerate over the array
    for(int j=0; j<=80; j++)
    {   //if one of the values is significant and both are valid
        if((avgspeed_v[0][j]>0 || avgspeed_v[1][j]) && (avgspeed_v[0][j]==avgspeed_v[0][j] && avgspeed_v[1][j]==avgspeed_v[1][j]))
        {   //values are printed
            printf("\t%d-%d\t|",age,(age+2));
            //because each bar represents 3%
            printf("    %.2f\t|   %.2f\t|    %.2f\t|\n",avgspeed_v[0][j],avgspeed_v[1][j],casual);
            printf("----------------------------------------------------------------------\n");
        }
        age=age+2;
    }
    //bottom of the screen
    printf("\t\t\t\t\t\tPress ENTER to leave\n");
    printf("======================================================================\n");
    getchar(); //waits for enter to be pressed and clears the screen
    system("clear");
    return;
}

/**
* write_avgsstats function: function that writes the results of AvgSpeedStats to a .txt file
* \param percent_v --> vector with some of the needed avg speed values from AvgSpeedStats
* \param casual --> double with the avg speed for casual users
*/
void write_avgsstats (double avgspeed_v[][80], double casual)
{
    int age=0;
    //file is created/overwritten
    FILE* fptr=fopen("avg_speed_stats.txt","w");
    //top of the page
    fprintf(fptr,"\t\t--Average Speed Stats--\n\n");
    //head of the table
    fprintf(fptr,"\t\t|\t   Registered  \t\t|\t\t\t|\n\t\t|  Female\t|\tMale\t|    Casual\t|\n");
    fprintf(fptr,"-------------------------------------------------------------------\n");
    //similar loop to the one used in print_avgsstats
    for(int j=0; j<=80; j++)
    {
        if((avgspeed_v[0][j]>0 || avgspeed_v[1][j]) && (avgspeed_v[0][j]==avgspeed_v[0][j] && avgspeed_v[1][j]==avgspeed_v[1][j]))
        {
            fprintf(fptr,"%d-%d   \t\t|",age,(age+2));

            fprintf(fptr,"    %.2f \t\t|   %.2f \t|    %.2f \t\t|\n",avgspeed_v[0][j],avgspeed_v[1][j],casual);
            fprintf(fptr,"-------------------------------------------------------------------\n");
        }
        age=age+2;
    }
    //file is closed
    fclose(fptr);
}

/**
* GetDistance function: function that calculates a distance between 1(lat,long) and 2(lat,long)
* \param lat1_deg, lat2_deg --> latitudes from both points in degrees
* \param long1_deg, long2_deg --> longitudes from both points in degrees
*/
double GetDistance (double lat1_deg, double long1_deg, double lat2_deg, double long2_deg)
{
    //variables utilized on the conversion and formulas
    double lat1_rad, long1_rad, lat2_rad, long2_rad, step1, step2, dist;
    //convert from degrees to radians
    lat1_rad=(lat1_deg*PI)/180;
    long1_rad=(long1_deg*PI)/180;
    lat2_rad=(lat2_deg*PI)/180;
    long2_rad=(long2_deg*PI)/180;
    //we use the haversine formula to calculate de distance of two points by its longitude and latitude
    step1=pow(sin((lat2_rad-lat1_rad)/2),2)+(cos(lat1_rad)*cos(lat2_rad)*pow(sin((long2_rad-long1_rad)/2),2));
    step2=2*atan2(sqrt(step1),sqrt(1-step1));
    dist=EARTH_RADIUS_M*step2;
    return dist;
}
