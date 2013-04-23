/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In the finished
 * version, this file will also specify subclasses for each of the
 * statement types.  As you design your own version of this class,
 * you should pay careful attention to the exp.h interface specified
 * in Chapter 19, which is an excellent model for the Statement class
 * hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include "exp.h"

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

   Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

   virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(context);
 * ------------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvaluationContext object for looking up variables or
 * controlling the operation of the interpreter.
 */

   virtual void execute(EvaluationContext & context) = 0;

};

/*
 * Class: PrintStmt
 */

class PrintStmt : public Statement {
public:
    PrintStmt(TokenScanner & scanner);
    virtual ~PrintStmt();
    
    /* Method: execute
     * ------
     * This method prints out the expression
     */
    virtual void execute(EvaluationContext & context);
private:
    Vector<std::string> strings;
    Vector<Expression *> expressions;
};

/*
 * Class: LetStmt
 */

class LetStmt : public Statement {
public:
    LetStmt(TokenScanner & scanner);
    virtual ~LetStmt();
    
    /* Method: execute
     * ------
     * This method assigns the variable to the value given by exp->eval
     */
    virtual void execute(EvaluationContext & context);
private:
    std::string variable;
    double value;
    std::string op;
    Expression *exp;
};

/*
 * Class: RemStmt
 */

class RemStmt : public Statement {
public:
    RemStmt(TokenScanner & scanner);
    virtual ~RemStmt();
    virtual void execute(EvaluationContext & context);
private:
};

/*
 * Class: InputStmt
 */

class InputStmt : public Statement {
public:
    InputStmt(TokenScanner & scanner);
    virtual ~InputStmt();
    
    /* Method: execute
     * -----
     * This method prompts the user for an double value with " ? " and stores the input in variable
     */
    virtual void execute(EvaluationContext & context);
private:
    std::string variable;
    double value;
};

/*
 * Class: GotoStmt
 */

class GotoStmt : public Statement {
public:
    GotoStmt(TokenScanner & scanner);
    virtual ~GotoStmt();
    
    /* Method: execute
     * -----
     * This method sets currentLine in context = the line in the expression
     */
    virtual void execute(EvaluationContext & context);
private:
    int lineDest;
};

/*
 * Class: IfStmt
 */

class IfStmt : public Statement {
public:
    IfStmt(TokenScanner & scanner);
    virtual ~IfStmt();
    
    /* Method: execute
     * -----
     * This method sets currentLine in context = the line number after THEN if exp1 op exp2 is satisfied.
     */
    virtual void execute(EvaluationContext & context);
private:
    Expression *exp1;
    std::string op;
    Expression *exp2;
    int lineDest;
};

/*
 * Class: EndStmt
 */

class EndStmt : public Statement {
public:
    EndStmt(TokenScanner & scanner);
    virtual ~EndStmt();
    
    /* Method: execute
     * -----
     * This method sets currentLine in context = NULL
     */
    virtual void execute(EvaluationContext & context);
private:
    
};

#endif
