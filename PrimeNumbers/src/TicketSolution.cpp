/*******************************************************************
* Author: cblythe
* Date: 8/25/2018
* Description: 
*******************************************************************/

#include <sstream>
#include "../headers/TicketSolution.h"
#include <algorithm>

void TicketSolution::RunSolution()
{
    for (int i =0; i < numThreads; i ++)
    {
        ticketTracker[i] = startNum;
        flagTracker[i] = false;
    }

    primeSum = 0;

    vector<thread> threads;
    vector<TicketThread*> ticketJobs;
    for (int i =0; i < numThreads; i++)
    {
        TicketThread *t = new TicketThread (numThreads, i, maxNum, &primeSum, &primeNumbers, ticketTracker, flagTracker);
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

    flagTracker[threadID] = true;
    ticketTracker[threadID] = *max_element(ticketTracker, ticketTracker+8) + 1;
    flagTracker[threadID] = false;

    return ticketTracker[threadID];
}

bool TicketThread::CheckLine(int ticket)
{
    for(int other = 0; other < numThreads ; other++)
    {
        if (other != threadID)
        {
            while (flagTracker[other])
            {

            }

            if(ticketTracker[other] != 0 && ticketTracker[other] == ticket && other < threadID)
            {
                LeaveLine();
                return false;
            }

            //lexicographical ordering
            while( ticketTracker[other] != 0 && ticketTracker[other] < ticket)
            {
                //Just waiting, nothing else to do here
            }
        }
    }

    return true;
}

void TicketThread::ProcessPrime(int ticket)
{
    int i =0;
    for(i = 0; i < primeNumbers->size(); i++)
    {
        if(primeNumbers->at(i) == ticket)
        {
            break;
        }
    }

    if(i==0 || i != primeNumbers->size())
    {
        *primeSum += long(ticket);
        primeNumbers->push_back(ticket);
    }

    /*stringstream str;
    str<<"\nThread #"<< threadID <<" found prime # "<< ticket<<"\n"<<flush;
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
    ticketTracker[threadID] = 0;
}