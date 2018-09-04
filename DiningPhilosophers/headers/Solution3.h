/*******************************************************************
* Author: cblythe
* Date: 8/31/2018
* Description: 
*******************************************************************/
//

#ifndef DININGPHILOSOPHERS_SEQUENTIAL_H
#define DININGPHILOSOPHERS_SEQUENTIAL_H

#include "Philosopher.h"
#include "Host.h"
#include <iostream>
#include <vector>
#include <atomic>

using namespace std;

void Initialize();

void Run(State _initialState);

void Stop();

void Cleanup();


class Philosopher3: public Philosopher
{
public:
    Philosopher3(int _id, volatile atomic<int> *_chopsticks, volatile atomic<int> *_reservations, volatile atomic<int> *_table,
            volatile atomic<bool> *_reservationFlag, volatile atomic<bool> *_chopstickFlag, int _numChopsticks):
    Philosopher(_id, _chopsticks, _numChopsticks)
    {
        chopsticks = _chopsticks;
        reservations = _reservations;
        table = _table;
        chopstickFlag = _chopstickFlag;
        reservationFlag = _reservationFlag;
    };

    void Hungry() override;

    int GetInLine();

    int SitAtTable();

    void WaitForChopstick(int target);

    void LeaveTable() override;

private:
    volatile atomic<bool> *reservationFlag;
    volatile atomic<bool> *chopstickFlag;
    volatile atomic<int> *reservations;
    volatile atomic<int> *table;
    volatile atomic<int> *chopsticks;
};


#endif //DININGPHILOSOPHERS_SEQUENTIAL_H
