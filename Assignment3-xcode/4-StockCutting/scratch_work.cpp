/*
 * File: StockCutting.cpp
 * ----------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter code for the stock-cutting problem
 * fron Assignment #3.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include "console.h"
#include "vector.h"
#include "queue.h"
using namespace std;

const bool DEBUGFLAG = 1;

/* Function prototypes */

int cutStock(Vector<int> & requests, int stockLength);
void fillPipe(Vector<int> & requests, int stockLength, int goal, Vector<int> & currentPipe, Vector<Vector<int> > & pipes, Vector<Vector<Vector<int> > > batches);
void debug(bool flag, string message, int test);

/* Main program */

int main() {
    cout << "Hello." << endl;
    Vector<int> requests;
    requests += 1, 3, 6, 3;
    int stockLength = 10;
    
    int x = cutStock(requests, stockLength);
    return 0;
}

/*
 * Function: cutStock
 * Usage: int units = cutStock(requests, stockLength);
 * ---------------------------------------------------
 * Computes the minimum number of stock pipes required to satisfy
 * the vector of requests of individual pieces.
 */

int cutStock(Vector<int> & requests, int stockLength) {
    Vector<Vector<Vector<int> > > batches;
    Vector<Vector<int> > pipes;
    Vector<int> currentPipe;
    
    fillPipe(requests, stockLength, stockLength, currentPipe, pipes, batches);
    
    /*   for (int i = 0; i < pipes.size(); i++) {
     Vector<int> currentPipe = pipes[i];
     cout << currentPipe.toString() << endl;
     }*/
    return 0;
}

void fillPipe(Vector<int> & requests, int stockLength, int goal, Vector<int> & currentPipe, Vector<Vector<int> > & pipes, Vector<Vector<Vector<int> > > batches) {
    if (stockLength < 0) {
        return;
    }
    if (stockLength == 0 || requests.isEmpty()) {                         // if goal filled, add to pipes
        pipes.add(currentPipe);
        //     cout << currentPipe.toString() << endl;
        if (!requests.isEmpty()) {
            currentPipe.clear();
            fillPipe(requests, goal, goal, currentPipe, pipes, batches);
        }
        batches.add(pipes);
        pipes.clear();
        return;
    }
    else {                                          // try to fill current goal
        debug(DEBUGFLAG, "The size of requests is ", requests.size());
        for (int i = 0; i < requests.size(); i++) {
            int cutChoice = requests[i];
            Vector<int> updatedRequests = requests;         // remove from requests
            updatedRequests.remove(i);
            debug(DEBUGFLAG, "The size of updatedRequests is ", updatedRequests.size());
            currentPipe.add(cutChoice);                     // add to current pipe
            cout << "The current pipe has " << currentPipe.toString() << endl;
            fillPipe(updatedRequests, stockLength - cutChoice, goal, currentPipe, pipes, batches);
        }
    }
    if (goal > 1) {                         // if cannot fill current goal, decrement goal
        goal--;
        debug(DEBUGFLAG, "Decrementing goal to ", goal);
        stockLength = goal;
        fillPipe(requests, stockLength, goal, currentPipe, pipes, batches);
    }
}

/* Function: debug
 * Usage: debug(debugFlag, "The current key is: ", str)
 * ------
 * debug methods to follow execution of code
 */
void debug(bool flag, string message, int test) {
    if (flag)
        cout << message << test << endl;
}