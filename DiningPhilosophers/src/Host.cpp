/*******************************************************************
* Author: cblythe
* Date: 9/1/2018
* Description: 
*******************************************************************/
//

#include <cstdio>
#include "../headers/Host.h"

void Host::Start()
{
    while(!stop)
    {
        if(!stop)
        {
            AssignSeating();
        }

        if(!stop)
        {
            GetMinReservations();
        }
    }
}

void Host::Stop()
{
    stop = true;
}

int Host::GetMinReservations()
{
    int numFound = 0;
    for(int i = 0; i < numPhilosophers; i ++)
    {
        int resv = atomic_load(&reservations[i]);
        if (resv != -1)
        {
            acceptedReservations.push_back(pair<int, int>(resv, i));
            reservations[i] = -1;
            numFound ++;
        }
    }

    if(numFound > 0)
    {
        sort(acceptedReservations.begin(), acceptedReservations.end());
    }
}


void Host::AssignSeating()
{
    bool found = true;
    while(!stop && found)
    {
        found = chopstickFlag->exchange(found);
    }

    numAtTable = 0;

    for (int i = 0; i < tableCapacity; i++)
    {
        if(table[i] != -1)
        {
            numAtTable++;
        }
    }

    int reservationsAccepted = acceptedReservations.size();

    for (int i = 0; i < tableCapacity && reservationsAccepted > 0; i += 2)
    {
        if (table[i] == -1)
        {
            numAtTable++;
            int index = acceptedReservations[0].second;
            seating[index] = i;
            table[i] = index;
            acceptedReservations.pop_front();
            reservationsAccepted --;
        }
    }

    for (int i = 1; i < tableCapacity && reservationsAccepted > 0; i += 2)
    {
        if (table[i] == -1)
        {
            numAtTable++;
            int index = acceptedReservations[0].second;
            seating[index] = i;
            table[i] = index;
            acceptedReservations.pop_front();
            reservationsAccepted --;
        }
    }

    *chopstickFlag = false;
}
