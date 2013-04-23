/*
 * File: BinaryEncoding.cpp
 * ------------------------
 * Starter file for the binary encoding problem on Assignment #3.
 */

#include <iostream>
#include "console.h"
#include "simpio.h"
#include "error.h"
using namespace std;

/* Function prototypes */

void generateBinaryCode(int nBits);
void generateBinaryCode(string prefix, int nBits);

/* Main program */

int main() {
    int nBits = getInteger("Number of bits: ");
    generateBinaryCode(nBits);
    return 0;
}

/*
 * Function: generateBinaryCode
 * Usage: generateBinaryCode(nBits);
 * ---------------------------------
 * Lists all the binary values containing the specified number of bits.
 */

void generateBinaryCode(int nBits) {
    if (nBits <=0) error("The number of bits must be positive.");
    generateBinaryCode("", nBits);
}

void generateBinaryCode(string prefix, int nBits) {
    if (nBits == 0)
        cout << prefix << endl;
    else {
        generateBinaryCode(prefix + "0", nBits-1);
        generateBinaryCode(prefix + "1", nBits-1);
    }
}