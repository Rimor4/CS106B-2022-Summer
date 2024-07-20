/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <climits>
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * Merge two individual sorted sequences into one larger sorted sequence
 *
 * @param a b Both are a queue<int> in increasing order from front to back
 */
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;
    int prev = INT_MIN;

    // Fetch and compare the elements in the two queues one by one
    while (!a.isEmpty() && !b.isEmpty()) {
        if (a.peek() <= b.peek()) {
            // enforce the merge precondition that <a> and <b> are both increasing
            if (prev > a.peek()) {
                error("The parameter <a> must be an increasing quene");
            } else {
                prev = a.dequeue();
                result.enqueue(prev);
            }
        } else {
            if (prev > b.peek()) {
                error("The parameter <b> must be an increasing quene");
            } else {
                prev = b.dequeue();
                result.enqueue(prev);
            }
        }
    }

    // Put the remaining elements from the longer quene into the result
    while (!a.isEmpty()) {
        if (prev > a.peek()) {
            error("The parameter <a> must be an increasing quene");
        } else {
            prev = a.dequeue();
            result.enqueue(prev);
        }
    }

    while (!b.isEmpty()) {
        if (prev > b.peek()) {
            error("The parameter <b> must be an increasing quene");
        } else {
            prev = b.dequeue();
            result.enqueue(prev);
        }
    }

    return result;
}

/*
 * A multiway merge receives k sorted sequences to merge into one sorted output.
 * This is a naive approach.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

/*
 * A multiway merge receives k sorted sequences to merge into one sorted output.
 * This function use a recursive approach to divide and conquer the sequences.
 *
 * @param all A Vector of Queue<int> where each Queue<int> is sorted in increasing order.
 * @return A Queue<int> that contains all the elements from the input queues, merged into a single sorted sequence.
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;
    int size = all.size();

    if (size == 1) {
        return all[0];
    } else if (size == 2) {
        return binaryMerge(all[0], all[1]);
    } else {
        Vector<Queue<int>> left = all.subList(0, size/2);
        Vector<Queue<int>> right = all.subList(size/2, size - size/2);

        return binaryMerge(recMultiMerge(left), recMultiMerge(right));
    }
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 100000;
    for (int power = 1; power <= 256; power *= 4) {
        Queue<int> a = createSequence(n * power);
        Queue<int> b = createSequence(n * power);
        TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
    }
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    // Measure n
    int n = 1100;
    int k = n/10;
    for (int power = 1; power <= 256; power *= 4) {
        Queue<int> input = createSequence(n * power);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(input.size(), naiveMultiMerge(all));
    }

    // Measure k
    n = 11000;
    k = n;
    for (int power = 1; power <= 256; power *= 4) {
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k / power);
        distribute(input, all);
        TIME_OPERATION(input.size(), naiveMultiMerge(all));
    }
}

PROVIDED_TEST("Time recMultiMerge operation") {
    // Measure n
    int n = 11000;
    int k = n/10;
    for (int power = 1; power <= 256; power *= 4) {
        Queue<int> input = createSequence(n * power);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(input.size(), recMultiMerge(all));
    }

    // Measure k
    n = 110000;
    k = n/10;
    for (int power = 1; power <= 256; power *= 4) {
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k / power);
        distribute(input, all);
        TIME_OPERATION(input.size(), recMultiMerge(all));
    }
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}
