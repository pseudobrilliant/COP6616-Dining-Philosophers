/*******************************************************************
* Author: cblythe
* Date: 8/25/2018
* Description: 
*******************************************************************/

#include <sstream>
#include "../headers/TicketSolution.h"

void TicketSolution::RunSolution()
{
    volatile atomic<int> *ticketTracker = new atomic<int>[numThreads];
    volatile atomic<int> *ticket = new atomic<int>();
    volatile atomic<bool> *flagTracker = new atomic<bool>[numThreads];

    for (int i =0; i < numThreads; i ++)
    {
        ticketTracker[i] = startNum;
        flagTracker[i] = false;
    }

    *ticket = startNum;

    primeSum = 0;

    vector<thread> threads;
    vector<TicketThread*> ticketJobs;
    for (int i =0; i < numThreads; i++)
    {
        TicketThread *t = new TicketThread (numThreads, i, maxNumber, &primeSum, &primeNumbers, ticketTracker, flagTracker, ticket);
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
        if(IsPrime(ticket) && CheckLine(ticket))
        {
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

    return ticketTracker[threadID];
}

bool TicketThread::CheckLine(int ticket)
{

    flagTracker[threadID] = true;

    for( int other = 0; other < numThreads ; other++)
    {
        if ( other != threadID)
        {

            if( ticketTracker[other] == ticket && other < threadID)
            {
                return false;
            }

            //lexicographical ordering
            while(flagTracker[other] && (ticketTracker[other] < ticket))
            {
                //Just waiting, nothing else to do here
            }
        }
    }

    return true;
}

void TicketThread::ProcessPrime(int ticket)
{
    *primeSum += long(ticket);
    primeNumbers->push_back(ticket);
    /*stringstream str;
    str<<"Thread #"<< threadID <<" found prime # "<< ticket<<"\n"<<flush;
    str<< "Ticket Tracker State: ";
    for(int i =0; i < numThreads; i++)
    {
        cout<< ticketTracker[i] << " ";
    }
    str<<"\n";

    cout<<str.str()<<flush;*/

}

void TicketThread::LeaveLine()
{
    flagTracker[threadID] = false;
}