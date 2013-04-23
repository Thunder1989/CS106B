/*
 * File: Pathfinder.cpp
 * --------------------
 * Name: Darren Hau
 * Section: Lingtong
 * ----
 * This program provides a graphical interface for the user to navigate a geographical region.  The user can:
 *      1. Choose among multiple maps
 *      2. Use Dijkstra's algorithm to find the shortest path between two locations
 *              a) the user can also start Dijkstra's algorithm by simply clicking on a starting node
 *      3. Go on a road trip and specify several destinations to visit, such that the program calculates the shortest
 *              route that visits each destination in order
 *      4. Use Kruskal's algorithm to find the minimum spanning tree that connects every location
 */

#include <iostream>
#include <cmath>
#include <string>
#include "console.h"
#include "filelib.h"
#include "gevents.h"
#include "ginteractors.h"
#include "gpathfinder.h"
#include "graphtypes.h"
#include "gwindow.h"
#include "pqueue.h"
#include "path.h"
#include "simpio.h"
using namespace std;

/* Constants */

const string DEFAULT_MAP = "USA.txt";
const string DEFAULT_IMAGE = "USA.png";
const int READ_NODES = 0;
const int READ_ARCS = 1;

/* Function prototypes */

void eventLoop(PathfinderGraph & graph);
void initButtons(GWindow & gw);
void welcome();
bool promptYesOrNo(string prompt);

void drawGraph(PathfinderGraph & graph, string node_color, string arc_color);
void readGraphData(PathfinderGraph & graph, ifstream & infile);
void initGraph(PathfinderGraph & graph);
string readImageName(ifstream & infile);
void readNode(string nodeData, PathfinderGraph & graph);
void readArc(string arcData, PathfinderGraph & graph);
void drawNodeSet(Set<Node *> & nodesToDraw, string color);
void drawArcSet(Set<Arc *> & arcsToDraw, string color);

void runDijkstra(PathfinderGraph & graph);
void runDijkstra(PathfinderGraph & graph, Node * first);
void runMultipleDijkstra(PathfinderGraph & graph);
Path findShortestPath(Node *start, Node *finish);
Node *findNodeAtGPoint(PathfinderGraph & graph, GPoint pt);
void displayPath(Path & path);

void runKruskal(PathfinderGraph & graph);
int findSetWithNode(Vector< Set<Node *> > & nodeSets, Node * n);

/* Main */

int main() {
    GWindow gw(WINDOW_WIDTH, WINDOW_HEIGHT);
    initPathfinderGraphics(gw);
    welcome();
    initButtons(gw);
    
    PathfinderGraph graph;
    initGraph(graph);           // initialize graph
    
    eventLoop(graph);
    return 0;
}

/***************************Kruskal*******************************/
/*****************************************************************/

/* Function: runKruskal
 * Usage: runKruskal(graph)
 * --
 * This function displays the shortest spanning tree according to Kruskal's algorithm.  It stores all arcs into a priority queue according to cost.  As it dequeues arcs, it checks for 4 situations:
 *      1. if neither node is in a set of connected nodes, it creates a new set with those nodes and adds it to a Vector of Sets of nodes.
 *      2. if only one node is in a set of connected nodes, it adds the other node to that set
 *      3. if both nodes are in different sets of connected nodes, it merges the two sets
 *      4. if both nodes are in the same set of connected nodes, it does nothing.
 */
