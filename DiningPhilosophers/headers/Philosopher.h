/*******************************************************************
* Author: cblythe
* Date: 8/31/2018
* Description: 
*******************************************************************/
//

#ifndef DININGPHILOSOPHERS_PHILOSOPHER_H
#define DININGPHILOSOPHERS_PHILOSOPHER_H

#include <thread>
#include <atomic>

using namespace std;

enum State {Init = 0, Thinking = 1,Hungry = 2, Eating = 3};

class Philosopher
{
public:

    Philosopher(int _id, volatile atomic<int> *_chopSticks, int _numChopsticks)
    {
        currentState = State::Init;
        philosopherID = _id;

        numChopsticks = _numChopsticks;
        chopsticks = _chopSticks;

        stop = false;
    };

    void Start(State _initialState)
    {
      currentState = _initialState;
      MainLoop();
    };

    void Stop()
    {
        stop = true;
    };

    int GetTimesEaten()
    {
        return timesEaten;
    }

    ~Philosopher()
    {
    }

protected:


    void MainLoop()
    {
        while(!stop)
        {
            if (currentState == State::Thinking)
            {
                Thinking();
            }
            else if (currentState == State::Hungry)
            {
                Hungry();
            }
            else if (currentState == State::Eating)
            {
                Eat();
            }
        }
    }

    void Thinking()
    {
        printf("Philosopher %d is now thinking.\n", philosopherID);
        this_thread::sleep_for(chrono::milliseconds(thinkingTime));
        currentState = State::Hungry;
    };

    virtual void Hungry()
    {
        printf("Philosopher %d is now hungry.\n", philosopherID);
    };

    void Eat()
    {
        printf("Philosopher %d is now eating for the %d time.\n", philosopherID, timesEaten + 1);
        this_thread::sleep_for(chrono::milliseconds(eatingTime));
        timesEaten ++;
        LeaveTable();
        currentState = State::Thinking;
    };

    virtual void LeaveTable()
    {

    };

    bool stop;
    int timesEaten = 0;
    int philosopherID = 0;
    int numChopsticks = 0;
    int leftChopstick = -1;
    int rightChopstick = -1;
    volatile atomic<int>* chopsticks;

    const int thinkingTime = int(1000 * 0.1);
    const int eatingTime = int(1000* 0.1);

    State currentState;

};


#endif //DININGPHILOSOPHERS_PHILOSOPHER_H
