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
    bool found = true;
    while(found)
    {
        found = reservationFlag->exchange(found);
    }

    int numFound = 0;
    for(int i = 0; i < numPhilosophers && numFound < tableCapacity - numAtTable; i ++)
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

    *reservationFlag = false;
}


void Host::AssignSeating()
{
    bool found = true;
    while(found)
    {
        found = chopstickFlag->exchange(found);
    }

    numAtTable = 0;

    for (int i = 0; i < tableCapacity; i++)
    {
        int seatedPhilosopher = table[i];

        if( seatedPhilosopher != -1)
        {
            numAtTable++;
            int left = chopsticks[i];
            int right = chopsticks[i+1];

            if((left == -1 && right == -1))
            {
                chopsticks[i] = seatedPhilosopher;
                chopsticks[i+1] = seatedPhilosopher;
                reservations[seatedPhilosopher] = i;
            }
        }
    }

    int seatsAvailable = tableCapacity - numAtTable;
    int reservationsAvailable = acceptedReservations.size();

    for(int i = 0; i < reservationsAvailable && i < seatsAvailable; i++)
    {
        bool inserted = false;
        for(int j = 0; j < tableCapacity; j++)
        {
            if(table[j] == -1 && chopsticks[j] == -1 && chopsticks[j + 1] == -1)
            {
                numAtTable++;
                int index = acceptedReservations[0].second;
                table[j] = index;
                chopsticks[j] = index;
                chopsticks[j+1] = index;
                reservations[index] = j;
                acceptedReservations.pop_front();
                inserted = true;
                break;
            }
        }

        for(int j = 0; j < tableCapacity && !inserted; j++)
        {
            if(table[j] == -1)
            {
                numAtTable++;
                int index = acceptedReservations[0].second;
                table[j] = index;
                acceptedReservations.pop_front();
                break;
            }
        }
    }

    *chopstickFlag = false;
}
