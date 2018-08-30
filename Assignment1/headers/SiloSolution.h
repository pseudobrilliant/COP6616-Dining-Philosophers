//
// Created by cblyt on 8/30/2018.
//

#ifndef COP6616_ASSIGNMENT1_SILOSOLUTION_H
#define COP6616_ASSIGNMENT1_SILOSOLUTION_H

#include "Solution.h"
#include <atomic>
#include <thread>

using namespace std;

class SiloSolution: public Solution {

public:
    SiloSolution(int t, string name, const int maxNum):Solution(name, maxNum)
    {
        numThreads = t;
    };

    void RunSolution() override;

    ~SiloSolution()
    {

    }

private:
    int numThreads = 0;
};


class SiloThread
{
public:
    SiloThread(int totalThreads, int thread, int start, int target)
    {
        numThreads = totalThreads;
        threadID = thread;
        startNum = start;
        maxNumber = target;
    };

    void RunThread();

    long GetSum();

    vector<int>* GetPrimes();

private:
    int threadID = 0;
    int startNum=0;
    int numThreads = 0;
    int maxNumber = 0;
    int privateCounter = 0;
    int privateDelta = 0;
    vector<int> privatePrimes;
    long privateSum = 0;
};

#endif //COP6616_ASSIGNMENT1_SILOSOLUTION_H
