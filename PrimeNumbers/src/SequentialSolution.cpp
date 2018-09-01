
#include "../headers/SequentialSolution.h"
#include <iostream>

using namespace std;

void SequentialSolution::RunSolution()
{
    for (int i = startNum; i <= maxNum; i+=2)
    {
        if (IsPrime(i))
        {
            primeNumbers.push_back(i);
            primeSum += long(i);
        }
    }
}