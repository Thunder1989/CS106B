/*
 * File: path.cpp
 * --------------
 * Name: Darren Hau
 * Section: Lingtong
 * This file implements the path.h interface.
 */

#include <iostream>
#include "path.h"
#include "graphtypes.h"
#include "vector.h"
#include "simpio.h"
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////
// The Path class encapsulates a Vector of arcs like the original Dijsktra's algorithm,
// but keeps track of the path distance as arcs are added to return the distance in
// constant time.
// The Path class consists of 2 instance variables: int distance, and Vector<Arc *> arcs
// The methods in the Path class are all required in Dijsktra's algorithm.  They are:
//      1. constructor / destructor
//      2. add
//      3. remove
//      4. clear
//      5. size
//      6. getDistance
//      7. at
//      8. getArcs
//      9. toString
//////////////////////////////////////////////////////////////////////////////////////

/* Constructor: Path
 */
Path::Path() {
    distance = 0;
}
    
/* Destructor: ~Path
 */
Path::~Path() {
    
}
    
/* Method: add
 * Usage: path.add(arc)
 * ---
 * Adds the given Arc pointer to the Path.
 */
void Path::add(Arc * value){
    distance += value->cost;
    arcs.add(value);
}

/* Method: remove
 * Usage: path.remove(path.size() - 1)
 * ---
 * Removes the Arc pointer at the given index.
 */
void Path::remove(int n){
    distance -= arcs[n]->cost;
    arcs.remove(n);
}

/* Method: clear
 * Usage: path.clear()
 * ---
 * Clears the Vector and resets distance to 0.
 */
void Path::clear(){
    distance = 0;
    arcs.clear();
}

/* Method: size
 * Usage: path.size()
 * ----
 * Returns the size of the Vector.
 */
int Path::size(){
    return arcs.size();
}
    
/* Method: getDistance
 * Usage: path.getDistance()
 * ----
 * Returns the total distance of traversing all the Arcs in the path.
 */
double Path::getDistance(){
    return distance;
}

/* Method: at
 * Usage: start = path.at(path.size() - 1)->finish
 * ----
 * Returns the Arc pointer at the given index.
 */
Arc * Path::at(int n){
    return arcs[n];
}

/* Method: getArcs
 * Usage: Vector<Arc *> arcs = path.getArcs()
 * ----
 * Returns the Vector of Arc pointers in the path.
 */
Vector<Arc *> Path::getArcs() {
    return arcs;
}
    
/* Method: toString
 * Usage: cout << "Shortest path: " << path.toString() << endl
 * ---
 * Prints out each element in the path separated by " -> ".
 */
string Path::toString(){
    string result = "";
    result += arcs[0]->start->name;
    foreach (Arc * a in arcs) {
        result += " -> " + a->finish->name;
    }
    result += " (" + integerToString(distance) + ")";
    return result;
}