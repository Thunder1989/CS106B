/*
 * File: StockCutting.cpp
 * ----------------------
 * Name: Darren Hau
 * Section: Lingtong
 * --------
 * This function prompts the user to either enter requests manually or generate random requests as a test sequence.  It then prints out the number of pipes required to satisfy the requests generated.
 */

#include <iostream>
#include "console.h"
#include "vector.h"
#include "simpio.h"
#include "random.h"
using namespace std;

/* Function prototypes */
int cutStock(Vector<int> & requests, int stockLength);
int fillPipe(Vector<int> & requests, int stockLength, int remainder, int numPipesUsed);
void mergeSort(Vector<int> & vec);
void merge(Vector<int> & vec, Vector<int> & v1, Vector<int> & v2);

/* Main program */
int main() {
    Vector<int> requests;
    int stockLength;
    int numRequests;
    
    int select = getInteger("Do you want to (1) enter your requests manually or (2) generate random requests? ");
    stockLength = getInteger("Enter stock length: ");
    numRequests = getInteger("Enter number of requests: ");
    
    if (select == 1) {
        for (int i = 0; i < numRequests; i++) {
            int n = getInteger("Next request: ");
            if (n > stockLength) {
                cout << "You cannot request a size larger than the stock length." << endl;
                break;
            }
            requests += n;
        }
    }
    else if (select == 2) {
        cout << "Requests are: " << endl;
        for (int i = 0; i < numRequests; i++) {
            int n = randomInteger(1, stockLength);
            requests += n;
            cout << n << ", ";
        }
        cout << endl;
    }
    
    int x = cutStock(requests, stockLength);
    cout << "You used " << x << " pipes." << endl;
   return 0;
}

/*
 * Function: cutStock
 * Usage: int units = cutStock(requests, stockLength);
 * ---------------------------------------------------
 * Computes the minimum number of stock pipes required to satisfy
 * the vector of requests of individual pieces.
 * This function first sorts the requests in descending order then uses the new Vector in function fillPipes.
 */

int cutStock(Vector<int> & requests, int stockLength) {
    mergeSort(requests);
    
    int numPipesUsed = 1;
    return fillPipe(requests, stockLength, stockLength, numPipesUsed);
}

/* Function: fillPipe
 * Usage: int x = fillPipe(requests, stockLength, stockLength, numPipesUsed)
 * ------
 * This function returns the number of pipes used to satisfy a Vector of requests sorted in descending order.  In the base case, there are no more requests and the function returns the number of pipes used up to that point.  If requests remain, the function iterates through each request to determine if it is less than the remainder of stock and therefore can be satisfied by the remainder.  If it can, the function removes the cut from the Vector of requests and calls itself recursively with an updated requests and remainder.  If the function has iterated through all the requests without finding one that is satisfied by the remainder, it calls itself recursively with a fresh stockLength and increments the number of pipes used.
 */
int fillPipe(Vector<int> & requests, int stockLength, int remainder, int numPipesUsed) {
    if (requests.isEmpty()) {
        return numPipesUsed;
    }
    for (int i = 0; i < requests.size(); i++) {
        int nextCut = requests[i];
        if (nextCut <= remainder) {
            requests.remove(i);
            return fillPipe(requests, stockLength, remainder-nextCut, numPipesUsed);
        }
    }
    return fillPipe(requests, stockLength, stockLength, ++numPipesUsed);
}

/* 
 * Function: mergeSort
 * ------
 * Uses the divide-and-conquer strategy recursively.  Divides a vector into two smaller vectors until you reach a vector of size 0 or 1, which is already sorted.  Then merges the two vectors using the merge function.  Complexity is O(Nlog(N)).
 */
void mergeSort(Vector<int> & vec) {
    int n = vec.size();
    if (n <= 1) return;
    Vector<int> v1;
    Vector<int> v2;
    for (int i = 0; i < n; i++) {
        if (i < n/2)
            v1.add(vec[i]);
        else
            v2.add(vec[i]);
    }
    mergeSort(v1);
    mergeSort(v2);
    vec.clear();
    merge(vec, v1, v2);
}

/* 
 * Function: merge
 * ------
 * Merges two smaller vectors by comparing elements from each vector and adding the smaller element into the larger vector.
 */
void merge(Vector<int> & vec, Vector<int> & v1, Vector<int> & v2) {
    int n1 = v1.size();
    int n2 = v2.size();
    int p1 = 0;
    int p2 = 0;
    while (p1 < n1 && p2 < n2) {
        if (v1[p1] > v2[p2])
            vec.add(v1[p1++]);
        else
            vec.add(v2[p2++]);
    }
    while (p1 < n1) vec.add(v1[p1++]);
    while (p2 < n2) vec.add(v2[p2++]);
}