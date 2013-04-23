/*
 * File: WordLadder.cpp
 * --------------------
 * Name: Darren Hau
 * Section: Lingtong
 * This file is the starter project for the word ladder problem
 * on Assignment #2.
 * This program finds the shortest word ladder between two user-inputted words.  The word ladders are stored in a Queue which is initialized with the first word ladder consisting of the starting word.  While the Queue is not empty and the ladder had not been found, the program dequeues the next ladder and sees if it bridges the start and end words (in which case it prints it out and prompts the user for the next pair of words).  If the current ladder does not satisfy, the program replaces each letter in the last word with each letter in the alphabet.  If the result is a valid word, it copies the current ladder, adds the new word to it, and enqueues the result.  There is a Set of previous words used to avoid retracing word sequences.
 */

#include <iostream>
#include "console.h"
#include "lexicon.h"
#include "queue.h"
#include "simpio.h"
#include "vector.h"
#include "set.h"
using namespace std;

/* Function Prototypes */
Set<char> setFromString(string str);
char stringToChar(string str);
bool newWords(string target, string ch, Vector<string> &neighborWords);
void printLadder(Vector<string> &ladder);
string isWord(Lexicon language, string target, string ch, int n);
void findNextLadder(Lexicon & language, string & target, Vector<string> & testLadder, Queue< Vector<string> > & allLadders, Set<string> & prevWords);

/* Main */
int main() {
    cout << "Welcome to Word Ladder!  Hit RETURN to exit." << endl;
    Lexicon english("EnglishWords.dat");
    string startWord = "";
    string endWord = "";
    
    while (true) {
        startWord = getLine("Type in starting word: ");
        if (startWord == "") {
            cout << "Program finished.  Please close the console." << endl;
            break;
        }
        if (!english.contains(startWord)) {
            cout << "Sorry, " << startWord << " is not an English word.  Program finished." << endl;
            break;
        }
        endWord = getLine("Type in ending word: ");
        if (!english.contains(endWord)) {
            cout << "Sorry, " << endWord << " is not an English word.  Program finished." << endl;
            break;
        }
        if (startWord.size() != endWord.size()) {
            cout << "No word ladder exists between words of different lengths!" << endl;
            cout << "Program finished.  Please close the console." << endl;
            break;
        }
        
        cout << "Searching..." << endl;
        
        Queue< Vector<string> > allLadders;
        Vector<string> seed;                // Initialize starting word in seed ladder
        seed.add(startWord);
        allLadders.enqueue(seed);           // Initialize allLadders with seed ladder
        
        Set<string> prevWords;              // Set of previous words to avoid retracing ladders
        prevWords.add(startWord);
        bool ladderFound = false;
        
        while (!(allLadders.isEmpty() || ladderFound)) {
            Vector<string> testLadder = allLadders.dequeue();   // previous ladders are discarded
            int ladderLength = testLadder.size();
            string lastWord = testLadder[ladderLength-1];
            
            if (lastWord == endWord) {                  // Success!
                cout << "The shortest word ladder is: " << endl;
                printLadder(testLadder);
                ladderFound = true;
            }
            else {
                findNextLadder(english, lastWord, testLadder, allLadders, prevWords);
            }
        }
        if (!ladderFound)
            cout << "There are no word ladders from " << startWord << " to " << endWord << "." << endl;
    }
	return 0;
}

/* Function: findNextLadder
 * Usage: findNextLadder(english, lastWord, testLadder, allLadders, prevWords)
 * ------
 * This function iterates through each letter in the target string and replaces it with each letter in the alphabet.  If the resulting string is a valid word in the Lexicon, the function creates a copy of the testLadder, appends the new word to the new ladder, and enqueues the new ladder to allLadders.  The function adds the word to the Set prevWords to avoid retracing word sequences.
 */
void findNextLadder(Lexicon & language, string & target, Vector<string> & testLadder, Queue< Vector<string> > & allLadders, Set<string> & prevWords) {
    for (char ch = 'a'; ch <= 'z'; ch++) {              // Using each letter in the alphabet...
        string strCh = string() + ch;                   // Turn the character into a string...
        int length = target.size();
        
        for (int i = 0; i < length; i++) {              // Replace and test each letter in lastWord
            string nextWord = isWord(language, target, strCh, i);
            
            if (!(nextWord == "")) {
                if (!prevWords.contains(nextWord)) {    // if word hasn't been used before
                    Vector<string> neighborWords = testLadder;  // Create copy of testLadder
                    neighborWords += nextWord;          // append word to neighborWords
                    allLadders.enqueue(neighborWords);  // enqueue new ladder
                    prevWords.add(nextWord);            // add word to prevWords
                }
            }
        }
    }
}

/* Function: isWord
 * Usage: string nextWord = isWord(english, lastWord, strCh, i)
 * -------
 * This function accepts as parameters a baseline word to modify, a modifying letter ch, and a position n to replace with ch.  If the resulting string is a valid word, isWord returns the new word.  If the resulting string is not valid, isWord returns "".
 */
string isWord(Lexicon language, string target, string ch, int n) {
    string test = target;
    test = test.replace(n,1,ch);
    if (language.contains(test) && (test != target)) {
        return test;
    }
    else {
        return "";
    }
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