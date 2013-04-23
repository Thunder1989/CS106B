/*
 * File: WordLadder.cpp
 * --------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the word ladder problem
 * on Assignment #2.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include "console.h"
#include "lexicon.h"
#include "queue.h"
#include "simpio.h"
#include "vector.h"
#include "set.h"
using namespace std;

/* Constants */
bool debugFlag = 1;

/* Function Prototypes */
Set<char> setFromString(string str);
char stringToChar(string str);
bool newWords(string target, string ch, Vector<string> &neighborWords);
void printLadder(Vector<string> &ladder);
string isWord(string target, string ch, int n);
void debug(bool flag, string str);

/*
 * Constant sets
 * These sets are initialized to contain the characters in the corresponding character classes.
 */
const Set<char> UPPER_SET = setFromString("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
const Set<char> LOWER_SET = setFromString("abcdefghijklmnopqrstuvwxyz");

/* Main */

int main() {
    Lexicon english("EnglishWords.dat");
    
    string startWord = getLine("Type in starting word: ");
    string endWord = getLine("Type in ending word: ");
    
    cout << "Searching..." << endl;

    /*
    while (true) {
        Vector<string> neighborWords;
        string replaceString = getLine("Enter replacement letter: ");
        
        // Checks that entered string is a valid character
        char replaceChar = stringToChar(replaceString);
        bool validLetter = (replaceString.size() == 1) && (UPPER_SET.contains(replaceChar) || LOWER_SET.contains(replaceChar));
        
        if (!validLetter)
            break;
        else {
            bool replacePossible = newWords(target, replaceString, neighborWords);
            if (replacePossible) {
                int neighbor_size = neighborWords.size();
                cout << "Neighboring words are: " << endl;
                for (int i = 0; i < neighbor_size; i++)
                    cout << neighborWords[i] << endl;
            }
            else
                cout << "No neighboring words exist by replacing " << replaceString << "." << endl;
        }
    }
    */
    
    /* create queue
     * initialize with 1 ladder Vector with start word
     * initialize Set previousWords
     * while (queue is not empty) {
     *      dequeue ladder
     *      if (last word in ladder = end word) {
     *          return ladder
     *      }
     *      for (each letter) {
     *          if (neighboring words exist || haven't been used yet) {
     *              copy current ladder
     *              add neighboring words to ladder
     *              enqueue new ladder
     *          }
     *      }
     * }
     */
    
    Queue< Vector<string> > allLadders;
    Vector<string> seed;                // Initialize starting word in seed ladder
    seed.add(startWord);
    allLadders.enqueue(seed);           // Initialize allLadders with seed ladder
    
    Set<string> prevWords;              // Set of previous words to avoid retracing ladders
    prevWords.add(startWord);
    
    while (!allLadders.isEmpty()) {     // While the queue is not empty.  Previous ladders are dequeued and discarded.
        Vector<string> testLadder = allLadders.dequeue();
        int ladderLength = testLadder.size();
        string lastWord = testLadder[ladderLength-1];
        
        if (lastWord == endWord) {      // Success case
            cout << "The shortest word ladder is: " << endl;
            printLadder(testLadder);
            return 0;
        }
        for (char ch = 'a'; ch <= 'z'; ch++) {      // Using each letter in the alphabet...
            string strCh = string() + ch;           // Turn the character into a string...
            int length = lastWord.size();
//            cout << "The last word is ";
//            debug(debugFlag, lastWord);
//            cout << "Testing: ";
//            debug(debugFlag, strCh);
            
            for (int i = 0; i < length; i++) {      // Replace and test each letter in lastWord
                string nextWord = isWord(lastWord, strCh, i);
//                cout << i << endl;
                
                if (!(nextWord == "")) {
                    if (!prevWords.contains(nextWord)) {            // if word hasn't been used before
                        Vector<string> neighborWords = testLadder;  // Create copy of testLadder
                        neighborWords += nextWord;                  // append it to neighborWords
                        allLadders.enqueue(neighborWords);          // enqueue new ladder
                        prevWords.add(nextWord);                    // add to prevWords
//                        debug(debugFlag, nextWord);
                    }
                }
            }
        }
    }
    cout << "There are no word ladders from " << startWord << " to " << endWord << "." << endl;
    
	return 0;
}

void debug(bool flag, string str) {
    cout << str << endl;
}

/* Function: isWord
 * Usage: 
 * -------
 * This function accepts as parameters a baseline word to modify, a modifying letter ch, and a position n to replace with ch.  If the resulting string is a valid word, isWord returns the new word.  If the resulting string is not valid, isWord returns "".
 */
string isWord(string target, string ch, int n) {
    Lexicon english("EnglishWords.dat");
    
    string test = target;
    test = test.replace(n,1,ch);
    if (english.contains(test) && (test != target)) {
        return test;
    }
    else {
        return "";
    }
}

/* Function: newWords
 * Usage: bool replacePossible = newWords(target, replaceString, neighborWords);
 * -----
 * This function accepts an input string ch and uses it to replace each letter position
 * in the target word.  If this produces new words, newWords returns true and changes the
 * reference parameter neighborWords to store every valid word that results.
 * -----
 * Pseudo:
 * for each character in the target
 *      replace with character ch
 *      if this results in a word that is different
 *          add to neighborWords
 */

bool newWords(string target, string ch, Vector<string> &neighborWords) {
    Lexicon english("EnglishWords.dat");
    
    int length = target.size();
    for (int i = 0; i < length; i++) {
        string test = target;
        test = test.replace(i,1,ch);
        if (english.contains(test) && (test != target)) {
            neighborWords.add(test);
        }
    }
    return (neighborWords.size() != 0);     // returns 1 if neighborWords includes new words
}

/* Function: printLadder
 * Usage: printLadder(testLadder)
 * -------
 * This function prints out the strings in Vector ladder.
 */
void printLadder(Vector<string> &ladder) {
    int length = ladder.size();
    for (int i = 0; i < length; i++) {
        if (i == length - 1)
            cout << ladder[i] << endl;
        else
            cout << ladder[i] << " -> ";
    }
}

/* Helper function to create a set from a string of characters */

Set<char> setFromString(string str) {
    Set<char> set;
    for (int i = 0; i < str.length(); i++) {
        set.add(str[i]);
    }
    return set;
}

/* Function: stringToChar
 * Usage: char replaceChar = stringToChar(replaceString)
 * -------
 * This function converts a string parameter into a character if it represents a letter, and complains if the string contains more than one letter or contains a non-letter symbol.
 */

char stringToChar(string str) {
    char result;
    if (str.size() > 1) {
        cout << "Please type a single letter." << endl;
        result = '?';
    }
    else {
        char ch = str[0];
        int ascii_count = int (ch);
        if (!((65 <= ascii_count && ascii_count <= 90) || (97 <= ascii_count && ascii_count <= 122))) {
            cout << "Please avoid using numbers or symbols." << endl;
            result = '?';
        }
        else
            result = ch;
    }
    return result;
}
