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
         volatile atomic<bool> *_reservationFlag, volatile atomic<bool> *_chopstickFlag, int _numChopsticks, int _numPhilosophers)
    {
        numChopsticks = _numChopsticks;
        numPhilosophers = _numPhilosophers;
        tableCapacity = numChopsticks - 1;
        numAtTable = 0;

        chopsticks = _chopsticks;
        reservations = _reservations;
        table = _table;
        chopstickFlag = _chopstickFlag;
        reservationFlag = _reservationFlag;
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

private:
    int GetMinReservations();

private:
    int numChopsticks = 0;
    int numPhilosophers = 0;
    int tableCapacity = 0;
    int numAtTable = 0;
    int *dirtyChopsticks;
    deque <pair<int,int>> acceptedReservations;
    volatile atomic<bool> *reservationFlag;
    volatile atomic<bool> *chopstickFlag;
    volatile atomic<int> *reservations;
    volatile atomic<int> *table;
    volatile atomic<int> *chopsticks;
    bool stop = false;

};

#endif //DININGPHILOSOPHERS_HOST_H
