/*
 * File: KarelGoesHome.cpp
 * -----------------------
 * Starter file for the KarelGoesHome problem.
 */

#include <iostream>
#include <iomanip>
#include "console.h"
#include "simpio.h"
using namespace std;

/* Function prototypes */

int countPaths(int street, int avenue);

/* Main program */

int main() {
    int street = getInteger("Which street is Karel on? ");
    int avenue = getInteger("Which avenue is Karel on? ");
    int numPaths = countPaths(street, avenue);
    cout << "There are " << numPaths << " paths back to Karel's home." << endl;
    return 0;
}

/*
 * Function: countPaths
 * Usage: int nPaths = countPaths(street, avenue);
 * -----------------------------------------------
 * Counts the paths from a particular street/avenue position in
 * Karel's world to the origin, subject to the restriction that
 * Karel can move only west and south.
 */

int countPaths(int street, int avenue) {
    while (!(street <= 0 || avenue <= 0)) {
        if (street == 1 && avenue == 1)
           return 1;
        else {
            return countPaths(street, avenue - 1) + countPaths(street - 1, avenue);
        }
    }
    return 0;
}
