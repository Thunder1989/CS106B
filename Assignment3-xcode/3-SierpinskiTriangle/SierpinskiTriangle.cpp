/*
 * File: SierpinskiTriangle.cpp
 * ----------------------------
 * Name: Darren Hau
 * Section: Lingtong
 * ------
 * This function prompts the user for an edge length and fractal order and draws the corresponding Sierpinski triangle.
 */

#include <iostream>
#include <cmath>
#include "console.h"
#include "gwindow.h"
#include "gmath.h"
#include "simpio.h"
using namespace std;

/* Function prototypes */
void drawSierpinski(GWindow & gw, double edgeLength, int order, GPoint pt);
void drawTriangle(GWindow & gw, double edgeLength, GPoint pt);

/* Main program */
int main() {
    GWindow gw(600, 500);
    cout << "Welcome to the Sierpinski Triangle." << endl;
    int edgeLength = getInteger("Enter an edge length: ");
    if (edgeLength <= 0) {
        cout << "The edge length must be greater than 0." << endl;
    }
    int fractalOrder = getInteger("Enter a fractal order: ");
    if (fractalOrder < 0) {
        cout << "The fractal order must be greater than or equal to 0." << endl;
    }
    GPoint pt = GPoint(100.0, 400.0);
    drawSierpinski(gw, edgeLength, fractalOrder, pt);
    return 0;
}

/* Function: drawSieirpinski
 * Usage: drawSierpinski(gw, edgeLength, fractalOrder, pt)
 * -----
 * This function draws a Sierpinski triangle given an edgeLength, order, and starting point.  If the edgeLength and order are invalid, it returns without drawing a triangle.  Otherwise, it takes the midpoints of the bottom and left edges of an equilateral triangle and uses them to generate Sierpinski triangles of edgeLength/2 and order-1 recursively.
 */
void drawSierpinski(GWindow & gw, double edgeLength, int order, GPoint pt) {
    if (edgeLength <= 0 || order < 0)
        return;
    if (order == 0) {
        drawTriangle(gw, edgeLength, pt);
    }
    else {
        drawSierpinski(gw, edgeLength/2, order - 1, pt);
        double x = pt.getX() + edgeLength/2;
        double y = pt.getY();
        GPoint pt1 = GPoint(x, y);
        drawSierpinski(gw, edgeLength/2, order - 1, pt1);
        x = pt.getX() + edgeLength/4;
        y = pt.getY() - edgeLength/4*sqrt(3);
        pt1 = GPoint(x,y);
        drawSierpinski(gw, edgeLength/2, order - 1, pt1);
    }
}

/* Function: drawTriangle
 * drawTriangle(gw, edgeLength, pt)
 * -------
 * This function draws an equilateral triangle with a starting point pt.
 */
void drawTriangle(GWindow & gw, double edgeLength, GPoint pt) {
    pt = gw.drawPolarLine(pt, edgeLength, 60.0);
    pt = gw.drawPolarLine(pt, edgeLength, -60.0);
    pt = gw.drawPolarLine(pt, edgeLength, 180.0);
}