void runKruskal(PathfinderGraph & graph) {
    PriorityQueue<Arc *> pqueue;
    Set<Arc *> originalArcs = graph.getArcSet();
    foreach(Arc * a in originalArcs) {                              // enqueue each Arc into a pqueue
        pqueue.enqueue(a, a->cost);
        cout << "Enqueuing with cost: " << a->cost << endl;
    }
    
    cout << "PQueue has size " << pqueue.size() << endl << endl;
    
    cout << "Finding minimum spanning tree using Kruskal..." << endl;
    cout << "Process edges in order of cost: " << endl;
    Set<Arc *> spanningArcs;                                        // set of arcs in spanning tree
    Vector< Set<Node *> > nodeSets;                                 // keeps track of sets of nodes
    while (!pqueue.isEmpty()) {
        Arc * a = pqueue.dequeue();
        cout << a->cost << ": " << a->start->name << " - " << a->finish->name << endl;
        
        int index1 = findSetWithNode(nodeSets, a->start);
        int index2 = findSetWithNode(nodeSets, a->finish);
        if (index1 == -1 && index2 == -1) {                         // if neither node is in a set
            Set<Node *> newSet;
            newSet += a->start, a->finish;
            nodeSets += newSet;
            spanningArcs.add(a);
        }
        else if (index1 == -1) {                                    // if only 1 node is not in a set
            nodeSets[index2] += a->start;
            spanningArcs.add(a);
        }
        else if (index2 == -1) {
            nodeSets[index1] += a->finish;
            spanningArcs.add(a);
        }
        else if (index1 != index2) {                                // if nodes are in different sets
            nodeSets[index1] = nodeSets[index1] + nodeSets[index2];
            nodeSets.remove(index2);                                // remove other set
            spanningArcs.add(a);
        }
    }
    
    drawArcSet(originalArcs, "lightgray");
    drawArcSet(spanningArcs, HIGHLIGHT_COLOR);
    drawNodeSet(nodeSets[0], HIGHLIGHT_COLOR);
}

/* Function: findSetWithNode
 * Usage: int index1 = findSetWithNode(nodeSets, a->start)
 * --
 * This function returns the index of the set which contains a node.  If the node is not found, it returns -1.
 */
int findSetWithNode(Vector< Set<Node *> > & nodeSets, Node * n) {
    for (int i = 0; i < nodeSets.size(); i++) {
        if (nodeSets[i].contains(n)) return i;
    }
    return -1;
}


/***************************Dijkstra*******************************/
/******************************************************************/

/* Function: runDijkstra
 * Usage: runDijkstra(graph), runDijkstra(graph, startNode)
 * ---
 * The first function prompts the user to click on the start and finish node.  It then finds the shortest Path between the two using Dijsktra's algorithm and highlights the Path.
 * The second function contains the start node provided by the user.  It prompts the user to click on the finish node, then executes the same algorithm as the first function.
 */
void runDijkstra(PathfinderGraph & graph) {
    Node *start;
    while (true) {                                          // waits for the user to click on a valid start Node
        cout << "Click the starting node." << endl;
        GPoint startPos = getMouseClick();
        start = findNodeAtGPoint(graph, startPos);
        if (start != NULL) break;
        cout << "Sorry, that's not a node." << endl;
    }
    drawPathfinderNode(start->position, HIGHLIGHT_COLOR);
    
    Node *finish;
    while (true) {                                          // waits for the user to click on a valid finish Node
        cout << "Click the destination node." << endl;
        GPoint finishPos = getMouseClick();
        finish = findNodeAtGPoint(graph, finishPos);
        if (finish == start) {                              // prevent user from clicking same start and finish node
            cout << "Please choose a different node." << endl;
        }
        else if (finish != NULL) break;
        cout << "Sorry, that's not a node." << endl;
    }
    drawPathfinderNode(finish->position, HIGHLIGHT_COLOR);
    
    Path path = findShortestPath(start, finish);
    
    drawGraph(graph, DIM_COLOR, DIM_COLOR);
    displayPath(path);
}

void runDijkstra(PathfinderGraph & graph, Node * first) {
    Node *start = first;
    drawPathfinderNode(start->position, HIGHLIGHT_COLOR);
    
    Node *finish;
    while (true) {                                          // waits for the user to click on a valid finish Node
        cout << "Click the destination node." << endl;
        GPoint finishPos = getMouseClick();
        finish = findNodeAtGPoint(graph, finishPos);
        if (finish == start) {                              // prevent user from clicking same start and finish node
            cout << "Please choose a different node." << endl;
        }
        else if (finish != NULL) break;
        cout << "Sorry, that's not a node." << endl;
    }
    drawPathfinderNode(finish->position, HIGHLIGHT_COLOR);
    
    Path path = findShortestPath(start, finish);
    
    drawGraph(graph, DIM_COLOR, DIM_COLOR);
    displayPath(path);
}

