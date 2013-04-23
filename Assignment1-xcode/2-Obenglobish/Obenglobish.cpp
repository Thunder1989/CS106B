/*
 * File: Obenglobish.java
 * ----------------------
 * Name: Darren Hau
 * Section: Lingtong
 * -------
 * This program allows the user to use an Obenglobish translator.  The user specifies a word to translate and the program prints out the obenglobish translation.
 */

#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "strlib.h"
#include "set.h"
using namespace std;

/* Constants */
string str1 = "ob";

/* Function prototypes */
Set<char> setFromString(string str);
string obenglobish(string word, string str);

/*
 * Constant sets
 * These sets are initialized to contain the characters in the corresponding character classes.
 */
const Set<char> VOWELS = setFromString("AEIOUaeiou");

/* Main program */

int main() {
    cout << "Welcome to the Obenglobish translator!" << endl;
    cout << "To exit the program, hit ENTER without entering a word and close the console." << endl;
    while (true) {
        string word = getLine("Enter a word: ");
        if (word == "") break;
        
        string trans = obenglobish(word, str1);
        cout << word << " -> " << trans << endl;
    }
    cout << "Thank you for saving a dying language!" << endl;
    return 0;
}

/* 
 * Function: obenglobish
 * Usage: string trans = obenglobish(word, str)
 * --------------
 * This function accepts a string word and string str as its parameters and applies the obenglobish set of rules to translate it.  The rules are as follows:
 * if it is a vowel and prevVowel == 0, insert str before the vowel and flag prevVowel
 * if it is a vowel and prevVowel == 1, do nothing
 * if it is an 'e' at the end of the string, do nothing
 */
string obenglobish(string word, string str) {
    string result = "";
    bool prevVowel = 0;                     // vowel flag
    int sizeOfString = word.size();
    
    for (int i = 0; i < sizeOfString; i++) {
        if (VOWELS.contains(word[i])) {
            if (!(i == (sizeOfString - 1) && word[i] == 'e')) { // if not an 'e' at the end of a word
                if (prevVowel == 0) {       // if there was not a vowel immediately before
                    result += str;
                    prevVowel = 1;
                }
            }
        }
        else {                              // if current char is not a vowel
            if (prevVowel == 1)             // reset the vowel flag
                prevVowel = 0;
        }
        result += word[i];
    }
    return result;
}

/* Helper function to create a set from a string of characters */

Set<char> setFromString(string str) {
    Set<char> set;
    for (int i = 0; i < str.length(); i++) {
        set.add(str[i]);
    }
    return set;
}