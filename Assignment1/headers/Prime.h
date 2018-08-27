/*****************************************************************************
 * Author: cblythe
 * Date: 8/25/2018
 * Description: Helper methods for quickly checking if a given number is prime.
 * This method used the Probablistic Miller-Rabin Method to first roughly define
 * whether the number is prime or not.  If the value shows as prime then
 * deterministic methods are then used to return an accurate answer.
 *
 * This implementation is heavily inspired by:
 * https://www.geeksforgeeks.org/primality-test-set-3-miller-rabin/
 *
*****************************************************************************/

#ifndef COP6616_ASSIGNMENT1_PRIME_H
#define COP6616_ASSIGNMENT1_PRIME_H
#endif //COP6616_ASSIGNMENT1_PRIME_H

#include <iostream>
#include <gmp.h>

//Number of times the test is applied with different base values.
//Chance of error is 4 ^ -k = 1 / 256
#define ACCURACY 9

bool IsPrime(int num);
bool MillerRabinTest(int num, int s, int k);

bool IsPrime(int num)
{

    MP_INT n;
    mpz_init(&n);
    mpz_set_si(&n, num);

    int result =   mpz_probab_prime_p (&n, ACCURACY);
    if (result == 2)
    {
        return true;
    }
    else if (result == 1)
    {
        //std::cout << "Possible Incorrect Number: " << num <<"\n";
        return true;
    }
    else
    {
        return false;
    }
}

