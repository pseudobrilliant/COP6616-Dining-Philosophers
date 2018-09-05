/*******************************************************************
* Author: cblythe
* Date: 8/25/2018
* Description: Simple Solution base class that accurately reports results.
*******************************************************************/
#include "../headers/Solution.h"

Solution::Solution(string name, const int max)
{
    solutionName = name;
    maxNum = max;
};

void Solution::Run()
{
    primeSum = 0;
    primeNumbers.clear();

    cout << "Solution: " << solutionName << "\n";
    cout << "Running..." << flush;
    startTime = high_resolution_clock::now();
    RunSolution();
    endTime = high_resolution_clock::now();
    cout << "Completed!\n";
}

void Solution::Report()
{
    long microDuration = duration_cast<microseconds>(endTime - startTime).count();
    float secondsDuration = float(microDuration) / float(1000000.0f);
    long size = primeNumbers.size();

    cout << "Execution_Time: " << secondsDuration << "(seconds)\n";
    cout << "# of Primes Found: " << size << "\n";
    cout << "Prime Total: " << primeSum << "\n";
    cout << "Top 10 Prime Numbers: ";

    for (int i = 1; i <= 30 && i < size; i++)
    {
        long num = primeNumbers[size - i];
        cout << num << " ";
    }
    cout << "\n\n------------------------------------------------------------------------\n\n";
};


// Used to debug and compare different solution results.
void Solution::Compare(vector<int> otherNumbers)
{
    cout <<"Left Join: ";
    for (int i =0; i < otherNumbers.size(); i ++)
    {
        int j;
        for( j = 0; j < primeNumbers.size(); j++)
        {
            if(primeNumbers[j] == otherNumbers[i])
                break;
        }

        if(j ==  primeNumbers.size())
        {
            cout << otherNumbers[i] << " ";
        }
    }
    cout<<"\n";

    cout <<"Right Join: ";
    for (int i =0; i < primeNumbers.size(); i ++)
    {
        int j;
        for(j = 0; j < otherNumbers.size(); j++)
        {
            if(primeNumbers[i] == otherNumbers[j])
                break;
        }

        if(j ==  otherNumbers.size())
        {
            cout << otherNumbers[i] << " ";
        }
    }
    cout<<"\n";
}
