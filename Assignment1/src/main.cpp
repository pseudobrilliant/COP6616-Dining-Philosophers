/*******************************************************************
* Author: cblythe
* Date: 8/25/2018
* Description:
*******************************************************************/

#include <iostream>
#include <cmath>
#include "../headers/SequentialSolution.h"
#include "../headers/SiloSolution.h"

int main()
{
    const int numThreads = 8;
    const int numMaxOrder = 8;
    const int numMax = int(pow(10,numMaxOrder));

    SequentialSolution seq("Sequential", numMax);
    seq.Run();
    seq.Report();

    SiloSolution silo(numThreads, "Silo", numMax);
    silo.Run();
    silo.Report();

    return 0;
}

