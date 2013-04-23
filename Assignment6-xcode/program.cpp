/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
#include "parser.h"
#include "strlib.h"
using namespace std;

Program::Program() {
    start = new Cell;
    start->link = NULL;
}

Program::~Program() {
    delete start;
}

/* Function: clear
 * Usage: 
 * ---
 * Deletes all cells except for the dummy cell such that Program appears to be reinitialized.
 */
void Program::clear() {
    Cell *cursor = start->link;
    while (cursor != NULL) {
        map.remove(cursor->lineNumber);
        Cell *next = cursor->link;
        delete cursor->parsedLine;
        delete cursor;
        cursor = next;
    }
    start->link = NULL;
}

/* Function: addSourceLine
 * Usage: 
 * ---
 * Inserts a new Cell in the proper place according to lineNumber.  Sets parsedStatement to NULL.
 */
void Program::addSourceLine(int lineNumber, string line) {
    int maxSmaller = -1;                        // records previous line number
    bool existingLine = false;
    Cell * cursor = start->link;
    while (cursor != NULL) {
        if (cursor->lineNumber == lineNumber) { // if line already exists
            cursor->sourceLine = line;
            delete cursor->parsedLine;          // delete parsed line
            cursor->parsedLine = NULL;
            existingLine = true;
            break;
        }
        else if ((cursor->lineNumber < lineNumber) && (cursor->lineNumber > maxSmaller)) {
            maxSmaller = cursor->lineNumber;
        }
        cursor = cursor->link;
    }
    
    if (!existingLine) {                        // if line does not already exist
        Cell *nextCell = new Cell;
        nextCell->lineNumber = lineNumber;
        nextCell->sourceLine = line;
        nextCell->parsedLine = NULL;
        
        if (map.size() == 0) {                  // if this is the first line
            start->link = nextCell;
            nextCell->link = NULL;
            map.put(lineNumber, nextCell);
        }
        else if (maxSmaller == -1) {            // if there is no smaller line number
            nextCell->link = start->link;       // but there are other lines
            start->link = nextCell;
            map.put(lineNumber, nextCell);
        }
        else {                                  // if there is a smaller line number
            Cell *prev = map.get(maxSmaller);
            nextCell->link = prev->link;
            prev->link = nextCell;
            map.put(lineNumber, nextCell);
        }
    }
}

void Program::removeSourceLine(int lineNumber) {
    if (map.containsKey(lineNumber)) {
        map.remove(lineNumber);
        
        Cell *cursor = start;                   // delete from linked list
        Cell *cp = cursor->link;
        while (cp->lineNumber != lineNumber) {
            cursor = cp;
            cp = cursor->link;
        }
        cursor->link = cp->link;
        delete cp->parsedLine;
        delete cp;
    }
}

string Program::getSourceLine(int lineNumber) {
    if (map.containsKey(lineNumber)) {
        Cell *cursor = map.get(lineNumber);
        return cursor->sourceLine;
    }
    return "";
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    if (!map.containsKey(lineNumber)) {
        error("setParsedStatement: line number " + realToString(lineNumber) + " does not exist");
    }
    
    Cell *cursor = map.get(lineNumber);
    if (cursor->parsedLine != NULL) {
        delete cursor->parsedLine;
    }
    cursor->parsedLine = stmt;
}

Statement *Program::getParsedStatement(int lineNumber) {
    Cell *cursor = map.get(lineNumber);
    if (cursor->parsedLine != NULL) {
        return cursor->parsedLine;
    }
    return NULL;
}

int Program::getFirstLineNumber() {
    Cell *cursor = start->link;
    if (cursor != NULL) {
        return cursor->lineNumber;
    }
    return -1;
}

int Program::getNextLineNumber(int lineNumber) {
    Cell *prev = map.get(lineNumber);
    Cell *cursor = prev->link;
    if (cursor != NULL) {
        return cursor->lineNumber;
    }
    return -1;
}

void Program::toString() {
    Cell *cursor = start->link;
    while (cursor != NULL) {
        cout << cursor->sourceLine << endl;
        cursor = cursor->link;
    }
}

void Program::toString(int startLine, int endLine) {
    if (!map.containsKey(startLine)) {
        error("Program::toString(): The program does not contain line " + integerToString(startLine));
    }
    if (!map.containsKey(endLine)) {
        error("Program::toString(): The program does not contain line " + integerToString(endLine));
    }
    Cell *cursor = start->link;
    while (cursor->lineNumber != startLine) {       // move to startLine
        cursor = cursor->link;
    }
    cout << cursor->sourceLine << endl;
    while ((cursor->lineNumber != endLine) && cursor != NULL) {
        cursor = cursor->link;
        cout << cursor->sourceLine << endl;
    }
}