/*******************************************************************
* Author: cblythe
* Date: 8/25/2018
* Description: 
*******************************************************************/

#include "../headers/TicketSolution.h"

void TicketSolution::RunSolution()
{
    volatile atomic<int> *ticketTracker = new atomic<int>[numThreads];
    volatile atomic<bool> *flagTracker = new atomic<bool>[numThreads];

    for (int i =0; i < numThreads; i ++)
    {
        ticketTracker[i] = startNum;
        flagTracker[i] = false;
    }

    vector<thread> threads;
    vector<TicketThread*> ticketJobs;
    for (int i =0; i < numThreads; i++)
    {
        TicketThread *t = new TicketThread (numThreads, i, maxNumber, &primeSum, &primeNumbers, ticketTracker, flagTracker);
        threads.push_back(thread(&TicketThread::RunThread, t));
        ticketJobs.push_back(t);
    }

    for (auto& t: threads)
    {
        t.join();
    }

    for (auto& j: ticketJobs)
    {
        delete j;
    }
};

void TicketThread:: RunThread()
{
    int ticket = GetTicket();

    while(ticket <= maxNumber)
    {
        bool isPrime = IsPrime(ticket);

        if(isPrime)
        {
            CheckLine(ticket);

            ProcessPrime(ticket);

            LeaveLine();
        }

        ticket = GetTicket();
    }
}

int TicketThread::GetTicket()
{
    int ticket = ticketTracker[threadID];
    for (int other = 0; other < numThreads ; other++)
    {
        int otherTicket = ticketTracker[other];
        if (otherTicket >= ticket)
        {
            ticket = otherTicket +1;
        }
    }

    ticketTracker[threadID] = ticket;

    return ticket;
}

void TicketThread::CheckLine(int ticket)
{

    flagTracker[threadID] = true;

    for( int other = 0; other < numThreads ; other++)
    {
        if ( other != threadID)
        {
            int otherTicket = ticketTracker[other];
            //lexicographical ordering
            while(flagTracker[other] && (otherTicket < ticket || ( otherTicket == ticket && other < threadID)))
            {
                //Just waiting, nothing else to do here
            }
        }
    }
}

void TicketThread::ProcessPrime(int ticket)
{
    primeSum += long(ticket);
    primeNumbers->push_back(ticket);
    //cout <<"Thread #"<< threadID <<" found prime # "<< ticket<<"\n"<<flush;
}

void TicketThread::LeaveLine()
{
    flagTracker[threadID] = false;
}