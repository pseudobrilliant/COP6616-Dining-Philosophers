/*******************************************************************
* Author: cblythe
* Date: 8/31/2018
* Description: 
*******************************************************************/
//

#include "../headers/Solution1.h"

Philosopher **philosophers;
volatile atomic<int> *chopsticks;
const int numChopSticks = 5;
const int numPhilosophers= 5;
const State initialState = State::Thinking;

vector<thread> threads;

int main()
{
    Initialize();

    cout << "Welcome to the Dining Philosophers restaurant, please press 'n' to leave.\n";

    Run(initialState);

    string in;
    do { cin >> in; } while(in != "n");

    Stop();

    Cleanup();
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
        philosophers[i] = new Philosopher1(i, chopsticks, numChopSticks);
    }
}

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
        printf("Philosopher #%d ate a total of %ld times.\n", i, philosophers[i]->GetTimesEaten());
    }
}

void Cleanup()
{
    delete [] chopsticks;

    for(int i = 0; i < numPhilosophers; i ++)
    {
        delete philosophers[i] ;
    }

    delete philosophers;
}

void Philosopher1::Hungry()
{
    currentState = State::Hungry;
    printf("Philosopher %d is now hungry.\n", philosopherID);

    leftChopstick = -1;
    rightChopstick = -1;
    int target1 = this->philosopherID;
    int target2 = (target1 + 1) % this->numChopsticks;
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

    currentState = State::Eating;
}

void Philosopher1::WaitForChopstick(int target)
{

    bool available = false;
    while (!stop && !available)
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

void Philosopher1::LeaveTable()
{
    atomic_store(&chopsticks[leftChopstick],-1);
    atomic_store(&chopsticks[rightChopstick],-1);

    printf("\tPhilosopher #%d now has left chopsticks #%d and #%d.\n", this->philosopherID, leftChopstick, rightChopstick);
}




