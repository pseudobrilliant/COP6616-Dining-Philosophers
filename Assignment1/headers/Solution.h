/*******************************************************************
* Author: cblythe
* Date: 8/25/2018
* Description:
*******************************************************************/
#ifndef COP6616_ASSIGNMENT1_SOLUTION_H
#define COP6616_ASSIGNMENT1_SOLUTION_H

#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <vector>

#include "Prime.h"

using namespace std;
using namespace std::chrono;

class Solution
{
public:
    Solution(string name, const int maxNum);

    void Run();

    void Report();

    ~Solution(){};

protected:
    virtual void RunSolution() = 0;
    vector<long> primeNumbers;
    long primeSum = 0;
    int maxNumber = 0;
    const int startNum = 2;

private:
    // 0 and 1 are not prime or composite so let's not include them.
    string solutionName;
    high_resolution_clock::time_point startTime;
    high_resolution_clock::time_point endTime;

};

#endif //COP6616_ASSIGNMENT1_SOLUTION_H
