/*****************************************************************************
 * Author: cblythe
 * Date: 8/25/2018
 * Description: Helper methods for quickly checking if a given number is prime.
 * This method used the Probablistic Miller-Rabin Method to first roughly define
 * whether the number is prime or not. Depending on the accuracy number provided
 * this algorithm will generate multiple runs of random numbers to build likelihood
 * of number actually being prime.
 * Since this is not integral to our study of conccurency this implementation is heavily inspired by:
 * https://www.geeksforgeeks.org/primality-test-set-3-milonger-rabin/
 * https://www.sanfoundry.com/cpp-program-implement-miller-rabin-primality-test/
*****************************************************************************/

#ifndef COP6616_ASSIGNMENT1_PRIME_H
#define COP6616_ASSIGNMENT1_PRIME_H

#include <iostream>
#include <stdlib.h>

//Number of times the test is applied with different base values.
//Chance of error is 4 ^ -k
#define ACCURACY 9

inline long MSquare(long base, long exponent, long mod)
{

    long x = 1;

    long y = base;

    while (exponent > 0)
    {
        if (exponent % 2 == 1)
        {

            x = (x * y) % mod;
        }

        y = (y * y) % mod;

        exponent = exponent / 2;

    }

    return x % mod;

}

inline long MMultiplication(long a, long b, long mod)
{
    long x = 0;
    long y = a % mod;

    while (b > 0)
    {
        if (b % 2 == 1)
        {
            x = (x + y) % mod;
        }

        y = (y * 2) % mod;

        b /= 2;
    }

    return x % mod;
}

inline bool IsPrime(int num)
{
    long n = long(num);

    if (n < 2 || (n % 2 == 0))
    {
        return false;
    }

    if (n == 3 || n == 5 || n == 7 || n == 9)
    {
        return true;
    }

    long d = n - 1;

    while (d % 2 == 0)
    {
        d /= 2;
    }

    for (int i = 0; i < ACCURACY; i++)
    {
        long randNumber = random() % (n - 1) + 1;

        long temp = d;

        long r = MSquare(randNumber, temp, n);

        while (temp != n - 1 && r != 1 && r != n - 1)
        {
            r = MMultiplication(r, r, n);
            temp *= 2;
        }

        if (r != n - 1 && temp % 2 == 0)
        {
            return false;
        }
    }

    return true;
}

#endif //COP6616_ASSIGNMENT1_PRIME_H
