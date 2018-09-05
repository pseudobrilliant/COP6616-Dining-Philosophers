
#include "../headers/SequentialSolution.h"
#include <iostream>

using namespace std;

void SequentialSolution::RunSolution()
{
    //increment by 2 since we only need to check odd numbers
    for (int i = startNum; i <= maxNum; i+=2)
    {
        if (IsPrime(i))
        {
            primeNumbers.push_back(i);
            primeSum += long(i);
        }
    }
}