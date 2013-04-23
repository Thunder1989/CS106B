/*
 * File: NumericConversion.cpp
 * ---------------------------
 * Name: Darren Hau
 * Section: Lingtong
 * -----
 * This program allows a user to input values and convert an integer to a string using the intToString function, or a string to an integer using the stringToInt function.
 */

#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
using namespace std;

/* Function prototypes */

string intToString(int n);
int stringToInt(string str);
int intDebugger(bool debug,int toBePrinted);

/* Main program */

int main() {
    while (true) {
        int choice = getInteger("Would you like to convert 1) int -> string, or 2) string -> int? ");
        if (choice == 1) {
            int testInt = getInteger("Enter integer: ");
            string result_intToString = intToString(testInt);
            cout << "The number is " << result_intToString << endl;
            
            result_intToString += " bunnies";           // string operations
            cout << result_intToString << " say I converted an integer to a string!" << endl;
        }
        else if (choice == 2) {
            string str1 = getLine("Enter string: ");
            int result_stringToInt = stringToInt(str1);
            cout << "The number is " << result_stringToInt << endl;
            
            int addProof = result_stringToInt + 1;      // integer operations
            cout << "Look, I can add 1 to " << result_stringToInt << " to get " << addProof << "!" << endl;
        }
        else
            cout << "Sorry, please choose either 1 or 2." << endl;
    }
    return 0;
}

/*
 * Function: stringToInt
 * Usage: int result = stringToInt(str)
 * -------
 * This function converts a string str into an integer that represents the same number.  If the input is not a valid number, the function prints out an error message warning the user that the output may be incorrect.
 * -------
 * Pseudo:
 * if empty string or "-", return 0
 * else
 * isolate last character and convert to integer
 * erase last character from str
 * return nextDigit + 10*stringToInt(str)
 * if str is negative, return 0 - nextDigit + 10*stringToInt(str)
 */

int stringToInt(string str) {
    int result;
    if ((str == "") || (str == "-")) {
        result = 0;
    }
    else {
        int n = str.size();
        char nextChar = str[n-1];                   // get last char
        int nextDigit = (int) (nextChar - 48);
        if (!(0 <= nextDigit && nextDigit <= 9)) {  // if the input is not a valid number
            cout << "Sorry, I don't understand.  The output may be wrong." << endl;
        }
        else {
            string shortStr = str.substr(0,n-1);    // generate substring lacking last char
            if (str.find("-") == 0)
                result = 10*stringToInt(shortStr) + (0 - nextDigit);
            else
                result = 10*stringToInt(shortStr) + nextDigit;
        }
    }
    return result;
}

/*
 * Function: intToString
 * Usage: string result = intToString(number)
 * -----
 * This function converts an integer n into a string that represents the same number.
 * -----
 * Pseudo:
 * if user enters 0, return "0"...otherwise
 * isolate last digit and convert to char
 * divide by 10 and truncate
 * use recursion for preceding string
 * if reached end of recursion...
 * if negative, replace "0" with "-"; if positive, eliminate "0"
 * return recursive string + isolated digit
 */

string intToString(int n) {
    string result;
    if (n == 0)                         // base case
        result = "0";
    else {
        int mod = n%10;                 // isolate last digit
        int m = n/10;
        
        string precedingString;
        char nextDigit;
        
        if (n < 0) {                            // if entry is negative
            nextDigit = (char) (48 - mod);      // convert to ASCII char
            if (intToString(m) == "0")
                precedingString = "-";
            else
                precedingString = intToString(m);
        }
        else {                                  // if entry is positive
            nextDigit = (char) (48 + mod);      // convert to ASCII char
            if (intToString(m) == "0")
                precedingString = "";
            else
                precedingString = intToString(m);
        }
        
        result = precedingString + nextDigit;
    }
    return result;
}