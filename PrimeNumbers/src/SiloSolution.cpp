/*******************************************************************
* Author: cblythe
* Date: 8/25/2018
* Description: /Concurrent solution that breaks up the problem into discrete sections (silos) that can work in parallel.
* The given range of prime numbers is broken up into blocks by the number of threads given (8).
* Each thread is in charge of their initial odd numbers going forward by a delta of 8 * 2 every block.
* So in this base case thread 0 will start with the initial number 3 then 19, 35, etc while thread 1 starts with 5,
* 21, 37, etc. Each thread continues incrementing their number by the delta and storing whether it's prime (# or 0)
* till they pass the max. Once all the threads are completed their numbers are guaranteed to be in order amongst the
* threads so the main thread can fold them all in together. Creating a complete list of all the prime numbers in the range.
*******************************************************************/

#include "../headers/SiloSolution.h"

void SiloSolution::RunSolution()
{
    primeSum = 0;
    primeNumbers.clear();

    //In order to efficiently use pointer arrays instead of dynamic vectors we need a max size for each silo.
    //This number is the total number of odds / number of threads and any possible remainder.
    int maxSize = ((maxNum / 2) /numThreads) + ((maxNum / 2) % numThreads) + (maxNum % 2);

    vector<thread> threads;
    vector<SiloThread*> siloJobs;

    //Create and store the threads
    for (int i =0; i < numThreads; i++)
    {
        SiloThread *t = new SiloThread (numThreads, i, startNum, maxNum, maxSize);
        threads.push_back(thread(&SiloThread::RunThread, t));
        siloJobs.push_back(t);
    }

    //Wait till all the threads are done and then gather the resulting data.
    vector<int*> siloPrimes;
    for (int i =0; i< numThreads; i++)
    {
        threads[i].join();
        primeSum += siloJobs[i]->GetSum();
        int* vec = siloJobs[i]->GetPrimes();
        siloPrimes.push_back(vec);
    }

    //run through all the possible results
    for (int i= 0; i < maxSize * numThreads; i++)
    {
        //get the silo (thread index) from the current index.
        int silo = i % numThreads;
        //get the block index from the current index.
        int place = i / numThreads;

        //We need the non ptime 0's to go through all the numbers in order. Otherwise we'd be unable to fold the arrays.
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
    //keeps count of the current silo number
    privateCounter = threadID * 2 + startNum;
    privateDelta = numThreads * 2;
    privateSum = 0;
    privateIndex = 0;

    while(privateCounter <= maxNumber)
    {
        bool isPrime = IsPrime(privateCounter);

        //increment the sum and store the actual number if prime
        if(isPrime)
        {
            privatePrimes[privateIndex] = privateCounter;
            privateSum += long(privateCounter);
        }
        else
        {
            //keep the order of numbers between all the silos by storing the false primes as well
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