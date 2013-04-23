/*
 * File: Combinations.cpp
 * ----------------------
 * Name: Darren Hau
 * Section: Lingtong
 * -----
 * This program allows the user to input n and k in the combinations function c(n,k) and returns the coefficient c(n,k).  If the user exits the program, it prompts the user for a number numRows and prints out the first numRows rows of Pascal's Triangle.
 */

#include <iostream>
#include <iomanip>
#include "console.h"
#include "simpio.h"
using namespace std;

/* Constants */

/* Function Prototypes */
int combinations(int n, int k);
void pascal(int n);

/* Main Function */

int main() {
    cout << "Combinations Function c(n,k): To end program, enter -1 for n and close the console." << endl;
    while (true) {
        int n = getInteger("What is n? ");
        
        if (n == -1) break;
        
        int k = getInteger("What is k? ");
        int coeff = combinations(n,k);
        if (coeff == -1)
            cout << "Invalid entry: n, k >= 0 and n must be greater than k." << endl;
        else
            cout << "c(" << n << "," << k << ") = " << coeff << endl;
    }
    
    int numRows = getInteger("How many rows of Pascal's Triangle do you want? ");
    if (numRows <= 0) {
        cout << "Sorry, you need to enter a positive number!" << endl;
    }
    else
        pascal(numRows);
    
    return 0;
}

/*
 * Function: combinations
 * Usage: int coeff = combinations(n,k)
 * --------
 * This function takes in n and k parameters and returns the combinations coefficient c(n,k).
 * --------
 * Pseudo:
 * c(n,k) = c(n-1,k-1) + c(n-1,k)
 * where c(n-1,k-1) == 0 when k-1 < 0
 * and c(n-1,k) == 0 when k == n.
 * c(0,0) is seeded as 1
 * return -1 if invalid case
 */
int combinations(int n, int k) {
    int output;
    if (n < 0 || k < 0 || k > n)            // invalid case
        output = -1;
    else if (n == 0 && k == 0)              // seed case c(0,0)
        output = 1;
    else if ((k-1) < 0)                     // left edge of Pascal's triangle
        output = 0 + combinations(n-1,k);
    else if (k == n)                        // right edge of Pascal's triangle
        output = combinations(n-1,k-1) + 0;
    else
        output = combinations(n-1,k-1) + combinations(n-1,k);
    return output;
}

/*
 * Function: pascal
 * Usage: pascal(numRows)
 * -----
 * This function prints out the first numRows rows of Pascal's Triangle.
 */
void pascal(int n) {
    cout << "Pascal's Triangle" << endl;
    for (int i = 0; i < n; i++) {
        cout << setw(30-2*i) << combinations(i,0);
        for (int j = 1; j <= i; j++) {
            cout << setw(4) << combinations(i,j);
        }
        cout << endl;
    }
}