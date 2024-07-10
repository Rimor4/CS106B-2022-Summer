/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "soundex.h"
#include "filelib.h"
#include "simpio.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
#include "vector.h"
#include <cctype>
#include <fstream>
#include <string>

static const string outputFile = string("res/soundexes.txt");

using namespace std;

/*
 * The soundex is a coded surname (last name) index based on the way a surname
 * sounds. According to https://www.archives.gov/research/census/soundex.
 */
string soundex(string s) {
  s = toUpperCase(removeNonLetters(s));
  removeDoubleLetters(s);
  removeAdjacentDuplicates(s);
  removeVowels(s);

  return toSoundexString(s);
}

/*
 * Search a file that contains strings of surname, and store each
 * string to the database(A Vector<string>).
 */
void soundexSearch(string filepath) {
  // The proivded code opens the file with the given name
  // and then reads the lines of that file into a vector.
  ifstream in;
  ofstream out;
  Vector<string> databaseNames, soundexes;

  readEntireFile(filepath, in, databaseNames);
  cout << "Read file " << filepath << ", " << databaseNames.size()
       << " names found." << endl;

  // The names in the database are now stored in the provided
  // vector named databaseNames

  /* Apply the soundex algorithm to the database and write the result to a new file */
  soundexes = soundexAll(databaseNames);
  writeEntireFile(outputFile, out, soundexes);
  cout << "Write file " << outputFile << ", " << soundexes.size()
       << " names output." << endl;
}

/*
 * Apply the soundex algorithm to an entire Vector<string>.
 */
Vector<string> soundexAll(const Vector<string> &database) {
  Vector<string> result;
  for (const string &s : database) {
    result.add(soundex(s));
  }
  return result;
}

/*
 * Read a file containing strings in lines and store them in a Vector.
 */
void readEntireFile(const string &filename, ifstream &in,
                    Vector<string> &database) {
  in.open(filename);
  if (!in.is_open()) {
    error("Failed to open the file: " + filename);
  } else {
    database = readLines(in);
  }
  in.close();
}

/*
 * Write strings in lines stored in a Vector to a file.
 */
void writeEntireFile(const string &filename, ofstream &out,
                     Vector<string> &data, ios_base::openmode mode) {
  // default open mode is trunc
  out.open(outputFile, mode);
  if (!out.is_open()) {
    error("Failed to open the output file: " + outputFile);
  } else {
    for (const string &s : data) {
      out << s << endl;
    }
    out.close();
  }
}

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!
 *
 * when s[0] is not a letter, original function mistakenly
 * use it as the first character of the result.
 */
string removeNonLetters(const string &s) {
  string result;
  for (int i = 0; i < s.length(); i++) {
    if (isalpha(s[i])) {
      result += s[i];
    }
  }
  return result;
}

/*
 * If the surname has any double letters, they should be treated as one
 * letter.
 */
void removeDoubleLetters(string &s) {
  for (int i = s.size() - 1; i > 0; i--) {
    if (s[i] == s[i - 1]) {
      // suppose there are no surname has triple letters.
      s.erase(i--, 1);
    }
  }
}

/*
 * If the surname has different letters side-by-side that have the same number
 * in the soundex coding guide, they should be treated as one letter.
 */
void removeAdjacentDuplicates(string &s) {
  for (int i = s.size() - 1; i > 0; i--) {
    int front = mapToSoundex(s[i - 1]);
    int rear = mapToSoundex(s[i]);
    if (front == rear && front != 0) {
      s.erase(i, 1);
    }
  }
}

/*
 * Simply remove vowel characters in the vector,
 * expect for initials
 */
void removeVowels(string &s) {
  for (int i = s.size() - 1; i > 0; i--) {
    if (!mapToSoundex(s[i])) {
      s.erase(i, 1);
    }
  }
}

/*
 * In the end, map each character to soundex number and add zero
 * or truncate to four digis, generate the final soundex string.
 */
string toSoundexString(const string &s) {
  if (s.empty()) {
    error("s cannot be empty");
    return string("0000");
  }

  int size = s.size();
  string result = charToString(s[0]);
  for (int i = 1; i < 4; i++) {
    if (i == size) {
      // Complete with zero
      for (int k = 0; k < 4 - i; k++) {
        result += '0';
      }
      return result;
    }
    result += integerToChar(mapToSoundex(s[i]));
  }
  return result;
}

/*
 * Maps individual characters to sounde numbers
 */
int mapToSoundex(const char &c) {
  if (c == 'B' || c == 'F' || c == 'P' || c == 'V')
    return 1;
  if (c == 'C' || c == 'G' || c == 'J' || c == 'K' || c == 'Q' || c == 'S' ||
      c == 'X' || c == 'Z')
    return 2;
  if (c == 'D' || c == 'T')
    return 3;
  if (c == 'L')
    return 4;
  if (c == 'M' || c == 'N')
    return 5;
  if (c == 'R')
    return 6;
  // discard the letters A, E, I, O, U, H, W, and Y.
  return 0;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
  string s = "O'Hara";
  string result = removeNonLetters(s);
  EXPECT_EQUAL(result, "OHara");
  s = "Planet9";
  result = removeNonLetters(s);
  EXPECT_EQUAL(result, "Planet");
  s = "tl dr";
  result = removeNonLetters(s);
  EXPECT_EQUAL(result, "tldr");
}

PROVIDED_TEST("Sample inputs from handout") {
  EXPECT_EQUAL(soundex("Curie"), "C600");
  EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
  EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
  EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
  EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
  EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
  EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
  EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
  EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
  EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
  EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
  EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
  // Some versions of Soundex make special case for consecutive codes split by
  // hw We do not make this special case, just treat same as codes split by
  // vowel
  EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// Add your test cases here
STUDENT_TEST("Test removeNonLetters correctness") {
  string s = "$%&* (";
  EXPECT_EQUAL(removeNonLetters(s), "");

  s = "O'Conner";
  EXPECT_EQUAL(removeNonLetters(s), "OConner");

  s = "Planet9";
  EXPECT_EQUAL(removeNonLetters(s), "Planet");

  s = "tl dr";
  EXPECT_EQUAL(removeNonLetters(s), "tldr");
}

STUDENT_TEST("Test removeDoubleLetters correctness") {
  string s = "string";
  removeDoubleLetters(s);
  EXPECT_EQUAL(s, "string");

  s = "aabbcc";
  removeDoubleLetters(s);
  EXPECT_EQUAL(s, "abc");

  s = " BBall";
  removeDoubleLetters(s);
  EXPECT_EQUAL(s, " Bal");
}

STUDENT_TEST("Test removeAdjacentDuplicates correctness") {
  string s = "PFISTER";
  removeAdjacentDuplicates(s);
  EXPECT_EQUAL(s, "PISTER");

  s = "JACKSON";
  removeAdjacentDuplicates(s);
  EXPECT_EQUAL(s, "JACON");

  s = "TYMCZAK";
  removeAdjacentDuplicates(s);
  EXPECT_EQUAL(s, "TYMCAK");
}

STUDENT_TEST("Test removeVowels correctness") {
  string s = "PISTER";
  removeVowels(s);
  EXPECT_EQUAL(s, "PSTR");

  s = "JACKSON";
  removeVowels(s);
  EXPECT_EQUAL(s, "JCKSN");

  s = "TYMCZAK";
  removeVowels(s);
  EXPECT_EQUAL(s, "TMCZK");

  s = "AEIOUHWY";
  removeVowels(s);
  EXPECT_EQUAL(s, "A");
}
