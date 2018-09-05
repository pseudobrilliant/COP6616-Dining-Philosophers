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
volatile atomic<bool> chopstickFlag;
volatile atomic<int> *reservations;
volatile atomic<int> *seatings;
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
    seatings = new volatile atomic<int> [numPhilosophers];
    reservations = new volatile atomic<int> [numPhilosophers];
    for(int i = 0; i < numPhilosophers; i ++)
    {
       reservations[i] = -1;
       seatings[i] = -1;
    }

    host = new Host(chopsticks, reservations, table, seatings, &chopstickFlag, numChopSticks, numPhilosophers);


    philosophers = new Philosopher*[numPhilosophers];
    for(int i = 0; i < numPhilosophers; i ++)
    {
        philosophers[i] = new Philosopher3(i, chopsticks, reservations, table, seatings, &chopstickFlag, numChopSticks);
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

    if(!stop)
    {
        int seat = GetInLine();

        if (seat % 2 == 0)
        {
            WaitForChopstick(seat + 1);
            WaitForChopstick(seat);
        }
        else
        {
            WaitForChopstick(seat);
            WaitForChopstick(seat + 1);
        }

        leftChopstick = seat;
        rightChopstick = seat + 1;
        currentState = State::Eating;
    }
}

int Philosopher3::GetInLine()
{
    reservations[philosopherID] = timesEaten;

    int te  = timesEaten;
    while(!stop && te == timesEaten)
    {
        te = reservations[philosopherID];
    }

    int seat = -1;
    while(seat == -1 && !stop)
    {
        seat = seatings[philosopherID].exchange(seat);
    }

    return seat;
}

void Philosopher3::WaitForChopstick(int target)
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

void Philosopher3::LeaveTable()
{
    bool found = true;
    while(!stop && found)
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




