#ifndef LISTINGS_H_INCLUDED
#define LISTINGS_H_INCLUDED
#include "trips.h"

void routelisting(tripnode**);
tripnode* SortTripList(tripnode**,int);
int order_bynumber(tripnode**,tripnode*,int[]);
void print_routelist(tripnode**,tripnode*,int, int, int[], int[]);
void statisticslisting(void);
void triplisting(tripnode**);
void stationlisting(void);

#endif // LISTINGS_H_INCLUDED
