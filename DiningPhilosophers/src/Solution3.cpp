/*******************************************************************
* Author: cblythe
* Date: 8/31/2018
* Description: 
*******************************************************************/
//

#include "../headers/Solution3.h"
#include "../headers/Host.h"

const int numChopSticks = 5;
const int numPhilosophers= 5;
const State initialState = State::Thinking;

Philosopher **philosophers;
volatile atomic<bool> reservationFlag;
volatile atomic<bool> chopstickFlag;
volatile atomic<int> *reservations;
volatile atomic<int> *table;
volatile atomic<int> *chopsticks;
Host *host;

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
    reservationFlag = false;
    chopstickFlag = false;

    chopsticks = new volatile atomic<int>[numChopSticks];
    table = new volatile atomic<int> [numChopSticks - 1];
    for(int i=0; i < numChopSticks; i++)
    {
        chopsticks[i] = -1;

        if(i < numChopSticks -1)
        {
            table[i] = -1;
        }
    }
    reservations = new volatile atomic<int> [numPhilosophers];
    for(int i = 0; i < numPhilosophers; i ++)
    {
       reservations[i] = -1;
    }

    host = new Host(chopsticks, reservations, table, &reservationFlag, &chopstickFlag, numChopSticks, numPhilosophers);


    philosophers = new Philosopher*[numPhilosophers];
    for(int i = 0; i < numPhilosophers; i ++)
    {
        philosophers[i] = new Philosopher3(i, chopsticks, reservations, table, &reservationFlag, &chopstickFlag, numChopSticks);
    }

}

void Run(State _initialState)
{
    threads.clear();

    threads.push_back(thread(&Host::Start, host));

    for (int i =0; i < numPhilosophers; i++)
    {
        threads.push_back(thread(&Philosopher::Start, philosophers[i], _initialState));
    }
}

void Stop()
{
    host->Stop();
    threads[0].join();

    for (int i =0; i < numPhilosophers; i++)
    {
        philosophers[i]->Stop();
        threads[i+1].join();
    }

    printf("\n--------------------------------------------------------------------------\n");
    for (int i =0; i < numPhilosophers; i++)
    {
        printf("Philosopher #%d ate a total of %d times.\n", i, philosophers[i]->GetTimesEaten());
    }
}

void Cleanup()
{
    delete [] chopsticks;
    delete [] reservations;
    delete [] table;

    for(int i = 0; i < numPhilosophers; i ++)
    {
        delete philosophers[i] ;
    }

    delete philosophers;
}

void Philosopher3::Hungry()
{
    printf("Philosopher %d is now hungry.\n", philosopherID);

    int seat = -1;
    if(!stop)
    {
        seat = GetInLine();
    }

    WaitForChopstick(seat);
    WaitForChopstick(seat + 1);

    leftChopstick = seat;
    rightChopstick = seat+1;

    currentState = State::Eating;
}

int Philosopher3::GetInLine()
{
    bool found = true;
    while(found)
    {
        found = reservationFlag->exchange(found);
    }
    reservations[philosopherID] = timesEaten;

    *reservationFlag = false;

    int te  = timesEaten;
    while(!stop && te == timesEaten)
    {
        te = reservations[philosopherID];
    }

    int seat = -1;
    while(!stop && seat == -1)
    {
        seat = reservations[philosopherID].exchange(-1);
    }

    return seat;
}

void Philosopher3::WaitForChopstick(int target)
{
    int available = -1;
    while (!stop && available != philosopherID)
    {
        available = chopsticks[target];
    }

    if (!stop)
    {
        printf("\tPhilosopher #%d now has chopstick #%d.\n", this->philosopherID, target);
    }
}

void Philosopher3::LeaveTable()
{
    bool found = true;
    while(found)
    {
        found = chopstickFlag->exchange(found);
    }

    if (!stop)
    {
        printf("\tPhilosopher #%d now has left chopsticks #%d and #%d.\n", this->philosopherID, leftChopstick,
               rightChopstick);

        atomic_store(&table[leftChopstick], -1);
        atomic_store(&chopsticks[leftChopstick], -1);
        atomic_store(&chopsticks[rightChopstick], -1);
        *chopstickFlag = false;
    }
}




