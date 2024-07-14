#include <iostream>
#include <fstream>
#include <cctype>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
#include "simpio.h"
using namespace std;


/*
 * Clean the punctuations(, at the beginning and end of strings,
 * convert letters to lower case, and clean no-word string
 */
string cleanToken(string s)
{
    string result;
    int begin = 0;
    int end = s.size() - 1;

    // remove leading punctuations
    while (begin <= end && ispunct(s[begin])) {
        begin++;
    }
    // remove trailing punctuations
    while (begin <= end && ispunct(s[end])) {
        end--;
    }

    // s is a pure punctuation string
    if (begin > end) {
        return "";
    } else {
        result = s.substr(begin, end - begin + 1);
        // s is a no-word string
        if (all_of(result.begin(), result.end(), ::isdigit)) {
            return "";
        }

        // convert result to lower case
        for (char& c : result) {
            c = tolower(c);
        }
        return result;
    }
}

/*
 * Gather tokens separated by space into a set, and remove the puncs before
 * and after each token
 */
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    size_t index = 0, found;

    /*
     * Split every token from 'text' and apply cleanToken function
     */
    while ((found = text.find(' ', index)) != string::npos) {
        string token = cleanToken(text.substr(index, found - index));
        if (!token.empty()) {
            tokens.add(token);
        }
        index = found + 1;
    }
    // Handle the last token
    if (text.size() > index) {
        tokens.add(cleanToken(text.substr(index)));
    }

    return tokens;
}

/*
 * Build the index set of every token in the documents in dbfile
 * @param dbfile has pairs of web links and tokens, each with a single line
 * @param index A Map, words to a Set of documents containing that word
 * @return int Number of pages(link)
 */
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    ifstream in;
    Vector<string> lines;
    Map<string, Set<string>> inverted;	// documents to words contained

    // read dbfile into a Vector
    if (!openFile(in, dbfile)) {
        error("Open " + dbfile + " error");
        return 0;
    } else {
        lines = readLines(in);
    }

    // process every line and map the corresponding result into the inverted map
    size_t numLines = lines.size();
    for (int i = 0; i < numLines; i += 2) {
        string link = lines.get(i);
        string text = lines.get(i + 1);

        // add tokens to the inverted map
        const Set<string>& tokens = gatherTokens(text);
        for (const string& token : tokens) {
            inverted[link].add(token);
        }
    }

    // convert the inverted map to index map
    for (const string& link : inverted.keys()) {
        for (const string& token : inverted.get(link)) {
            index[token].add(link);
        }
    }

    // return number of pages
    return inverted.keys().size();
}

/*
 * Find the query content in the index which is a mapping from the token to the web page
 * that appears.
 * @param query instead of only inputing some words, you can alse use '+' '-' compound search.
 * rules:	+<word> means the <word> must be included in the search result
 * 			-<word> means the word must not be included
 * 			e.g. 'red fish' query pages containing more than one of these two words.
 * 				 'red +fish' query pages containing 'red' and 'fish' at the same time.
 * 			 	 'red -fish' query pages containing 'red' but not 'fish'.
 */
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    Vector<string> tokens = stringSplit(query, ' ');

    for (const string& token : tokens) {
        switch (token[0]) {
        case '+':
            result *= index.get(toLowerCase(token.substr(1)));
            break;
        case '-':
            result -= index.get(toLowerCase(token.substr(1)));
            break;
        default:
            result += index.get(toLowerCase(token));
            break;
        }
    }
    return result;
}

/*
 * A simple searchEngine, integrating all the functionality of the above implementation,
 * including reading database file, building indexes, and performing searches based on
 * user-entered queries.
 */
void searchEngine(string dbfile)
{
    Map<string, Set<string>> index;

    cout << "Building index from file: " << dbfile << endl ;
    int numPages = buildIndex(dbfile, index);
    cout << "Indexed " << numPages << " pages containing "
         << index.size() << " unique terms." << endl;

    while (true) {
        cout << "Enter query sentence (or ENTER to quit): ";
        string query;
        getLine(cin, query);
        if (query.empty()) {
            break;
        }

        // perform the search
        Set<string> results = findQueryMatches(index, query);
        if (results.isEmpty()) {
            cout << "No results found." << endl;
        } else {
            cout << "Results:" << endl;
            for (const string& link : results) {
                cout << " " << link << endl;
            }
        }
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


STUDENT_TEST("More test of compound queries of findQueryMatches") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish +green");
    EXPECT_EQUAL(matchesRedOrFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red fish -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
    Set<string> matchesRedAndFish = findQueryMatches(index, "fish +eat -I");
    EXPECT_EQUAL(matchesRedAndFish.size(), 0);
}