/* Function: runMultipleDijkstra
 * Usage: runMultipleDijkstra(graph)
 * ---
 * This function allows the user to specify multiple nodes to visit.  The function will then find the shortest path to each node according to the order in which the user clicked on the nodes and display the path the user should take on his/her fun road trip!
 */
void runMultipleDijkstra(PathfinderGraph & graph) {
    int numTotalNodes = (graph.getNodeSet()).size();
    Vector<Node *> nodesToVisit;                        // collect all nodes user wants to visit
    Node *prevNode = NULL;                              // records previous node so user doesn't enter same node twice in a row
    while (nodesToVisit.size() < numTotalNodes) {       // the user is limited to the number of nodes on the graph
        Node *nextNode;
        while (true) {
            cout << "Click the next stop." << endl;
            GPoint startPos = getMouseClick();
            nextNode = findNodeAtGPoint(graph, startPos);
            if (nextNode != NULL) break;
            cout << "Sorry, that's not a node." << endl;
        }
        if (prevNode == nextNode) {                     // if user clicks same node twice in a row
            cout << "Sorry, you just visited that node. Don't you want to go somewhere different?" << endl;
        }
        else {                                          // if valid next destination
            nodesToVisit += nextNode;
            drawPathfinderNode(nextNode->position, HIGHLIGHT_COLOR);
            prevNode = nextNode;
            bool done = promptYesOrNo("Is that your final destination?");
            if (done == true) break;
        }
    }
    
    Vector<Path> allPaths;
    int numStops = nodesToVisit.size();
    for (int i = 0; i < (numStops - 1); i++) {          // find and store each successive path in allPaths
        Path nextPath = findShortestPath(nodesToVisit[i], nodesToVisit[i+1]);
        allPaths += nextPath;
    }
    
    drawGraph(graph, DIM_COLOR, DIM_COLOR);             // grey out original nodes and arcs
    
    cout << endl << "Visit your tourist stops in this order!" << endl;
    foreach(Path p in allPaths) {                       // highlight each successive path
        displayPath(p);
        cout << p.toString() << endl;
    }
    cout << endl;
}

/* Function: displayPath
 * Usage: displayPath(path)
 * --
 * This function highlights each of the arcs in the given Path.
 */
void displayPath(Path & path) {
    Vector<Arc *> arcs = path.getArcs();
    drawPathfinderNode(arcs[0]->start->position, HIGHLIGHT_COLOR);                      // highlight first Node
    foreach (Arc * a in arcs) {
        drawPathfinderArc(a->start->position, a->finish->position, HIGHLIGHT_COLOR);    // highlight Arc
        drawPathfinderNode(a->finish->position, HIGHLIGHT_COLOR);                       // highlight finish Node
    }
}

/* Function: findNodeAtGPoint
 * Usage: finish = findNodeAtGPoint(graph, finishPos)
 * ----
 * This function returns a Node * if the user clicks on the graphical representation of the Node.  Therefore, the user can click up to NODE_RADIUS away from the actual coordinates and still hit the Node.
 */
Node *findNodeAtGPoint(PathfinderGraph & graph, GPoint pt) {
    Set<Node *> nodes = graph.getNodeSet();
    foreach (Node * n in nodes) {
        GPoint test = n->position;
        int xc = test.getX();
        int yc = test.getY();
        if (abs(pt.getX() - xc) <= NODE_RADIUS && abs(pt.getY() - yc) <= NODE_RADIUS) return n;
    }
    return NULL;
}

/* Function: findShortestPath
 * Usage: Path path = findShortestPath(start, finish)
 * ----
 * This function uses Dijsktra's algorithm to determine the shortest path.  It uses the Path class to keep track of the distance of a certain set of arcs.  This distance is used to enqueue the Path in a priority queue, such as the next Path dequeued is the shortest Path from start to finish.
 */
