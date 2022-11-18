---
name: 'Task 1: Implement `isPrime()` Function'
about: Task 1 for Students
title: 'Task 1: Implement `isPrime()` Function'
labels: enhancement, good first issue
assignees: ''

---

**Description**

The first task is to create a working version of the `isPrime()` method.  This method should take a positive integer value (`1 <= value <= max_int`), and return a boolean result.  The result should be true if the value is a prime number.  A prime number is a number that is only divisible by 1 and itself.  Thus 2, 3, 5, 7, etc. are all examples of prime because they only divide by 1 and themselves.  But 4 is not prime (and should return false) because it is evenly divisible by 2.

**Suggested Solution**
A simple brute force approach is to test all possible divisors from `divisor = 2` up to `divisor = value - 1`.  If any divisor is found that evenly divides the number, then you can immediately return an answer of `False`, the number cannot be prime.  But if you check all possible divisors and don't find any, then the answer is `True`, the number is prime.

**Additional Requirements**

There are no additional requirements on this tasks implementation.
