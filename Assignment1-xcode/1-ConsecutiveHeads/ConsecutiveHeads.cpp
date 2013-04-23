/*
 * File: ConsecutiveHeads.cpp
 * --------------------------
 * Name: Darren Hau
 * Section: Lingtong
 * ---------
 * This program allows the user to set a desired number of repeat heads, and returns the number of flips made in a particular random scenario to achieve the desired number of repeat heads.
 */

#include <iostream>
#include <string>
#include "console.h"
#include "random.h"
#include "simpio.h"
using namespace std;

/* Function Prototypes */
int consecutiveHeads(int required);

/* Main */

int main() {
    while (true) {
        int numHeadsRequired = getInteger("Enter # of consecutive heads desired: ");
        if (numHeadsRequired <= 0) {
            cout << "Please enter a positive value." << endl;
        }
        else {
            int numFlipsRequired = consecutiveHeads(numHeadsRequired);
            cout << "It took " << numFlipsRequired << " flips to get " << numHeadsRequired << " consecutive heads." << endl;
        }
    }
   return 0;
}

/*
 * Function: consecutiveHeads
 * Usage: int numFlips = consecutiveHeads(required)
 * --------
 * Returns the number of flips required to obtain the required number of heads.
 */

int consecutiveHeads(int required) {
    int repeat = 0;
    int numFlips = 0;
    while (repeat != required) {
        bool result = randomChance(.50);
        if (result == 0) {
            cout << "Tails" << endl;
            repeat = 0;
        }
        else {
            cout << "Heads" << endl;
            repeat++;
        }
        numFlips++;
    }
    return numFlips;
}