Path findShortestPath(Node *start, Node *finish) {
    Path path;
    PriorityQueue<Path> queue;
    Map<string,double> fixed;
    while (start != finish) {
        if (!fixed.containsKey(start->name)) {
            fixed.put(start->name, path.getDistance());
            cout << "Fix the distance to " << start->name << " at " << path.getDistance() << endl;
            cout << "Process the arcs out of " << start->name << endl;
            foreach (Arc *arc in start->arcs) {                             // look at each arc out of start node
                if (!fixed.containsKey(arc->finish->name)) {
                    path.add(arc);
                    queue.enqueue(path, path.getDistance());
                    cout << "Enqueue the path: " << start->name << " -> " << arc->finish->name << endl;
                    path.remove(path.size() - 1);                           // backtrack one step to get to the next arc
                }
            }
        }
        if (queue.isEmpty()) {
            path.clear();
            return path;
        }
        path = queue.dequeue();
        cout << "Dequeue the shortest path: " << path.toString() << endl;
        start = path.at(path.size() - 1)->finish;                           // set the next start node
    }
    cout << endl << "Shortest path: " << path.toString() << endl;
    return path;
}

/***************************Initializing Graph*******************************/
/****************************************************************************/

/* Function: drawGraph
 * Usage: drawGraph(graph, NODE_COLOR, ARC_COLOR)
 * -----
 * This function displays the nodes and arcs of an initialized graph.
 */
void drawGraph(PathfinderGraph & graph, string node_color, string arc_color) {
    Set<Node *> nodesToDraw = graph.getNodeSet();
    drawNodeSet(nodesToDraw, node_color);
    Set<Arc *> arcsToDraw = graph.getArcSet();
    drawArcSet(arcsToDraw, arc_color);
}

/* Function: drawArcSet
 * Usage: drawArcSet(arcsToDraw, arc_color)
 * ----
 * This function draws a Set of Arc pointers with the given color.
 */
void drawArcSet(Set<Arc *> & arcsToDraw, string color) {
    foreach(Arc * a in arcsToDraw) {
        Node *n1 = a->start;
        Node *n2 = a->finish;
        drawPathfinderArc(n1->position, n2->position, color);
    }
}

/* Function: drawNodeSet
 * Usage: drawNodeSet(nodesToDraw, node_color)
 * ---
 * This function draws a Set of Node pointers with the given color.
 */
void drawNodeSet(Set<Node *> & nodesToDraw, string color) {
    foreach(Node * n in nodesToDraw) {
        drawPathfinderNode(n->position, color, n->name);
    }
}

/* Function: readGraphData
 * Usage: readGraphData(graph, infile)
 * -----
 * This function interprets the data for a given graph file, which the user must provide.  It returns the image name, adds the nodes to the graph, and adds the arcs to the graph.  Finally, it displays the graph.
 */
void readGraphData(PathfinderGraph & graph, ifstream & infile) {
    graph.clear();                                  // clears the previous graph
    string nextLine;
    int mode = -1;                                  // there are 2 modes for interpreting the data: as nodes and as arcs
    
    string imageName = readImageName(infile);       // read image name first
    drawPathfinderMap(imageName);
    
    while (true) {
        getline(infile, nextLine);
        if (infile.fail()) break;                   // while not at end of file
        if (nextLine == "NODES") mode = READ_NODES;
        else if (nextLine == "ARCS") mode = READ_ARCS;
        else {
            switch (mode) {
                case READ_NODES: readNode(nextLine, graph);
                    break;
                case READ_ARCS: readArc(nextLine, graph);
                    break;
            }
        }
    }
    drawGraph(graph, NODE_COLOR, ARC_COLOR);
}

/* Function: initGraph
 * Usage: initGraph(graph)
 * ----
 * This function initializes the graph to the default map.
 */
void initGraph(PathfinderGraph & graph) {
    ifstream infile;
    openFile(infile, DEFAULT_MAP);
    readGraphData(graph, infile);
}

/* Function: readArc
 * Usage: readArc(nextLine, graph)
 * ----
 * This function interprets the given string as an arc and adds it to the graph.  It also places the arc into the arc Set of both nodes.
 */
void readArc(string arcData, PathfinderGraph & graph) {
    TokenScanner scanner(arcData);
    scanner.ignoreWhitespace();
    string s1 = scanner.nextToken();                    // scan first endpoint
    string s2 = scanner.nextToken();                    // scan second endpoint
    int cost = stringToInteger(scanner.nextToken());    // scan cost
    Arc *arc = graph.addArc(s1, s2);
    arc->cost = cost;
    arc = graph.addArc(s2, s1);
    arc->cost = cost;
    arc->start->arcs += arc;            // add arc into Set of arcs in start node.  We only care about arcs coming out of a node.
}

