/*******************************************************************
* Author: cblythe
* Date: 8/25/2018
* Description: 
*******************************************************************/


#ifndef COP6616_ASSIGNMENT1_TICKETSOLUTION_H
#define COP6616_ASSIGNMENT1_TICKETSOLUTION_H

#include "Solution.h"
#include <thread>
#include <atomic>

class TicketSolution: public Solution
{
public:
    TicketSolution(int t, string name, const int maxNum): Solution(name,maxNum)
    {
        numThreads = t;
    }

    void RunSolution() override;

    ~TicketSolution()
    {
    }

private:
    int numThreads = 0;
};

class TicketThread
{
public:
    TicketThread(int totalThreads, int thread, int target, long *sum, vector<int>* numbers,
            volatile atomic<int> *tickets, volatile atomic<bool> *flags, volatile atomic<int> *ticket)
    {

        numThreads = totalThreads;
        threadID = thread;
        maxNumber = target;
        primeSum = sum;
        primeNumbers = numbers;
        ticketTracker = tickets;
        flagTracker = flags;
        ticketCounter = ticket;

    };

    void RunThread();

private:
    int threadID = 0;
    int numThreads = 1;
    int maxNumber = 0;
    volatile atomic<int> *ticketCounter;
    long *primeSum;
    vector<int> *primeNumbers;
    volatile atomic<int> *ticketTracker;
    volatile atomic<bool> *flagTracker;

    int GetTicket();
    bool CheckLine(int ticket);
    void ProcessPrime(int ticket);
    void LeaveLine();

};


#endif //COP6616_ASSIGNMENT1_TICKETSOLUTION_H

