---
name: 'Task 2: Implement `findPrimes()` Function'
about: Task 2 for Students
title: 'Task 2: Implement `findPrimes()` Function'
labels: enhancement
assignees: ''

---

**Description**
The second task is to implement the `findPrimes()` method.  This method will be given a begin and end range of integer values (`1 <= value <= max_int`).  The method should test all numbers in this range to determine which numbers are prime numbers.  This method should return the number of primes it finds in the asked for range.  

In addition to the begin and end of the range to search, the method should also support a third parameter which is a boolean flag.  When this flag is true, the method should display progress/results on standard output of its search for prime numbers in the given range.

**Suggested Solution**
This method should reuse the `isPrime()` method when testing each number to see if is a prime or not.

The method is a simple loop, then, that searches all values in the given range, counting and returning the number of primes that it finds.

**Additional Requirements**

- You are required to reuse your implementation of the `isPrime()` function when implementing the `findPrimes()` method.
- The indicated range should be inclusive of both the begin and end values of the range.  E.g. if asked to start at 2 and stop at 5, both 2 and 5 are prime numbers, thus you would return a count of 3, because 2, 3 and 5 are the 3 primes that are in the inclusive range from 2 to 5.
