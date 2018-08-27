/*******************************************************************
* Author: cblythe
* Date: 8/25/2018
* Description: 
*******************************************************************/
#include "../headers/Solution.h"

Solution::Solution(string name, const int maxNum)
{
solutionName = name;
maxNumber = maxNum;
};

void  Solution::Run()
{
    primeSum = 0;
    primeNumbers.clear();

    cout << "Solution: " << solutionName << "\n";
    cout << "Running..."<<flush;
    startTime = high_resolution_clock::now();
    RunSolution();
    endTime = high_resolution_clock::now();
    cout << "Completed!\n";
}

void Solution::Report()
{
    long microDuration = duration_cast<microseconds>( endTime - startTime ).count();
    float secondsDuration = float(microDuration) / float(1000000.0f);
    long size = primeNumbers.size();

    cout << "Execution_Time: " << secondsDuration << "(seconds)\n";
    cout << "# of Primes Found: " << size << "\n";
    cout << "Prime Total: " << primeSum << "\n";
    cout << "Top 10 Prime Numbers: ";

    for (int i = 1; i <= 10 && i < size; i ++)
    {
        long num = primeNumbers[size - i];
        cout << num << " ";
    }
    cout << "\n";

};