/*******************************************************************
* Author: cblythe
* Date: 9/1/2018
* Description: 
*******************************************************************/
//

#ifndef DININGPHILOSOPHERS_HOST_H
#define DININGPHILOSOPHERS_HOST_H

#include <atomic>
#include <deque>
#include <algorithm>


using namespace std;

class Host
{
public:
    Host(volatile atomic<int> *_chopsticks, volatile atomic<int> *_reservations, volatile atomic<int> *_table,
         volatile atomic<int> *_seating, volatile atomic<bool> *_chopstickFlag, int _numChopsticks, int _numPhilosophers)
    {
        numChopsticks = _numChopsticks;
        numPhilosophers = _numPhilosophers;
        tableCapacity = numChopsticks - 1;
        numAtTable = 0;

        seating = _seating;
        reservations = _reservations;
        table = _table;
        chopstickFlag = _chopstickFlag;
        stop = false;


        dirtyChopsticks = new int[numChopsticks];
        for (int i = 0; i < numChopsticks; i ++)
        {
            dirtyChopsticks[i] = -1;
        }
    };

    void Start();

    void Stop();

    void AssignSeating();

    void CleanTable();

private:
    int GetMinReservations();

private:
    int numChopsticks = 0;
    int numPhilosophers = 0;
    int tableCapacity = 0;
    int numAtTable = 0;
    int *dirtyChopsticks;
    deque <pair<int,int>> acceptedReservations;
    volatile atomic<bool> *chopstickFlag;
    volatile atomic<int> *reservations;
    volatile atomic<int> *table;
    volatile atomic<int> *seating;
    bool stop = false;

};

#endif //DININGPHILOSOPHERS_HOST_H
