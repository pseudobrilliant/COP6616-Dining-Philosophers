/*******************************************************************
* Author: cblythe
* Date: 8/25/2018
* Description:
*******************************************************************/

#include <iostream>
#include <cmath>
#include "../headers/SequentialSolution.h"
#include "../headers/TicketSolution.h"

int main()
{
    const int numThreads = 8;
    const int numMaxOrder = 8;
    const int numMax = int(pow(10,numMaxOrder));

    SequentialSolution seq("Sequential", numMax);
    seq.Run();
    seq.Report();

    TicketSolution ticket(numThreads, "Ticket", numMax);
    ticket.Run();
    ticket.Report();

    return 0;
}

