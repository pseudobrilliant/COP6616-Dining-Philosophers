/*******************************************************************
* Author: cblythe
* Date: 8/25/2018
* Description:
*******************************************************************/

#ifndef COP6616_ASSIGNMENT1_SEQUENTIALSOLUTION_H
#define COP6616_ASSIGNMENT1_SEQUENTIALSOLUTION_H

#include "Solution.h"

class SequentialSolution: public Solution
{
public:
    SequentialSolution(string name, int numMax) : Solution(name, numMax) {}

    void RunSolution();
};


#endif //COP6616_ASSIGNMENT1_SEQUENTIALSOLUTION_H
