
#include "../headers/SequentialSolution.h"
#include "../headers/Prime.h"
#include <iostream>

using namespace std;

void SequentialSolution::RunSolution()
{
    for (int i =1; i <= maxNumber; i++)
    {
        if (IsPrime(i))
        {
            primeNumbers.push_back(i);
            primeSum += long(i);
        }
    }
}