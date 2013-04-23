/*
 * File: ListCompletions.cpp
 * -------------------------
 * Name: Darren Hau
 * Section: Lingtong
 * --------
 * This program prompts the user for a string of digits and generates possible words based on a standard digit-to-letter mapping.
 */

#include <iostream>
#include "console.h"
#include "simpio.h"
#include "lexicon.h"
using namespace std;

/* Function prototypes */
void listCompletions(string digits, Lexicon & lex);
void initializeMap(Map<string, string> & map);
void generatePrefixes(string digits, string prefix, Map<string, string> &conversion, Lexicon & lex, Vector<string> & results);
void generateExtensions(string prefix, Lexicon & lex, Vector<string> & results);

/* Main program */
int main() {
   Lexicon english("EnglishWords.dat");
   cout << "Cell phone mind-reading problem:" << endl;
   while (true) {
      string digits = getLine("Enter a set of digits: ");
      if (digits == "" || digits == "quit") break;
      cout << "The words beginning with those digits are:" << endl;
      listCompletions(digits, english);
   }
   return 0;
}

/*
 * Function: listCompletions
 * Usage: listCompletions(digits, lex);
 * ------------------------------------
 * Lists all the words in the lexicon that begin with the letters
 * corresponding to the specified digits on a telephone keypad.
 */

void listCompletions(string digits, Lexicon & lex) {
    Map<string, string> digitsToLetters;        // initialize Mapping of digits to letters
    initializeMap(digitsToLetters);
    
    Vector<string> possibleWords;
    generatePrefixes(digits, "", digitsToLetters, lex, possibleWords);
    
    cout << possibleWords.toString() << endl;
}

/* Function: generateExtensions
 * Usage: generateExtensions(prefix, lex, results)
 * -----
 * This function extends a given prefix and adds words to a Vector results if the extension generates a valid word in Lexicon lex.  It first checks if the prefix is valid - if so, it iterates through each letter in the alphabet and appends the new letter to the prefix and calls generateExtensions recursively.  It then checks if the prefix is already a word, in which case it adds the word to results.
 */
void generateExtensions(string prefix, Lexicon & lex, Vector<string> & results) {
    if (lex.containsPrefix(prefix)) {
        for (char i = 'A'; i <= 'Z'; i++) {
            string nextPrefix = prefix + i;
            generateExtensions(nextPrefix, lex, results);
        }
    }
    if (lex.contains(prefix)) {
        results += prefix;
        return;
    }
}

/*
 * Function: generatePrefixes
 * Usage: generatePrefixes(digits, "", prefixes, digitsToLetters)
 * -----
 * This function builds a recursive tree to generate all the prefixes associated with the input digits.  Once a prefix has been made it is called in generateExtensions to generate possible words.  A Map 'conversion' maps the input digits to the proper letters.
 */
void generatePrefixes(string digits, string prefix, Map<string, string> &conversion, Lexicon & lex, Vector<string> & results) {
    if (digits.size() == 0) {                   // once a prefix is complete, calls generateExtensions
        generateExtensions(prefix, lex, results);
        return;
    }
    else {
        string key = "";
        key = digits[0];                        // takes the first digit remaining
        string letterChoices = conversion[key]; // maps digit to its corresponding string of letters
        int numLetters = letterChoices.size();
        for (int i = 0; i < numLetters; i++) {  // calls recursively for each possible next letter
            string chosenLetter = "";
            chosenLetter = letterChoices[i];
            string nextDigits = digits.substr(1);
            generatePrefixes(nextDigits, prefix + chosenLetter, conversion, lex, results);
        }
    }
}

/*
 * Function: initializeMap
 * -----
 * Initializes a map that links digits to letters.
 */
void initializeMap(Map<string, string> & map) {
    string key = "";
    string value = "";
    for (char digit = '2'; digit <= '9'; digit++) {
        switch (digit) {
            case '2': value = "ABC";
                break;
            case '3': value = "DEF";
                break;
            case '4': value = "GHI";
                break;
            case '5': value = "JKL";
                break;
            case '6': value = "MNO";
                break;
            case '7': value = "PQRS";
                break;
            case '8': value = "TUV";
                break;
            case '9': value = "WXYZ";
                break;
            default: value = "";
                break;
        }
        key = digit;
        map[key] = value;
    }
}
