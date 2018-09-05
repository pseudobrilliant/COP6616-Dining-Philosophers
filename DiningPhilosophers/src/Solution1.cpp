/*******************************************************************
* Author: cblythe
* Date: 8/31/2018
* Description: Concurrent solution to Dining Philosophers with Deadlock
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

    cout << "Thanks for visiting, please press 'q' to leave.\n";

    do { cin >> in; } while(in != "q");
}

//Initializes the chopsticks atomic values to -1
void Initialize()
{
    chopsticks = new volatile atomic<int>[numChopSticks];
    for(int i=0; i < numChopSticks; i++)
    {
        chopsticks[i] = -1;
    }

    //Creates philosophers
    philosophers = new Philosopher*[numPhilosophers];
    for(int i = 0; i < numPhilosophers; i ++)
    {
        philosophers[i] = new Philosopher1(i, chopsticks, numChopSticks);
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

//Concurrent solution for deadlock where each thread tries to grab the left chopstick of it's index first and
//then the one ot the right. However, n

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

//Attempts to grab a chopstick by using compare_exchange. If the chopstick value is -1 then we exchange our philosopher ID,
//otherwise the thread keeps waiting.
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
//Leave the table by setting down both chopsticks.
void Philosopher1::LeaveTable()
{
    atomic_store(&chopsticks[leftChopstick],-1);
    atomic_store(&chopsticks[rightChopstick],-1);

    printf("\tPhilosopher #%d now has left chopsticks #%d and #%d.\n", this->philosopherID, leftChopstick, rightChopstick);
}




