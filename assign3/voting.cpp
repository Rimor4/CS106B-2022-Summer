// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include <cmath>
#include "map.h"
#include "voting.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function calculates the power index (Banzhaf power index) for each block in the given vector of blocks.
 * The power index measures the power of each voter(block) in deciding the outcome of voting.
 * It returns a vector of integers representing the power index for each block.
 */
Vector<int> computePowerIndexes(Vector<int>& blocks) {
    int total = 0;
    Vector<int> result;
    Map<int, int> memo;

    // Calculate the total number of votes for all blocks
    for (int block : blocks) {
        total += block;
    }

    // Calculate the power index for each block
    for (int i = 0; i < blocks.size(); i++) {
        int block = blocks.get(i);
        // Create a new vector excluding the current block
        Vector restBlocks = blocks.subList(0, i) + blocks.subList(i + 1, blocks.size() - i - 1);
        Vector<int> subset;

        // Check if the power index for the current blok has already been computed
        if (memo.containsKey(block)) {
            result.add(memo.get(block));
        } else {
            // Compute the power index for the current block
            int powerIndex = calculateCriticalVotes(restBlocks, subset, block, total / 2, 0, 0);
            result.add(powerIndex);
            memo[block] = powerIndex;
        }
    }

    result = convertToPercentages(result);

    return result;
}

/*
 * Recursively calculate the power index of a given target block.
 * It generates all possible subsets of the remaining blocks and checks if the target block is a critical voter.
 * A block is a critical voter if its addition to the subset causes the subset's total votes to exceed the boundary
 */
int calculateCriticalVotes(Vector<int>& rest, Vector<int>& subset, int target, int boundary, int index, int currentSum) {
    int result = 0;

    // Check if the subset is non-empty and if the current block is a critical voter
    if (!subset.isEmpty()) {
        if (currentSum > boundary) {
            // Return if the subset already exceeds the boundary
            return 0;
        } else {
            // Check if adding the target block makes the subset exceed the boundary
            if (currentSum <= boundary && (currentSum + target) > boundary) {
                result++;
            }
        }
    }

    // Recursively generate all possible subsets
    for (int i = index; i < rest.size(); i++) {
        // Choose the current element and add it to the subset
        subset.add(rest.get(i));
        // Recur with the updated subset and remaining elements
        result += calculateCriticalVotes(rest, subset, target, boundary, i + 1, currentSum + rest.get(i));
        // Backtrack
        subset.remove(subset.size() - 1);
    }

    return result;
}

// Convert a vector of integer value to their corresponding percentages of the total sum,
// rounded down to the nearest integer
Vector<int> convertToPercentages(const Vector<int>& values) {
    Vector<int> percentages;
    double sum = 0;

    // Calculate the sum of values in the vector
    for (int value : values) {
        sum += value;
    }

    if (sum == 0) {
        percentages = Vector<int> (values.size(), 0);
    } else {
        // Calculate the percentage for each value
        for (int value : values) {
            int percentage = static_cast<int>((value / sum) * 100);
            percentages.add(percentage);
        }
    }

    return percentages;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Time power index operation") {
    for (int size = 2; size <= 32; size *= 2) {
        Vector<int> blocks;
        for (int i = 0; i < size; i++) {
            blocks.add(randomInteger(1, 10));
        }
        TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
    }
}