/* Function: readNode
 * Usage: readNode(nextLine, graph)
 * ----
 * This function interprets the given string as a node and adds it to the graph.
 */
void readNode(string nodeData, PathfinderGraph & graph) {
    TokenScanner scanner(nodeData);
    scanner.ignoreWhitespace();
    string name = scanner.nextToken();              // scan name
    int xc = stringToInteger(scanner.nextToken());  // scan x coordinate
    int yc = stringToInteger(scanner.nextToken());  // scan y coordinate
    GPoint pos = GPoint(xc, yc);
    Node *node = graph.addNode(name);
    node->position = pos;
}

/* Function: readImageName
 * Usage: string imageName = readImageName(infile)
 * ---
 * This function returns the first line (name of image) of a text file.
 */
string readImageName(ifstream & infile) {
    string result;
    getline(infile, result);
    return result;
}

/* Function: initButtons
 * Usage: initButtons(gw)
 * ---
 * This function draws 4 buttons in the GWindow.
 */
void initButtons(GWindow & gw) {
    gw.addToRegion(new GButton("Map"), "SOUTH");            // init buttons
    gw.addToRegion(new GButton("Dijkstra"), "SOUTH");
    gw.addToRegion(new GButton("Road Trip"), "SOUTH");
    gw.addToRegion(new GButton("Kruskal"), "SOUTH");
    gw.addToRegion(new GButton("Quit"), "SOUTH");
}

/* Function: welcome
 * Usage: welcome()
 * ---
 * This function displays a welcome message.
 */
void welcome() {
    cout << "Welcome to Google Maps Beta!" << endl;
    cout << "This is our first foray into making long-distance " << endl;
    cout << "vacations and road trips more pleasant.  If you " << endl;
    cout << "ever got fed up folding and unfolding unwieldy " << endl;
    cout << "paper maps, here is your solution.  All you need " << endl;
    cout << "is your laptop.  Happy traveling!" << endl;
}

/***************************Events*******************************/
/****************************************************************/

/*
 * Function: eventLoop
 * Usage: eventLoop();
 * -------------------
 * Waits for events to occur and then executes the appopriate action.
 * Note that this function never returns.  The program exits only when
 * a command executes the exitGraphics function.
 */

void eventLoop(PathfinderGraph & graph) {
   while (true) {
       GEvent e = waitForEvent(ACTION_EVENT + CLICK_EVENT);
       drawGraph(graph, NODE_COLOR, ARC_COLOR);                     // redraw graph when a new button is clicked
       if (e.getEventClass() == ACTION_EVENT) {
           string cmd = ((GActionEvent) e).getActionCommand();
           if (cmd == "Quit") exitGraphics();
           else if (cmd == "Map") {
               ifstream infile;
               promptUserForFile(infile, "Enter your file: ");
               readGraphData(graph, infile);
           }
           else if (cmd == "Dijkstra") runDijkstra(graph);
           else if (cmd == "Road Trip") runMultipleDijkstra(graph);
           else if (cmd == "Kruskal") runKruskal(graph);
       }
       else {
           double x = ((GMouseEvent) e).getX();
           double y = ((GMouseEvent) e).getY();
           GPoint pos = GPoint(x,y);
           Node * startNode = findNodeAtGPoint(graph, pos);
           if (startNode != NULL) {
               runDijkstra(graph, startNode);
           }
       }
   }
}

/* Function: promptYesOrNo
 * Usage: bool customChoice = promptYesOrNo("Do you want to configure a custom board? ")
 * -----
 * This function prompts the user to enter a yes or a no.  It returns true if yes and false if no.  Otherwise, it re-prompts the user.
 */
bool promptYesOrNo(string prompt) {
    while (true) {
        string response = getLine(prompt);
        if (response == "yes" || response == "YES" || response == "y" || response == "Y") return true;
        if (response == "no" || response == "NO" || response == "n" || response == "N") return false;
    }
}