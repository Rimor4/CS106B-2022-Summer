/**
 * File: soundex.h
 *
 * This is just here to communicate the prototype
 * information for the functions in soundex.cpp that
 * will be called from main.cpp
 */
#pragma once
#include "vector.h"
#include <string>
using namespace std;

void soundexSearch(std::string filepath);
string soundex(std::string s);
string removeNonLetters(const string &s);
void removeDoubleLetters(string &s);
void removeAdjacentDuplicates(string &s);
void removeVowels(string &s);
string toSoundexString(const string &s);
int mapToSoundex(const char &c);
void readEntireFile(const string &filename, ifstream &in,
                    Vector<string> &database);
void writeEntireFile(const string &filename, ofstream &out,
                     Vector<string> &data,
                     ios_base::openmode mode = ios_base::trunc);
Vector<string> soundexAll(const Vector<string> &database);
