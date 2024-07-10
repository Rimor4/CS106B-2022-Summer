/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include "error.h"
#include "testing/SimpleTest.h"
#include <chrono>
#include <cmath>
#include <iostream>
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
  long total = 0;
  for (long divisor = 1; divisor < n; divisor++) {
    if (n % divisor == 0) {
      total += divisor;
    }
  }
  return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) { return (n != 0) && (n == divisorSum(n)); }

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfects(long stop) {
  for (long num = 1; num < stop; num++) {
    if (isPerfect(num)) {
      cout << "Found perfect number: " << num << endl;
    }
    if (num % 10000 == 0)
      cout << "." << flush; // progress bar
  }
  cout << endl << "Done searching up to " << stop << endl;
}

/*
 * A smarter implement of divisorSum():
 * skip the case where n is 0 and 1;
 * if k is the factor of n, then (n / k) is also a factor,
 * hence we can reduce the number of loops to sqrt(n) times.
 */
long smarterSum(long n) {
  // 0 has no factors
  if (n <= 1)
    return 0;

  // 1 is a factor of any number
  long total = 1;
  double sqrtN = sqrt(n);
  for (long divisor = 2; divisor <= sqrtN; divisor++) {
    if (n % divisor == 0) {
      if (divisor == sqrtN) {
        // divisor is ths square root of n
        total += sqrtN;
      } else {
        total += divisor;
        total += n / divisor;
      }
    }
  }
  return total;
}

/*
 * A smarter implement of isPerfect():
 * skip the case where n is 1;
 * invoke a smarter function: smarterSum().
 */
bool isPerfectSmarter(long n) { return (n > 1) && (n == smarterSum(n)); }

/*
 * A smarter implement of findPerfects()
 * invoke a smarter function: isPerfecSmarter().
 */
void findPerfectsSmarter(long stop) {
  for (long num = 1; num < stop; num++) {
    if (isPerfectSmarter(num)) {
      cout << "Found perfect number: " << num << endl;
    }
    if (num % 10000 == 0)
      cout << "." << flush; // progress bar
  }
  cout << endl << "Done searching up to " << stop << endl;
}

/*
 * Find the NTH perfect Euclidean number.
 * iterate from 1 to positive infinity, until we find it
 */
long findNthPerfectEuclid(long n) {
  if (n <= 0) {
    error("n must be positive");
  }

  long num;
  int Nth = 0;
  for (num = 1;; num++) {
    if (isPerfectSmarter(num)) {
      Nth++;
      if (Nth == n)
        return num;
    }
  }
}

/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
  EXPECT_EQUAL(divisorSum(1), 0);
  EXPECT_EQUAL(divisorSum(6), 6);
  EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
  EXPECT(isPerfect(6));
  EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
  EXPECT(!isPerfect(12));
  EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
  EXPECT(!isPerfect(0));
  EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") { EXPECT(isPerfect(33550336)); }

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes") {
  TIME_OPERATION(10000, findPerfects(10000));
  TIME_OPERATION(20000, findPerfects(20000));
  TIME_OPERATION(40000, findPerfects(40000));
}

// Add your student test cases here
STUDENT_TEST("To solve Q2") {
    TIME_OPERATION(6, findPerfects(6));
    TIME_OPERATION(28, findPerfects(28));
    TIME_OPERATION(496, findPerfects(496));
    TIME_OPERATION(8128, findPerfects(8128));
}

STUDENT_TEST("To solve Q3") {
  TIME_OPERATION(10, isPerfect(10));
  TIME_OPERATION(1000, isPerfect(1000));
  TIME_OPERATION(1000, findPerfects(1000));
  TIME_OPERATION(2000, findPerfects(2000));
}

STUDENT_TEST("Test smartSum correctness") {
    EXPECT_EQUAL(smarterSum(1), 0);
    EXPECT_EQUAL(smarterSum(6), 6);
    EXPECT_EQUAL(smarterSum(12), 16);
    EXPECT_EQUAL(divisorSum(256), smarterSum(256));
    EXPECT_EQUAL(divisorSum(12566), smarterSum(12566));
    EXPECT_EQUAL(divisorSum(364824), smarterSum(364824));
}

STUDENT_TEST("Test findNthPerfectEuclid correctness") {
  EXPECT_ERROR(findNthPerfectEuclid(0));
  EXPECT_EQUAL(6, findNthPerfectEuclid(1));
  EXPECT_EQUAL(28, findNthPerfectEuclid(2));
  EXPECT_EQUAL(496, findNthPerfectEuclid(3));
  EXPECT_EQUAL(8128, findNthPerfectEuclid(4));
}

STUDENT_TEST("To solve Q7") {
  TIME_OPERATION(10000, findPerfectsSmarter(10000));
  TIME_OPERATION(20000, findPerfectsSmarter(20000));
  TIME_OPERATION(40000, findPerfectsSmarter(40000));
}
