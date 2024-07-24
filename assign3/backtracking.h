#pragma once

/* Needed for boggle.cpp */
#include "grid.h"
#include "lexicon.h"
using namespace std;

int scoreBoard(Grid<char>& board, Lexicon& lex);
void findWords(Grid<char>& board, Lexicon& lex, Set<string>& foundWords,
               Grid<bool>& visited, int row, int col, string& soFar);

/* Needed for voting.cpp */
#include "vector.h"

Vector<int> computePowerIndexes(Vector<int>& blocks);
