/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "map.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * Return the points of a word in Boggle according to its length.
 *
 * A word with 3 or fewer characters is invalid by the above criteria and is thus worth zero points.
 * A 4-letter word earns 1 point; a 5-letter word is 2 points; 6-letter words earn 3 points; and so on.
 */
int points(string str) {
    if (str.size() < 4) return 0;
    return str.size() - 3;
}

// Helper function to perform recursive backtracking search
void findWords(Grid<char>& board, Lexicon& lex, Set<string>& foundWords,
               Grid<bool>& visited, int row, int col, string& soFar) {
    // Base case: if out of bounds or already visited
    if (!board.inBounds(row, col) || visited[row][col])
        return;

    // Check if soFar(include the current letter) is valid as a prefix
    if (!lex.containsPrefix(soFar + board[row][col]))
        return;

    // Choose!
    // Append current letter to the forming word,
    // and mark the current cell as visited
    soFar += board[row][col];
    visited[row][col] = true;


    // If current word is valid, add it to the set of found words
    if (soFar.length() >= 4 && lex.contains(soFar)) {
        foundWords.add(soFar);
    }

    // Explore all 8 possible directions
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr != 0 || dc != 0) {
                findWords(board, lex, foundWords, visited, row + dr, col + dc, soFar);
            }
        }
    }

    // Unchoose!
    // Erase the last letter in the forming word, and
    // unmark the current cell as visited for other paths
    soFar.pop_back();
    visited[row][col] = false;
}

// Main function to score the Boggle board
int scoreBoard(Grid<char>& board, Lexicon& lex) {
    Set<string> foundWords;
    Grid<bool> visited(board.numRows(), board.numCols(), false);

    // Start backtracking from each cell
    for (int row = 0; row < board.numRows(); row++) {
        for (int col = 0; col < board.numCols(); col++) {
            string cur = "";
            findWords(board, lex, foundWords, visited, row, col, cur);
        }
    }

    // Calculate the total score
    int totalScore = 0;
    for (const string& word : foundWords) {
        totalScore += points(word);
    }

    return totalScore;
}

/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    // Lexicon lex = sharedLexicon();
    // EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    // EXPECT_EQUAL(points("and"), 0);
    // EXPECT_EQUAL(points("quad"), 1);
    // EXPECT_EQUAL(points("quint"), 2);
    // EXPECT_EQUAL(points("sextet"), 3);
    // EXPECT_EQUAL(points("seventh"), 4);
    // EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    // Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
    //                     {'G','H','J','K'},
    //                     {'L','M','N','P'},
    //                     {'Q','R','S','T'}};
    // EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    // Grid<char> board = {{'C','_','_','_'},
    //                     {'Z','_','_','_'},
    //                     {'_','A','_','_'},
    //                     {'_','_','R','_'}};
    // EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    // Grid<char> board = {{'C','C','_','_'},
    //                     {'C','Z','C','_'},
    //                     {'_','A','_','_'},
    //                     {'R','_','R','_'}};
    // EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}
