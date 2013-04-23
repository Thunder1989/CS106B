/*
 * File: Boggle.cpp
 * ----------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the main starter file for Assignment #4, Boggle.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include "gboggle.h"
#include "grid.h"
#include "gwindow.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
using namespace std;

/* Constants */

const int BOGGLE_WINDOW_WIDTH = 650;
const int BOGGLE_WINDOW_HEIGHT = 550;           // Adjusted to support Big Boggle

const string STANDARD_CUBES[16]  = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

const string BIG_BOGGLE_CUBES[25]  = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

/* Function prototypes */
void welcome();
void giveInstructions();
bool promptYesOrNo(string prompt);
char promptForChar(string prompt);
void initRandomBoard(int dim, Grid<char> & boggleChars);
void initCustomBoard(int dim, Grid<char> & boggleChars);
void setRandomLetter(int n, string letterChoices, int dim, Grid<char> & boggleChars);
void turnHuman(const Lexicon & lex, Grid<char> & boggleChars, int dim, Lexicon & prevWords);
bool searchValidBoardWord(string str, Grid<char> & boggleChars, int dim);
bool validBoardWord(string str, int row, int col, int dim, Grid<char> & boggleChars, Grid<bool> & charsFlagged);
void toggleHighlight(Grid<bool> & charsFlagged, bool flag);
void turnComputer(const Lexicon & lex, Grid<char> & boggleChars, int dim, Lexicon & prevWords);
void searchForWord(string str, int row, int col, const Lexicon & lex, Lexicon & prevWords, Grid<char> & boggleChars, Grid<bool> & charsFlagged, int dim);

/*************** Debug **************/
const bool DEBUGFLAG = true;

void debug(char ch, int row, int col, bool flag) {
    cout << "DEBUG: turnComputer is searching from " << ch << " at (" << row << "," << col << ")." << endl;
}

void initRiggedBoard(Grid<char> & boggleChars) {
    labelCube(0, 0, 'A');
    boggleChars[0][0] = 'A';
    labelCube(0, 1, 'P');
    boggleChars[0][1] = 'P';
    labelCube(0, 2, 'P');
    boggleChars[0][2] = 'P';
    labelCube(0, 3, 'L');
    boggleChars[0][3] = 'L';
    labelCube(1, 0, 'B');
    boggleChars[1][0] = 'B';
    labelCube(1, 1, 'U');
    boggleChars[1][1] = 'U';
    labelCube(1, 2, 'G');
    boggleChars[1][2] = 'G';
    labelCube(1, 3, 'E');
    boggleChars[1][3] = 'E';
    labelCube(2, 0, 'A');
    boggleChars[2][0] = 'A';
    labelCube(2, 1, 'C');
    boggleChars[2][1] = 'C';
    labelCube(2, 2, 'S');
    boggleChars[2][2] = 'S';
    labelCube(2, 3, 'S');
    boggleChars[2][3] = 'S';
    labelCube(3, 0, 'T');
    boggleChars[3][0] = 'T';
    labelCube(3, 1, 'S');
    boggleChars[3][1] = 'S';
    labelCube(3, 2, 'I');
    boggleChars[3][2] = 'I';
    labelCube(3, 3, 'E');
    boggleChars[3][3] = 'E';
}

/* Main program */

