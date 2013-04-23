/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "parser.h"
#include "simpio.h"

/* Implementation of the Statement class */

Statement::Statement() {
   /* Empty */
}

Statement::~Statement() {
   /* Empty */
}

/*************PrintStmt Subclass*************/
/********************************************/

PrintStmt::PrintStmt(TokenScanner & scanner) {
    scanner.scanStrings();
    while (scanner.hasMoreTokens()) {
        std::string str = "";
        std::string next = "";
        while (scanner.hasMoreTokens()) {
            next = scanner.nextToken();
            if (next != ",") {
                str += next;
            }
            else break;
        }
        if (str[0] == '"') {                        // if nextToken represents a string
            strings.add(str);                       // add it to the Vector<string>
        }
        else {                                      // if nextToken represents an expression
            strings.add("");                        // mark it as an expression
            TokenScanner subScanner(str);
            Expression *exp = readE(subScanner, 0);
            expressions.add(exp);                   // and add it to the Vector<Expression *>
        }
    }
}

PrintStmt::~PrintStmt() {
    foreach(Expression *exp in expressions) {
        delete exp;
    }
}

void PrintStmt::execute(EvaluationContext & context) {
    int count = 0;
    foreach(string str in strings) {
        if (str == "") {                            // if the next token is an expression, print exp
            cout << expressions[count]->eval(context);
            count++;
        }
        else {                                      // otherwise, print the string
            cout << str.substr(1, str.size() - 2);  // without the quotation marks
        }
    }
    cout << endl;
}

/*************LetStmt Subclass*************/
/******************************************/

LetStmt::LetStmt(TokenScanner & scanner) {
    variable = scanner.nextToken();
    op = scanner.nextToken();
    if (op != "=") {
        error("LetStmt: operator is not =");
    }
    value = NULL;
    exp = readE(scanner, 0);
    if (scanner.hasMoreTokens()) {
        error("LetStmt: Extraneous token " + scanner.nextToken());
    }
}

LetStmt::~LetStmt() {
    delete exp;
}

void LetStmt::execute(EvaluationContext & context) {
    value = exp->eval(context);
    context.setValue(variable, value);
}

/*************RemStmt Subclass*************/
/******************************************/

RemStmt::RemStmt(TokenScanner & scanner) {
    // nothing needs to be assigned
}

RemStmt::~RemStmt() {
    // nothing needs to be deleted
}

void RemStmt::execute(EvaluationContext & context) {
    // nothing needs to be executed
}

/*************InputStmt Subclass*************/
/********************************************/

InputStmt::InputStmt(TokenScanner & scanner) {
    variable = scanner.nextToken();
    value = NULL;
    if (scanner.hasMoreTokens()) {
        error("InputStmt: Extraneous token " + scanner.nextToken());
    }
}

InputStmt::~InputStmt() {
    // nothing needs to be deleted
}

void InputStmt::execute(EvaluationContext & context) {
    value = getReal(" ? ");
    context.setValue(variable, value);
}

/*************GotoStmt Subclass*************/
/*******************************************/

GotoStmt::GotoStmt(TokenScanner & scanner) {
    lineDest = stringToInteger(scanner.nextToken());
    if (scanner.hasMoreTokens()) {
        error("GotoStmt: Extraneous token " + scanner.nextToken());
    }
}

GotoStmt::~GotoStmt() {
    // nothing needs to be deleted
}

void GotoStmt::execute(EvaluationContext & context) {
    context.setCurrentLine(lineDest);
}

/*************IfStmt Subclass*************/
/*****************************************/

IfStmt::IfStmt(TokenScanner & scanner) {
    std::string str1 = "";                  // pull out first expression
    std::string next = scanner.nextToken();
    while (next != "<" && next != "=" && next != ">") {
        str1 += next;
        next = scanner.nextToken();
    }
    TokenScanner subScanner(str1);
    exp1 = readE(subScanner, 0);
    
    op = next;                              // set operator
    
    std::string str2 = "";                  // pull out second expression
    next = scanner.nextToken();
    while (toUpperCase(next) != "THEN") {
        str2 += next;
        next = scanner.nextToken();
    }
    subScanner.setInput(str2);
    exp2 = readE(subScanner, 0);
    
    lineDest = stringToInteger(scanner.nextToken());
    if (scanner.hasMoreTokens()) {
        error("IfStmt: Extraneous token " + scanner.nextToken());
    }
}

IfStmt::~IfStmt() {
    delete exp1;
    delete exp2;
}

void IfStmt::execute(EvaluationContext & context) {
    bool conditionSatisfied = ((op == "=") && exp1->eval(context) == exp2->eval(context)) || ((op == "<") && exp1->eval(context) < exp2->eval(context)) || ((op == ">") && exp1->eval(context) > exp2->eval(context));
    if (conditionSatisfied) {
        context.setCurrentLine(lineDest);
    }
}

/*************EndStmt Subclass*************/
/*****************************************/

EndStmt::EndStmt(TokenScanner & scanner) {
    
}

EndStmt::~EndStmt() {
    
}

void EndStmt::execute(EvaluationContext & context) {
    context.setCurrentLine(-1);
}