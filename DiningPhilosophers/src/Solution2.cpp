/*******************************************************************
* Author: cblythe
* Date: 8/31/2018
* Description: Concurrent Solution to Dining Philosophers WITHOUT Deadlock
*******************************************************************/
//

#include "../headers/Solution2.h"

Philosopher **philosophers;
volatile atomic<int> *chopsticks;
const int numChopSticks = 5;
const int numPhilosophers= 5;
const State initialState = State::Thinking;

vector<thread> threads;

int main()
{
    Initialize();

    cout << "Welcome to the Dining Philosophers restaurant, please press 'n' to stop.\n";

    Run(initialState);

    string in;
    do { cin >> in; } while(in != "n");

    Stop();

    Cleanup();

    cout << "Thanks for visiting, please press 'q' to leave.\n";

    do { cin >> in; } while(in != "q");
}

void Initialize()
{
    chopsticks = new volatile atomic<int>[numChopSticks];
    for(int i=0; i < numChopSticks; i++)
    {
        chopsticks[i] = -1;
    }

    philosophers = new Philosopher*[numPhilosophers];
    for(int i = 0; i < numPhilosophers; i ++)
    {
        philosophers[i] = new Philosopher2(i, chopsticks, numChopSticks);
    }
}

//Creates and runs each thread as a philosopher calling Start with an initial state
void Run(State _initialState)
{
    threads.clear();
    for (int i =0; i < numPhilosophers; i++)
    {
        threads.push_back(thread(&Philosopher::Start, philosophers[i], _initialState));
    }
}

void Stop()
{
    for (int i =0; i < numPhilosophers; i++)
    {
        philosophers[i]->Stop();
        threads[i].join();
    }

    printf("\n--------------------------------------------------------------------------\n");
    for (int i =0; i < numPhilosophers; i++)
    {
        printf("Philosopher #%d ate a total of %d times.\n", i, philosophers[i]->GetTimesEaten());
    }
}

//Try and stop those memory leaks
void Cleanup()
{
    delete [] chopsticks;

    for(int i = 0; i < numPhilosophers; i ++)
    {
        delete philosophers[i] ;
    }

    delete philosophers;
}

//Concurrent solution for deadlock where each thread tries to grab chopsticks in a structured and ordered manner.
//When selecting the first chopstick the philosopher first takes into account it's lowest number to select.
//For the philosophers in the middle this will be the left chopstick, but the philosopher on the end will try
//to grab the first chopstick. If the first chopstick is in use by the first philosopher then the last will
//just continue to wait, but the previous threads can continue on merrily. This is deadlock free, but NOT starvation free
//as the last thread or any of the others could be waiting indifinetely as a thread keeps taking the resource before
//they can get to it.
void Philosopher2::Hungry()
{
    currentState = State::Hungry;
    printf("Philosopher %d is now hungry.\n", philosopherID);

    leftChopstick = -1;
    rightChopstick = -1;

    // grab smallest chopstick first
    int target1 = min(this->philosopherID, (this->philosopherID + 1) % this->numChopsticks);
    // grab largest chopstick second
    int target2 = max(this->philosopherID, (this->philosopherID + 1) % this->numChopsticks);;

    //keep trying till you have both chopsticks.
    while(!stop && (leftChopstick < 0 || rightChopstick < 0))
    {
        if(leftChopstick < 0)
        {
            WaitForChopstick(target1);
            leftChopstick = target1;
        }

        if(leftChopstick >= 0 && rightChopstick < 1)
        {
            WaitForChopstick(target2);
            rightChopstick = target2;
        }
    }

    if(!stop)
    {
        currentState = State::Eating;
    }
}

//Attempts to grab a chopstick by using compare_exchange. If the chopstick value is -1 then we exchange our philosopher ID,
//otherwise the thread keeps waiting.
void Philosopher2::WaitForChopstick(int target)
{

    bool available = false;
    while(!stop && !available)
    {
        int desired = -1;
        atomic_compare_exchange_strong(&chopsticks[target], &desired, this->philosopherID);
        available = desired == -1;
    }

    if (!stop)
    {
        printf("\tPhilosopher #%d now has chopstick #%d.\n", this->philosopherID, target);
    }
}

//Leave the table by setting down both chopsticks.
void Philosopher2::LeaveTable()
{
    atomic_store(&chopsticks[leftChopstick],-1);
    atomic_store(&chopsticks[rightChopstick],-1);

    printf("\tPhilosopher #%d now has left chopsticks #%d and #%d.\n", this->philosopherID, leftChopstick, rightChopstick);
}




