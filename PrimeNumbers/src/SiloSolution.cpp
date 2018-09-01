//
// Created by cblyt on 8/30/2018.
//

#include "../headers/SiloSolution.h"

void SiloSolution::RunSolution()
{
    primeSum = 0;
    primeNumbers.clear();
    int maxSize = ((maxNum / 2) /numThreads) + ((maxNum / 2) % numThreads) + (maxNum % 2);

    vector<thread> threads;
    vector<SiloThread*> siloJobs;

    for (int i =0; i < numThreads; i++)
    {
        SiloThread *t = new SiloThread (numThreads, i, startNum, maxNum, maxSize);
        threads.push_back(thread(&SiloThread::RunThread, t));
        siloJobs.push_back(t);
    }

    vector<int*> siloPrimes;
    for (int i =0; i< numThreads; i++)
    {
        threads[i].join();
        primeSum += siloJobs[i]->GetSum();
        int* vec = siloJobs[i]->GetPrimes();
        siloPrimes.push_back(vec);
    }

    for (int i= 0; i < maxSize * numThreads; i++)
    {
        int silo = i % numThreads;
        int place = i / numThreads;
        int cur = siloPrimes[silo][place];
        if (cur != 0)
        {
             primeNumbers.push_back(cur);
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
    privateIndex = 0;

    while(privateCounter <= maxNumber)
    {
        bool isPrime = IsPrime(privateCounter);

        if(isPrime)
        {
            privatePrimes[privateIndex] = privateCounter;
            privateSum += long(privateCounter);
        }
        else
        {
            privatePrimes[privateIndex] = 0;
        }

        privateIndex += 1;
        privateCounter += privateDelta;
    }
}

long SiloThread::GetSum()
{
    return privateSum;
}

int* SiloThread::GetPrimes()
{
    return privatePrimes;
}