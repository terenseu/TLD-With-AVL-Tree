//
//  LOW SING WEE, TERENCE
//  2110003L
//  AP3 Excercise 1
//  This is my own work as defined in the Academic Ethics agreement I have signed.
//
//  date.c

#include <stdio.h>
#include "date.h"
#include <string.h>
#include <stdlib.h>
#include <regex.h>

struct date
{
	int day;
	int month;
	int year;
};

Date *date_create(char *datestr){
	
	Date *p = (Date *) malloc (sizeof(Date));
	
    if (p != NULL)
    {
        sscanf(datestr, "%2d/%2d/%4d", &p->day, &p->month, &p->year);
    }
    else
    {
        date_destroy(p);
    }
    
    return p;
}


Date *date_duplicate(Date *d)
{
    Date *dupdate = (Date *) malloc (sizeof(d));
    if (dupdate != NULL)
    {
        return memcpy(dupdate, d, sizeof(*dupdate));
    }
    else
    {
        date_destroy(dupdate);
        return NULL;
    }
}

int date_compare(Date *date1, Date *date2)
{
    if (date1 != NULL && date2 != NULL)
    {
        int reR = 0;
        
        if (date1->year == date2->year)
        {
            
            if(date1->month == date2->month)
            {
                
                if(date1->day == date2->day)
                {
                    reR = 0;
                } else if (date1->day > date2->day)
                {
                    reR = 1;
                } else if (date1->day < date2->day)
                {
                    reR = -1;
                }
            } else if (date1->month > date2->month)
            {
                reR = 1;
            } else if (date1->month < date2->month)
            {
                reR = -1;
            }
        } else if (date1->year > date2->year)
        {
            reR = 1;
        } else if (date1->year < date2->year)
        {
            reR = -1;
        }
        return reR;
    }
    else
    {
        return 1;
    }
}

void date_destroy(Date *d) //free date memory
{
    free(d);
}



