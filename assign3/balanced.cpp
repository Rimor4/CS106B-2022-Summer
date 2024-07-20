/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include <regex>
#include <stack>
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/*
 * Extract and retain brackets('[' ']' '(' ')' '{' '}') in <str>
 */
string operatorsFrom(string str) {
    string result;

    // Use regular expression to match brackets
    regex re("[^\\[\\]\\(\\)\\{\\}]");
    result = regex_replace(str, re, "");

    return result;
}

/*
 * Verify that the extracted brackets are balanced
 */
bool operatorsAreMatched(string ops) {
    // // Iterative approach(Use stack)
    // stack<char> s;
    // for (char c : ops) {
    //     if (c == '[' || c == '(' || c == '{') {
    //         s.push(c);
    //     } else {
    //         if (s.empty()) {
    //             return false;
    //         }
    //         char top = s.top();
    //         s.pop();
    //         if (!parenthesesMatched(top, c)) {
    //             return false;
    //         }
    //     }
    // }
    // return s.empty();

    // Recursive approach
    if (ops.empty()) {
        return true;
    }

    // check the string for parentheses
    size_t pos;
    if ((pos = ops.find("()")) != string::npos) {
        return operatorsAreMatched(ops.substr(0, pos) + ops.substr(pos + 2));
    }
    if ((pos = ops.find("[]")) != string::npos) {
        return operatorsAreMatched(ops.substr(0, pos) + ops.substr(pos + 2));
    }
    if ((pos = ops.find("{}")) != string::npos) {
        return operatorsAreMatched(ops.substr(0, pos) + ops.substr(pos + 2));
    }

    return false;
}

/*
 * Verify a pair of parentheses match
 */
bool parenthesesMatched(char left, char right) {
    switch (left) {
    case '[':
        return right == ']';
    case '(':
        return right == ')';
    case '{':
        return right == '}';
    default:
        return false;
    }
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}
