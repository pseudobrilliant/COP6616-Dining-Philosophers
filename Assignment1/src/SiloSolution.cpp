//
// Created by cblyt on 8/30/2018.
//

#include "../headers/SiloSolution.h"

void SiloSolution::RunSolution()
{
    primeSum = 0;
    primeNumbers.clear();

    vector<thread> threads;
    vector<SiloThread*> siloJobs;
    for (int i =0; i < numThreads; i++)
    {
        SiloThread *t = new SiloThread (numThreads, i, startNum, maxNumber);
        threads.push_back(thread(&SiloThread::RunThread, t));
        siloJobs.push_back(t);
    }

    for (auto& t: threads)
    {
        t.join();
    }

    vector<vector<int>*> siloPrimes;
    int siloTotals= 0;
    for (int i =0; i< numThreads; i ++)
    {
        primeSum += siloJobs[i]->GetSum();
        vector<int> * vec = siloJobs[i]->GetPrimes();
        siloTotals += vec->size();
        siloPrimes.push_back(vec);
    }

    for (int i= 0; i < siloTotals; i++)
    {
        int silo = i % numThreads;
        int place = i / numThreads;
        if(place < siloPrimes[silo]->size())
        {
            int cur = siloPrimes[silo]->at(place);
            if (cur != 0)
            {
                primeNumbers.push_back(cur);
            }
        }
    }

    for (auto& j : siloJobs)
    {
        delete j;
    }
}

void SiloThread::RunThread()
{
    privateCounter = threadID * 2 + startNum;
    privateDelta = numThreads * 2;
    privateSum = 0;

    while(privateCounter <= maxNumber)
    {
        bool isPrime = IsPrime(privateCounter);

        if(isPrime)
        {
            privatePrimes.push_back(privateCounter);
            privateSum += long(privateCounter);
        }
        else
        {
            privatePrimes.push_back(0);
        }

        privateCounter += privateDelta;
    }
}

long SiloThread::GetSum()
{
    return privateSum;
}

vector<int>* SiloThread::GetPrimes()
{
    return &privatePrimes;
}