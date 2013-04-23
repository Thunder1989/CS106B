/*
 * File: Basic.cpp
 * ---------------
 * Name: Darren Hau
 * Section: Lingtong
 * ---------
 * This class implements a BASIC interpreter.  The user can type in program lines
 * and statements and execute them.  For more information, type HELP in the console.
 */

#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "error.h"
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "tokenscanner.h"
#include "simpio.h"
#include "strlib.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program & program, EvaluationContext & context);
void runProgram(Program & program, EvaluationContext & context);
void listProgram(Program & program, TokenScanner & scanner);
void displayHelp();
void specificHelp(TokenScanner & scanner);

/* Main program */

int main() {
   EvaluationContext context;
   Program program;
   cout << "Basic BASIC Interpreter" << endl << endl;
   while (true) {
      try {
         processLine(getLine(), program, context);
      } catch (ErrorException & ex) {
          cerr << "Error: " << ex.getMessage() << endl;
          cout << "Error: " << ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, context);
 * -------------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program & program, EvaluationContext & context) {
   TokenScanner scanner;
   scanner.ignoreWhitespace();
   scanner.setInput(line);
    
    string firstToken = toUpperCase(scanner.nextToken());
    if (firstToken == "RUN") {
        runProgram(program, context);
    }
    else if (firstToken == "LIST") {
        listProgram(program, scanner);
    }
    else if (firstToken == "CLEAR") {
        program.clear();
    }
    else if (firstToken == "HELP") {
        if (!scanner.hasMoreTokens()) {
            displayHelp();
        }
        else {
            specificHelp(scanner);
        }
    }
    else if (firstToken == "QUIT") {
        exit(0);
    }
    else {
        int lineNumber = stringToInteger(firstToken);
        if (!scanner.hasMoreTokens()) {
            program.removeSourceLine(lineNumber);
        }
        else {
            Statement *stmt = parseStatement(scanner);
            program.addSourceLine(lineNumber, line);
            program.setParsedStatement(lineNumber, stmt);
        }
    }
}

/*
 * Function: runProgram
 * Usage: runProgram(program, context);
 * -------------------------------------------
 * Runs the program starting from the first line number.
 */
void runProgram(Program & program, EvaluationContext & context) {
    context.setCurrentLine(program.getFirstLineNumber());
    int programLine = context.getCurrentLine();
    while (programLine != -1) {
        Statement *stmt = program.getParsedStatement(programLine);
        context.setCurrentLine(program.getNextLineNumber(programLine)); // updates to line + 1
        stmt->execute(context);                         // unless controlled by IF, GOTO, or END
        programLine = context.getCurrentLine();
    }
}

/*
 * Function: listProgram
 * Usage: listProgram(program, scanner);
 * -------------------------------------------
 * Displays the lines of the program.  It calls either version of program.toString()
 * depending on whether the user specifies line numbers.
 */
void listProgram(Program & program, TokenScanner & scanner) {
    if (!scanner.hasMoreTokens()) {
        program.toString();
    }
    else {
        int startLine = stringToInteger(scanner.nextToken());
        scanner.nextToken();
        int endLine = stringToInteger(scanner.nextToken());
        program.toString(startLine, endLine);
    }
}

/*
 * Function: displayHelp
 * Usage: displayHelp();
 * ---------------------
 * Displays a helpful message.
 */
void displayHelp() {
    cout << "Welcome to basic BASIC!" << endl;
    cout << "-----------------------" << endl;
    cout << "This will be an introduction to programming." << endl;
    cout << "To create a program, type in a line number" << endl;
    cout << "followed by a statement.  For instance:" << endl << endl;
    
    cout << "10 LET x = 1" << endl << endl;
    
    cout << "There are several statements available for you to use:" << endl;
    cout << "REM, LET, PRINT, INPUT, GOTO, IF, and END are all" << endl;
    cout << "part of a program, and therefore must follow line numbers." << endl << endl;
    
    cout << "There are also several commands available for you to use:" << endl;
    cout << "RUN, LIST, CLEAR, HELP, and QUIT control the BASIC interpreter" << endl;
    cout << "and should be used without line numbers.  You must have figured" << endl;
    cout << "HELP out already, unless you became desperate." << endl << endl;
    
    cout << "For more detailed help, type HELP followed by a command or statement." << endl;
    cout << "For example, you can type HELP EXTENSIONS to find a list of all the" << endl;
    cout << "extensions implemented." << endl;
}

/*
 * Function: specificHelp
 * Usage: specificHelp(scanner);
 * ---------------------
 * Displays a specialized help message depending on user input.
 */
void specificHelp(TokenScanner & scanner) {
    string specificHelp = toUpperCase(scanner.nextToken());
    if (specificHelp == "REM") {
        cout << "Help for REM" << endl;
        cout << "------------" << endl;
        cout << "Use REM to comment your code.  You can type in anything" << endl;
        cout << "after REM and it will be ignored.  Example:" << endl;
        cout << "10 REM This is an example of commenting" << endl;
    }
    else if (specificHelp == "LET") {
        cout << "Help for LET" << endl;
        cout << "------------" << endl;
        cout << "Use LET to set a variable to an expression.  You can declare" << endl;
        cout << "a new variable on the left side of the equals sign, but remember" << endl;
        cout << "to only use constants or variables you have already initialized" << endl;
        cout << "on the right side of the equals sign.  If you use doubles, make" << endl;
        cout << "sure to include a number before the decimal point.  Example:" << endl;
        cout << "10 LET x = y + 2.1" << endl << endl;
        cout << "You can also create a LET statement without the LET keyword.  Example:" << endl;
        cout << "10 x = y + 2.1" << endl;
    }
    else if (specificHelp == "PRINT") {
        cout << "Help for PRINT" << endl;
        cout << "------------" << endl;
        cout << "Use PRINT to print a value.  You can print strings and expressions" << endl;
        cout << "separated by commas.  Example:" << endl;
        cout << "10 PRINT \"Countdown in T minus \", time" << endl;
    }
    else if (specificHelp == "INPUT") {
        cout << "Help for INPUT" << endl;
        cout << "------------" << endl;
        cout << "Use INPUT to prompt the user for values.  The program will prompt" << endl;
        cout << "the user with ' ? ' and the variable following INPUT will be set" << endl;
        cout << "to the value that the user enters.  Example:" << endl;
        cout << "10 INPUT x" << endl;
    }
    else if (specificHelp == "GOTO") {
        cout << "Help for GOTO" << endl;
        cout << "------------" << endl;
        cout << "Use GOTO to jump to a line in the program.  This is commonly used" << endl;
        cout << "for loops.  Example:" << endl;
        cout << "10 GOTO 5" << endl;
    }
    else if (specificHelp == "IF") {
        cout << "Help for IF" << endl;
        cout << "------------" << endl;
        cout << "Use IF to create a conditional statement.  If the condition is" << endl;
        cout << "satisfied, the program will jump to the line specified after THEN." << endl;
        cout << "A condition consists of an expression followed by an operator followed" << endl;
        cout << "by an expression.  Valid operators are <, =, and >.  Example:" << endl;
        cout << "10 IF x + 1 = y THEN 30" << endl;
    }
    else if (specificHelp == "END") {
        cout << "Help for END" << endl;
        cout << "------------" << endl;
        cout << "Use END to mark the end of the program.  Example:" << endl;
        cout << "10 END" << endl;
    }
    else if (specificHelp == "RUN") {
        cout << "Help for RUN" << endl;
        cout << "------------" << endl;
        cout << "Type RUN to run a program." << endl;
    }
    else if (specificHelp == "LIST") {
        cout << "Help for LIST" << endl;
        cout << "------------" << endl;
        cout << "Type LIST to list all the lines in a program." << endl;
        cout << "Type LIST 3-5 to list lines 3 to 5 inclusive." << endl;
    }
    else if (specificHelp == "CLEAR") {
        cout << "Help for CLEAR" << endl;
        cout << "------------" << endl;
        cout << "Type CLEAR to delete all the lines in a program." << endl;
    }
    else if (specificHelp == "HELP") {
        cout << "Help for HELP" << endl;
        cout << "------------" << endl;
        cout << "What are you doing here?  I thought you already knew" << endl;
        cout << "how to ask for help!" << endl;
    }
    else if (specificHelp == "QUIT") {
        cout << "Help for QUIT" << endl;
        cout << "------------" << endl;
        cout << "Type QUIT to exit this BASIC interpreter." << endl;
    }
    else if (specificHelp == "EXTENSIONS") {
        cout << "Help for EXTENSIONS" << endl;
        cout << "------------" << endl;
        cout << "Basic BASIC has actually become quite awesome.  Here are" << endl;
        cout << "a list of extensions for this particular interpreter:" << endl << endl;
        cout << "Optional LET:" << endl;
        cout << "-------------" << endl;
        cout << "You do not need to type the LET keyword to create a LetStmt." << endl;
        cout << "Typing \"10 x = 3\" will assign x to 3." << endl << endl;
        cout << "Floating-point Numbers:" << endl;
        cout << "-------------" << endl;
        cout << "Expressions can use doubles instead of integers." << endl;
        cout << "Typing \"10 x = 3.14\" will assign x to 3.14." << endl << endl;
        cout << "Flexible PRINT:" << endl;
        cout << "-------------" << endl;
        cout << "Print can print string tokens as well." << endl;
        cout << "Typing \"10 PRINT \"Countdown in T minus \", time\" is valid." << endl << endl;
        cout << "Selective LIST:" << endl;
        cout << "-------------" << endl;
        cout << "You can choose to list selective lines." << endl;
        cout << "Typing LIST 30-50 will display lines 30 through 50 inclusive." << endl << endl;
    }
}