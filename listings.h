#ifndef LISTINGS_H_INCLUDED
#define LISTINGS_H_INCLUDED
#include "trips.h"
#include <math.h>
#define EARTH_RADIUS_M 6371000
#define PI 3.14159265
int system(const char *command);

void routelisting(tripnode**);
tripnode* SortTripList(tripnode**,int);
int order_bynumber(tripnode**,tripnode*,int[]);
void print_routelist(tripnode**,tripnode*,int, int, int[], int[]);

void statisticslisting(tripnode**);
void GenderStats (tripnode**);
void print_gstats (double[]);
void write_gstats (double[],double);
void AgeStats (tripnode**);
void print_astats (double[]);
void write_astats (double[],double);
void DurationStats (tripnode**);
void print_dstats (double[]);
void write_dstats (double[],double);
void AvgSpeedStats (tripnode**);
void print_avgsstats (double[][80],double);
void write_avgsstats (double[][80],double);
double GetDistance (double, double, double, double);

void triplisting(tripnode**);
void stationlisting(tripnode**,stationnode**);

#endif // LISTINGS_H_INCLUDED
