/*******************************************************************
* Author: cblythe
* Date: 8/25/2018
* Description: Prime checking program that identifies and stores all primes
* from 3 to 10^8. A sequential and concurrent solution are provided
*******************************************************************/

#include <iostream>
#include <cmath>
#include <time.h>
#include "../headers/SequentialSolution.h"
#include "../headers/SiloSolution.h"

int main()
{
    const int numThreads = 8;
    const int numMaxOrder = 8;
    const int numMax = int(pow(10,numMaxOrder));

    //Sequential solution for initial planning as well as comparison to
    SequentialSolution seq("Sequential", numMax);
    seq.Run();
    seq.Report();

    //Concurrent solution that breaks up the problem into discrete sections (silos) that can work in parallel.
    //The given range of prime numbers is broken up into blocks by the number of threads given (8).
    //Each thread is in charge of their initial odd numbers going forward by a delta of 8 * 2 every block.
    //So in this base case thread 0 will start with the initial number 3 then 19, 35, etc while thread 1 starts with 5,
    //21, 37, etc. Each thread continues incrementing their number by the delta and storing whether it's prime (# or 0)
    //till they pass the max. Once all the threads are completed their numbers are guaranteed to be in order amongst the
    //threads so the main thread can fold them all in together. Creating a complete list of all the prime numbers in the range.
    SiloSolution silo(numThreads, "Silo", numMax);
    silo.Run();
    silo.Report();

    return 0;
}

