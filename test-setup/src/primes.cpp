/** @file primes.cpp
 *
 * @brief Implementation file for Assignment Primes: Practice Example
 *   and Class Workflow
 *
 * @author Jane Programmer
 * @note   cwid : 123 45 678
 * @note   class: COSC 2336, Summer 2021
 * @note   ide  : VS Code Server 3.9.3
 * @note   assg : Assignment Example
 * @date   June 1, 2021
 *
 * Assignment Primes: example Workflow for Assignments and getting
 * Git and GitHub and VSCode setup.  This is the file where you should
 * implement the functions that are dscribed in this practice
 * exercise.
 */
#include <cmath>
#include <iostream>
using namespace std;

/** @brief Is value prime
 *
 * Determine if a given (positive) integer value >= 1 is prime.
 * Prime numbers are numbers that are divisible only by 1 and
 * themselves.  Thus in this implementation, we use a brute force
 * method and test to see if any number from 2 up to the value-1 is
 * a divisor of the number.  If we find such a divisor, then the
 * number is not prime.  If we fail to find such a divisor, then the
 * number is prime.
 *
 * @param value The value to be tested to see if prime or not.
 *
 * @returns bool Returns true if the value is prime, false if it
 *   is not.
 */
bool isPrime(int value)
{
  // check all possible divisors of the value from 2 up to value-1 to
  // see if we can find a valid divisor
  for (int divisor = 2; divisor < value - 1; divisor++)
  {
    // if divisor divides the value, remainder is 0, and thus we found
    // a divisor
    if (value % divisor == 0)
    {
      // if there is a divisor other than 1 and value, then the answer is
      // false, it is not prime
      return false;
    }
  }

  // but if we check all divisors and don't find one, then the answer
  // is true, it is a prime
  return true;
}

/** @brief Find primes in range
 *
 * Find primes in a range of values from start to end (inclusive).
 * This function returns a count of the number of primes found within
 * the range.  As a side effect, the primes that are found in the range
 * are displayed on standard output.
 *
 * @param start The start of the range to search.  This value is inclusive,
 *   we test the start value of the range to see if it is a prime or not.
 * @param end The end of the range to search.  This value is inclusive,
 *   we test the end value of the range to see if it is a prime or not.
 * @param displayOnCout A parameter controlling whether or not primes found
 *   in the range are displayed on the standard cout stream or not.  This
 *   parameter defaults to true, found primes will be displayed when found.
 *
 * @returns int Returns the count of the number of primes we find in the
 *   asked for range.
 */
// write your implementation of the findPrimes() function here

int findPrimes(int start, int end, bool displayOnCout = true)
{
  int primeCount = 0; // count primes we see in range to return

  if (displayOnCout)
  {
    cout << "List of primes in range " << start << " to " << end << ": ";
  }

  // search the range of values from start to end
  for (int value = start; value <= end; value++)
  {
    // whenever we find a prime list it out to standard output
    if (isPrime(value))
    {
      if (displayOnCout)
      {
        cout << value << ", ";
      }
      primeCount++;
    }
  }

  if (displayOnCout)
  {
    cout << endl;
    cout << "Count of primes: " << primeCount << endl;
  }

  // return the count of the number of primes we found in the range
  return primeCount;
}