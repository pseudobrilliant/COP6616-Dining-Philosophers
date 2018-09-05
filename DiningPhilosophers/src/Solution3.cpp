/*******************************************************************
 * Author: cblythe
 * Date: 8/31/2018
 * Description: Concurrent solution to Dining Philosophers with no Deadlock or Starvation.
 * This solution differs from the others in that it uses a host thread manager to manage
 * how and when threads can have access to the resources based on priority.
 * To facilitate this 3 more shared resource arrays and one flag are introduced. When hungry,
 * Philosophers start out by placing the number of times they have eaten in the reservation table
 * at the index equal to their philosopherID. The host will eventually see their reservation and store it
 * in a private vector sorted by times eaten and the philosopher' ID, while resetting the value to -1.
 * The host will then turn to assigning seats to waiting philosophers. The host first locks the chopsticks to take stock of
 * the empty seats, then it begins assigning seats by priority in the queue. Seat assignments are reported
 * to the philosopher through a shared seating resource, where each philosopher has an index. Once a
 * Philosophers who have noticed their reservation has been reset to -1 will continously read this seating array
 * until they have received a seat, which they then reset to -1. Having succesfully acquired a seat, the
 * philosopher then attempts to grab the lowest fork related to their seat first, before grabbing the highest.
 * Once the philosopher has both forks, he eats, and eventually leaves the table. Before he can leave the table he must wait
 * until the host has finished checking the chopsticks, after which he is free to reset the chopsticks and mark his seat as empty.
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
    table = new volatile atomic<int> [numChopSticks];
    for(int i=0; i < numChopSticks; i++)
    {
        chopsticks[i] = -1;
        table[i] = -1;
    }
    seatings = new volatile atomic<int> [numPhilosophers];
    reservations = new volatile atomic<int> [numPhilosophers];
    for(int i = 0; i < numPhilosophers; i ++)
    {
       reservations[i] = -1;
       seatings[i] = -1;
    }

    //Creates host
    host = new Host(chopsticks, reservations, table, seatings, &chopstickFlag, numChopSticks, numPhilosophers);

    //Create N threads
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

//Philosopher gets to eat by setting his reservation, waiting for his seat, and finally waiting for both
//of his chopsticks.
void Philosopher3::Hungry()
{
    printf("Philosopher %d is now hungry.\n", philosopherID);

    if(!stop)
    {
        int seat = GetInLine();

        // grab smallest chopstick first
        int target1 = min(seat, (seat + 1) % this->numChopsticks);
        // grab largest chopstick second
        int target2 = max(seat, (seat + 1) % this->numChopsticks);;

        WaitForChopstick(target1);
        WaitForChopstick(target2);

        leftChopstick = target1;
        rightChopstick = target2;
        currentState = State::Eating;
    }
}

//Philosopher first sets his reservation, and then waits for confimation.
// After which he wait for a seating, and resets the index to -1.
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




