/*******************************************************************
* Author: cblythe
* Date: 8/31/2018
* Description: 
*******************************************************************/
//

#ifndef DININGPHILOSOPHERS_SEQUENTIAL_H
#define DININGPHILOSOPHERS_SEQUENTIAL_H

#include "Philosopher.h"
#include <iostream>
#include <vector>
#include <atomic>

using namespace std;

void Initialize();

void Run(State _initialState);

void Stop();

void Cleanup();


class Philosopher1: public Philosopher
{
public:
    Philosopher1(int _id, volatile atomic<int> *_chopSticks, int _numChopsticks):
    Philosopher(_id, _chopSticks, _numChopsticks)
    {

    };

    void Hungry() override;

    void WaitForFork(int target);

    void LeaveTable() override;
};


#endif //DININGPHILOSOPHERS_SEQUENTIAL_H
