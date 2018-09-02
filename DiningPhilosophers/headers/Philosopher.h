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
      SwitchState(_initialState);
    };

    void Stop()
    {
        stop = true;
    };


    State GetState()
    {
        return currentState;
    }

    long GetTimesEaten()
    {
        return timesEaten;
    }

    ~Philosopher()
    {
    }

protected:


    void SwitchState( State target)
    {
        if(stop)
        {
            return;
        }

        if(target == State::Thinking)
        {
            Thinking();
        }
        else
        if(target == State::Hungry)
        {
            Hungry();
        }
        else
        if(target == State::Eating)
        {
            Eat();
        }
    }

    void Thinking()
    {
        currentState = State::Thinking;
        printf("Philosopher %d is now thinking.\n", philosopherID);
        this_thread::sleep_for(chrono::milliseconds(thinkingTime));
        Hungry();
    };

    virtual void Hungry()
    {
        currentState = State::Hungry;
        printf("Philosopher %d is now hungry.\n", philosopherID);
    };

    void Eat()
    {
        currentState = State::Eating;
        printf("Philosopher %d is now eating for the %d time.\n", philosopherID, timesEaten);
        this_thread::sleep_for(chrono::milliseconds(eatingTime));
        timesEaten ++;
        LeaveTable();
        Thinking();
    };

    virtual void LeaveTable()
    {

    };

    bool stop;
    long timesEaten = 0;
    int philosopherID = 0;
    int numChopsticks = 0;
    int leftChopstick = -1;
    int rightChopstick = -1;
    volatile atomic<int>* chopsticks;

    const int thinkingTime = int(1000 * 0.3);
    const int eatingTime = int(1000* 0.2);

    State currentState;

};


#endif //DININGPHILOSOPHERS_PHILOSOPHER_H
