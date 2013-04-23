/*
 * File: RandomWriter.cpp
 * ----------------------
 * Name: Darren Hau
 * Section: Lingtong
 * This file is the starter project for the random writer problem
 * on Assignment #2.
 * [TODO: extend the documentation]
 */

#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include "console.h"
#include "map.h"
#include "random.h"
#include "strlib.h"
#include "vector.h"
#include "filelib.h"
#include "simpio.h"
using namespace std;

/* Constants */
bool debugFlag = 0;

/* Function Prototypes */
void analyzeWriting(ifstream & infile, Map<string, Vector<char> > & model, int order);
void printAnalysis(Map<string, Vector<char> > & model);
void randomWrite(Map<string, Vector<char> > & model);
void debug(bool flag, string str, string test);
void debug(bool flag, string str, char test);

/* Main */
int main() {
    cout << "Welcome to RandomWriter - your IHUM essays solved!  We should create a startup..." << endl << endl;
    ifstream infile;
    promptUserForFile(infile, "Input file: ");
    int order = getInteger("What order of analysis do you want (from 1 - 10)? ");
    
    Map<string, Vector<char> > writingModel;
    analyzeWriting(infile, writingModel, order);        // analyze text
    //printAnalysis(writingModel);
    
    randomWrite(writingModel);
    
    infile.close();
	return 0;
}

/* Function: randomWrite
 * Usage: randomWrite(model)
 * -------
 * This function will write random text following the parameter 'model'.  The function will first determine a seed from the string keys that occur most often in model.  In the case that there are multiple options for the seed, randomWrite will randomly select one as the currentSeed.  While the model contains currentSeed, randomWrite will randomly select a char in the Vector associated with the key currentSeed, print the char, and update currentSeed.
 */
void randomWrite(Map<string, Vector<char> > & model) {
    Vector<string> seedOptions;                         // list seed options
    int seedOccurances = 0;
    foreach(string key in model) {
        int occurances = model[key].size();
        if (occurances > seedOccurances) {              // if next key occurs more often
            seedOptions.clear();                        // clear seeds and add key
            seedOptions += key;
            seedOccurances = occurances;
        }
        if (occurances == seedOccurances) {             // if next key occurs just as often, add key
            seedOptions += key;
        }
    }
    
    int numSeeds = seedOptions.size();                  // select seed
    int selectedSeed = rand() % numSeeds;
    string currentSeed = seedOptions[selectedSeed];
    
    int charCount = currentSeed.size();
    
    while (model.containsKey(currentSeed) && (charCount < 2000)) {
        Vector<char> charOptions = model[currentSeed];  // select char
        int numChars = charOptions.size();
        int selectedChar = rand() % numChars;
        char nextChar = charOptions[selectedChar];
        
        cout << nextChar;                               // print next char
        charCount++;
        
        currentSeed.erase(0,1);                         // update currentSeed
        currentSeed += nextChar;
    }
}

/* Function: analyzeWriting
 * Usage: analyzeWriting(infile, writingModel, order);
 * ------
 * This function scans through a stream 'infile' and compiles a model of the writing style in Map 'model' based on the int 'order'.  'model' will use strings of length 'order' as keys to corresponding Vectors that store each character following the key string.  Duplicate characters are included in the Vector such that the Vector stores the frequency distribution of a particular character following the key string.
 */
void analyzeWriting(ifstream & infile, Map<string, Vector<char> > & model, int order) {
    while (true) {
        string str = "";                    // string that will serve as key
        char buffer[order+1];               // char array for stream.read()
        char * pt = buffer;                 // pointer to buffer
        infile.read(pt,order);              // read order-number of characters from stream
        
        for (int i = 0; i < order; i++) {   // extract character array as string
            str += buffer[i];
        }
        
        char ch = infile.get();             // extract following char
        if (infile.fail()) break;
        
        debug(debugFlag, "The current key is: ", str);
        debug(debugFlag, "The next char is: ", ch);
        
        model[str] += ch;                   // add ch to the Vector at key str
        // if str does not exist, create a new entry
        for (int i = 0; i < order; i++) {
            infile.unget();                 // push back pointer to capture every group
        }
    }
}

/* Function: debug
 * Usage: debug(debugFlag, "The current key is: ", str)
 * ------
 * debug methods to follow execution of code
 */
void debug(bool flag, string str, string test) {
    if (flag)
        cout << str << test << endl;
}

void debug(bool flag, string str, char test) {
    if (flag)
        cout << str << test << endl;
}

/* Function: printAnalysis
 * Usage: printAnalysis(model)
 * -------
 * prints out the key and corresponding Vector
 */

void printAnalysis(Map<string, Vector<char> > & model) {
    foreach(string key in model) {
        Vector<char> vec = model[key];
        cout << key << setw(20) << vec.toString() << endl;
    }
}