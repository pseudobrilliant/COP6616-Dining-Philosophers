/*******************************************************************
 * Author: cblythe
 * Date: 8/31/2018
 * Description: Base class of philosopher that contains basic implementations of:
 * A thread start function, a main loop that perpetually runs the philosopher logic,
 * and functions for eating / thinking. A virtual Hungry loop is provided so that all
 * philosophers can be hungry with the implementation left up to the child class.
 * A virtual LeaveTable is also provided so each child class can leave the table however it best sees fit.
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

    //At minimum each philosopher needs an id and link to grab chopsticks
    Philosopher(int _id, volatile atomic<int> *_chopSticks, int _numChopsticks)
    {
        currentState = State::Init;
        philosopherID = _id;

        numChopsticks = _numChopsticks;
        chopsticks = _chopSticks;

        stop = false;
    };

    //Starts the loop with an initial state
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

    //Each loop a a state function is selected, this function will modify the state to be picked up by the next loop
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

    //Thinks for thinkingTime and then becomes hungry
    void Thinking()
    {
        printf("Philosopher %d is now thinking.\n", philosopherID);
        this_thread::sleep_for(chrono::milliseconds(thinkingTime));
        currentState = State::Hungry;
    };

    //To be overwritten by child implementation
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

    //To be overwritten by child implementation
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
    const int eatingTime = int(1000* 0.15);

    State currentState;

};


#endif //DININGPHILOSOPHERS_PHILOSOPHER_H
