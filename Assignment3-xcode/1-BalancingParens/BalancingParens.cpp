/*
 * File: BalancingParens.cpp
 * -------------------------
 * Name: Darren Hau
 * Section: Lingtong
 * -----
 * The Main program prompts the user for an expression using parentheses, braces, and brackets and determines whether the expression is balanced.
 */

#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "set.h"
using namespace std;

/* Function prototypes */
Set<char> setFromString(string str);
bool hasOnlyBraces(string str);
bool isBalanced(string exp);
int eraseMatchingPair(string & exp);

/* Constant sets
 * These sets are initialized to contain the characters in the corresponding character classes.
 */
const Set<char> BRACES = setFromString("(){}[]");

/* Main program */
int main() {
    while (true) {
        string str = getLine("Enter an expression with only ()s, {}s, and []s: ");
        bool valid = hasOnlyBraces(str);
        if (!valid) {
            cout << "Please use only parentheses, braces, or brackets!" << endl;
            break;
        }
        bool balanced = isBalanced(str);
        if (balanced == 1)
            cout << "This expression is balanced :)" << endl;
        else
            cout << "This expression is not balanced :{" << endl;
    }
    return 0;
}

/*
 * Function: isBalanced
 * Usage: if (isBalanced(exp)) . . .
 * ---------------------------------
 * Returns true if the string exp is balanced, which means that it
 * is either (a) empty or (b) contains a consecutive paired operator
 * and the string is balanced if you remove that operator.
 */

bool isBalanced(string exp) {
    if (exp == "")
        return true;
    else {
        int x = eraseMatchingPair(exp);
        if (x == string::npos)
            return false;
        return isBalanced(exp);
    }
}

/*
 * Function: eraseMatchingPair
 * Usage: int x = eraseMatchingPair(exp)
 * -------
 * This function searches for an instance of (), {}, or [] and returns the position of the first occurance and erases it from the string exp passed by reference.  If a matching pair does not exist, it returns string::npos.
 */
int eraseMatchingPair(string & exp) {
    int pos = exp.find("()");
    if (pos == string::npos) {
        pos = exp.find("{}");
        if (pos == string::npos) {
            pos = exp.find("[]");
        }
    }
    if (pos != string::npos)
        exp.erase(pos, 2);
    return pos;
}

/* 
 * Function: hasOnlyBraces
 * Usage: 
 * ------
 * This function returns true if the given string contains only characters in the set BRACES, and false otherwise.
 */
bool hasOnlyBraces(string str) {
    int length = str.size();
    for (int i = 0; i < length; i++) {
        if (!BRACES.contains(str[i]))
            return false;
    }
    return true;
}

/* Helper function to create a set from a string of characters */
Set<char> setFromString(string str) {
    Set<char> set;
    for (int i = 0; i < str.length(); i++) {
        set.add(str[i]);
    }
    return set;
}