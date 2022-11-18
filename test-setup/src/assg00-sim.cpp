/** @file assg00-sim.cpp
 *
 * @brief main/debug executable for Assignment Primes: Practice Example
 *   and Class Workflow
 *
 * @author Jane Programmer
 * @note   class: COSC 2336, Summer 2021
 * @note   ide  : VS Code Server 3.9.3
 * @note   assg : Assignment Example
 * @date   June 1, 2021
 *
 * Assignment Example, example Workflow for Assignments and getting Git and
 * GitHub and VSCode setup.  This file contains a separate main()
 * function and is meant to be compiled to a separate executable to be
 * used for debugging or testing.
 */
#include "primes.hpp"
#include <cmath>
#include <iostream>
using namespace std;

/**
 * @brief usage
 *
 * Usage information for invoking simulator with command line
 * arguments.  Print usage information and exit with non success
 * status to indicate error.
 */
void usage()
{
  cout << "Usage: ./sim begin end" << endl
       << "Test for primes in the indicated range" << endl
       << endl
       << "begin      The begin unsigned integer inclusive to begin search for primes" << endl
       << "end        The end unsigned integer inclusive of end range to search" << endl
       << "             for primes" << endl;
  exit(1);
}

/** @brief Main entry point
 *
 * Main entry point for debugging functions.
 *
 * @param argc The command line argument count, the number of arguments
 *   provided by user on the command line.
 * @param argv An array of char* old style c-strings.  Each argv[x]
 *   that is passed in holds one of the command line arguments provided
 *   by the user to the program when started.
 *
 * @returns int Returns 0 to indicate successfull completion of program,
 *   and a non-zero value to indicate an error code.
 */
int main(int argc, char** argv)
{
  // parse command line arguments
  // if we do not get required command line arguments, print usage
  // and exit immediately.
  if (argc != 3)
  {
    usage();
  }
  int beginRange = atoi(argv[1]);
  int endRange = atoi(argv[2]);

  int numPrimes = 0;
  // need to uncomment the following when you have finished implementing the functions
  // in order to run the simulations
  // numPrimes = findPrimes(beginRange, endRange, true);
  cout << "Number of primes found between " << beginRange << " and " << endRange << " (inclusive): " << numPrimes << endl;

  // return 0 to indicate successful completion
  return 0;
}