int main() {
    GWindow gw(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
    initGBoggle(gw);
    welcome();
    
    cout << "Please answer yes/no." << endl << endl;
    bool needInstructions = promptYesOrNo("Do you need instructions? ");
    if (needInstructions)
        giveInstructions();
    Lexicon english("EnglishWords.dat");
    
    while (true) {
        int dimensions = 0;
        bool bigBoggleChoice = promptYesOrNo("Do you want to play Big Boggle? ");
        if (bigBoggleChoice == true)
            dimensions = 5;
        else
            dimensions = 4;
        drawBoard(dimensions, dimensions);                  // graphic board showing chars placed
        Grid<char> boggleChars(dimensions, dimensions);     // grid containing chars placed
        
        bool customChoice = promptYesOrNo("Do you want to configure a custom board? ");
        if (customChoice == true)
            initCustomBoard(dimensions, boggleChars);
        else
            initRandomBoard(dimensions, boggleChars);
        
        Lexicon prevWords;                                  // list of previous words shared between Human and Computer players
        turnHuman(english, boggleChars, dimensions, prevWords);
        cout << endl;
        turnComputer(english, boggleChars, dimensions, prevWords);
        
        bool playAgain = promptYesOrNo("Do you want to play again? ");
        if (playAgain == false) break;
    }
    
    exitGraphics();
    return 0;
}

/********************************COMPUTER TURN*********************************/
/******************************************************************************/

/* Function: turnComputer
 * Usage: turnComputer(english, boggleChars, dimensions, prevWords)
 * -----
 * This function is the computer player's turn. It iterates through each label on the Boggle board as a source for words.  It calls searchForWord to build and test for valid words that the human player missed.
 */
void turnComputer(const Lexicon & lex, Grid<char> & boggleChars, int dim, Lexicon & prevWords) {
    cout << "Now it's my turn.  I'll show you all the words you missed." << endl;
    Grid<bool> charsFlagged(dim, dim);                  // Grid to keep track of traversed labels
    
    for (int row = 0; row < boggleChars.numRows(); row++) {
        for (int col = 0; col < boggleChars.numCols(); col++) {
            string str = "";
            str += boggleChars[row][col];
            searchForWord(str, row, col, lex, prevWords, boggleChars, charsFlagged, dim);
        }
    }
}

/* Function: searchForWord
 * Usage: searchForWord(str, row, col, lex, prevWords, boggleChars, charsFlagged, dim)
 * -----
 * This function builds and tests for valid words by comparing a string str to a Lexicon lex.  If str is a new word with at least 4 letters, it updates the word list and score.  If str is a prefix, the function continues to build strings to test by adding the character in the labels around the current label.
 */
void searchForWord(string str, int row, int col, const Lexicon & lex, Lexicon & prevWords, Grid<char> & boggleChars, Grid<bool> & charsFlagged, int dim) {
    if (lex.contains(str) && !prevWords.contains(str) && str.size() >= 4) {        // success case
        recordWordForPlayer(str, COMPUTER);
        prevWords.add(str);
    }
    if (lex.containsPrefix(str)) {
        charsFlagged[row][col] = true;
        for (int nextRow = row-1; nextRow <= row+1; nextRow++) {
            for (int nextCol = col-1; nextCol <= col+1; nextCol++) {
                if (0 <= nextRow && nextRow < dim && 0 <= nextCol && nextCol < dim && charsFlagged[nextRow][nextCol] == false)
                    searchForWord(str + boggleChars[nextRow][nextCol], nextRow, nextCol, lex, prevWords, boggleChars, charsFlagged, dim);
            }
        }
    }
    charsFlagged[row][col] = false;
}

/********************************HUMAN TURN*********************************/
/***************************************************************************/

/* Function: turnHuman
 * Usage: turnHuman(english, boggleChars, dimensions, prevWords)
 * -----
 * This function is the human player's turn.  It allows the player to enter as many words as desired.  If the word is a valid word (in the lexicon, not a previous word, has at least 4 letters, and can be constructed on the Boggle board) then the function updates the word list and score.
 */
void turnHuman(const Lexicon & lex, Grid<char> & boggleChars, int dim, Lexicon & prevWords) {
    cout << "I'll let you go first!" << endl;
    
    while (true) {
        string newWord = getLine("Enter next word: ");
        newWord = toUpperCase(newWord);                 // convert to upper case for comparison
        if (newWord == "") break;
        if (!lex.contains(newWord))
            cout << "That's not a real word!" << endl;
        else if (prevWords.contains(newWord))
            cout << "Sorry, you've used that word already." << endl;
        else if (newWord.size() < 4)
            cout << "I'm afraid that's not a valid word.  Make sure you enter a word with more than 4 letters." << endl;
        else if (!searchValidBoardWord(newWord, boggleChars, dim))
            cout << "Sorry, this word isn't on the board." << endl;
        else {                                          // valid word
            recordWordForPlayer(newWord, HUMAN);        // update word list and score
            prevWords.add(newWord);
        }
    }
}

/* Function: searchValidBoardWord
 * Usage: else if (!searchValidBoardWord(newWord, boggleChars, dim))
 * ----
 * A wrapper function for validBoardWord that searches for the first label that contains the first letter of a given word.  If the word can be constructed on the Boggle board, it highlights the labels for a period and then returns true.
 */
bool searchValidBoardWord(string str, Grid<char> & boggleChars, int dim) {
    Grid<bool> charsFlagged(dim, dim);                  // Grid to keep track of labels to highlight (also traversed labels)
    
    for (int row = 0; row < dim; row++) {
        for (int col = 0; col < dim; col++) {
            if (boggleChars[row][col] == str[0]) {      // if label = first char
                bool isValid = validBoardWord(str, row, col, dim, boggleChars, charsFlagged);
                if (isValid) {
                    toggleHighlight(charsFlagged, true);    // highlight
                    pause(1000.0);
                    toggleHighlight(charsFlagged, false);   // unhighlight
                    return true;
                }
            }
        }
    }
    return false;
}

/* Function: toggleHighlight
 * Usage: toggleHighlight(charsFlagged, true)
 * ----
 * This function iterates through every location in the Grid charsFlagged and highlights the corresponding label if the location is marked true.
 */
void toggleHighlight(Grid<bool> & charsFlagged, bool flag) {
    for (int row = 0; row < charsFlagged.numRows(); row++) {
        for (int col = 0; col < charsFlagged.numCols(); col++) {
            if (charsFlagged[row][col] == true)
                highlightCube(row, col, flag);
        }
    }
}

/* Function: validBoardWord
 * Usage: isValid = validBoardWord(str.substr(1), row-1, col-1, dim, boggleChars, charsFlagged)
 * -----
 * This function  checks if the given label contains the first letter of the given string.  If so, it calls itself recursively on the surrounding labels and the string minus its first letter.  If the passed substring is empty, that means a given word can be constructed on the Boggle board and validBoardWord returns true.  The labels that compose that word are flagged so searchValidBoardWord can highlight them, and the flags are used during recursion to avoid reusing labels.
 */
bool validBoardWord(string str, int row, int col, int dim, Grid<char> & boggleChars, Grid<bool> & charsFlagged) {
    if (str == "") return true;                                         // success case
    if (row < 0 || col < 0 || row >= dim || col >= dim) return false;      // if label out of bounds
    if (charsFlagged[row][col] == true) return false;                   // if label already used
    
    bool isValid = false;
    if (boggleChars[row][col] == str[0]) {              // if label = first char
        charsFlagged[row][col] = true;                  // tentatively flag label to avoid reuse
        isValid = validBoardWord(str.substr(1), row-1, col-1, dim, boggleChars, charsFlagged) || validBoardWord(str.substr(1), row-1, col, dim, boggleChars, charsFlagged) || validBoardWord(str.substr(1), row-1, col+1, dim, boggleChars, charsFlagged) || validBoardWord(str.substr(1), row, col-1, dim, boggleChars, charsFlagged) || validBoardWord(str.substr(1), row, col+1, dim, boggleChars, charsFlagged) || validBoardWord(str.substr(1), row+1, col-1, dim, boggleChars, charsFlagged) || validBoardWord(str.substr(1), row+1, col, dim, boggleChars, charsFlagged) || validBoardWord(str.substr(1), row+1, col+1, dim, boggleChars, charsFlagged);
    }
    if (!isValid)
        charsFlagged[row][col] = false;                 // unflag label if not valid
    return isValid;
}

/********************************INITIALIZING BOARD*********************************/
/***********************************************************************************/

/* Function: initRandomBoard
 * Usage: initRandomBoard(4, boggleChars)
 * -----
 * This function shuffles a Vector of string cubes, then chooses a random letter from each cube's string to set on the board.  It records the chars placed in Grid boggleChars.
 */
void initRandomBoard(int dim, Grid<char> & boggleChars) {
    Vector<string> cubes;
    for (int i = 0; i < (dim*dim); i++) {       // copy array into Vector
        if (dim == 4)
            cubes += STANDARD_CUBES[i];
        if (dim == 5)
            cubes += BIG_BOGGLE_CUBES[i];
    }
    for (int i = 0; i < cubes.size(); i++) {    // shuffle the cubes
        int swapIndex = randomInteger(i, cubes.size() - 1);
        string temp = cubes[swapIndex];
        cubes[swapIndex] = cubes[i];
        cubes[i] = temp;
    }
    for (int i = 0; i < (dim*dim); i++) {       // sets the board
        setRandomLetter(i, cubes[i], dim, boggleChars);
    }
}

/* Function: initCustomBoard
 * Usage: initCustomBoard(4, boggleChars)
 * -----
 * This function allows the user to input letters for each position.  It records the chars placed in Grid boggleChars.
 */
void initCustomBoard(int dim, Grid<char> & boggleChars) {
    for (int i = 0; i < (dim*dim); i++) {
        char ch = promptForChar("Please enter the next letter: ");
        int row = i / dim;
        int col = i % dim;
        labelCube(row, col, ch);
        boggleChars[row][col] = ch;
    }
}

/* Function: setRandomLetter
 * Usage: setRandomLetter(i, cubes[i], dim, boggleChars)
 * -------
 * Given an index n and dimension dim, this function computes the corresponding row and column on the board.  Then, it chooses a random letter in letterChoices and sets it in the proper place.  It records that character in the proper place in Grid boggleChars.
 */
void setRandomLetter(int n, string letterChoices, int dim, Grid<char> & boggleChars) {
    int row = n / dim;
    int col = n % dim;
    int rand = randomInteger(0, letterChoices.size() - 1);
    labelCube(row, col, letterChoices[rand]);
    boggleChars[row][col] = letterChoices[rand];
}

/********************************OTHER***********************************/
/************************************************************************/

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

/* Function: promptForChar
 * Usage: char ch = promptForChar("Please enter the next letter: ")
 * -----
 * This function prompts the user to enter a letter.  If the first char entered is a letter, it returns that letter.  Otherwise, it re-prompts the user.
 */
char promptForChar(string prompt) {
    while (true) {
        string entry = getLine(prompt);
        entry = toUpperCase(entry);         // capitalize for aesthetic consistency
        if (entry.size() > 1)
            cout << "I'll just take your first character!" << endl;
        char ch = entry[0];
        if ('A' <= ch && ch <= 'Z') {      // ensure that user enters valid letter
            return ch;
        }
    }
}

/********************************WELCOME*********************************/
/************************************************************************/

/*
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */

void welcome() {
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

/*
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */

void giveInstructions() {
    cout << endl;
    cout << "The boggle board is a grid onto which ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl << endl;
    cout << "Hit return when you're ready...";
    getLine();
}