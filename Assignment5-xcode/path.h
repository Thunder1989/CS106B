/*
 * File: path.h
 * ------------
 * Name: Darren Hau
 * Section: Lingtong
 * This file is the interface for a Path class that encapsulates
 * the idea of a multistep path.
 */

#ifndef _path_h
#define _path_h

#include "graphtypes.h"
#include "vector.h"

class Path {
public:
    
    /* Constructor: Path
     */
    Path();
    
    /* Destructor: ~Path
     */
    ~Path();
    
    /* Method: add
     * Usage: path.add(arc)
     * ---
     * Adds the given Arc pointer to the Path.
     */
    void add(Arc * value);
    
    /* Method: remove
     * Usage: path.remove(path.size() - 1)
     * ---
     * Removes the Arc pointer at the given index.
     */
    void remove(int n);
    
    /* Method: clear
     * Usage: path.clear()
     * ---
     * Clears the Vector and resets distance to 0.
     */
    void clear();
    
    /* Method: size
     * Usage: path.size()
     * ----
     * Returns the size of the Vector.
     */
    int size();
    
    /* Method: getDistance
     * Usage: path.getDistance()
     * ----
     * Returns the total distance of traversing all the Arcs in the path.
     */
    double getDistance();
    
    /* Method: at
     * Usage: start = path.at(path.size() - 1)->finish
     * ----
     * Returns the Arc pointer at the given index.
     */
    Arc * at(int n);
    
    /* Method: getArcs
     * Usage: Vector<Arc *> arcs = path.getArcs()
     * ----
     * Returns the Vector of Arc pointers in the path.
     */
    Vector<Arc *> getArcs();
    
    /* Method: toString
     * Usage: cout << "Shortest path: " << path.toString() << endl
     * ---
     * Prints out each element in the path separated by " -> ".
     */
    string toString();
    
private:
    
    /* Instance variables */
    int distance;
    Vector<Arc *> arcs;
    
};

#endif
