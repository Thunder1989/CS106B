/*
 * File: parser.cpp
 * ----------------
 * This file implements the parser.h interface.
 */

#include <iostream>
#include <string>
#include "error.h"
#include "exp.h"
#include "parser.h"
#include "strlib.h"
#include "tokenscanner.h"
using namespace std;

/*
 * Implementation notes: parseExp
 * ------------------------------
 * This code just reads an expression and then checks for extra tokens.
 */

Expression *parseExp(TokenScanner & scanner) {
   Expression *exp = readE(scanner, 0);
   if (scanner.hasMoreTokens()) {
      error("Unexpected token \"" + scanner.nextToken() + "\"");
   }
   return exp;
}

/*
 * Implementation notes: parsedStatement
 * --------------------------------
 * This function checks the token against each of the defined Statement subclasses
 * and returns the corresponding Statement.
 */

Statement *parseStatement(TokenScanner & scanner) {
    string token = scanner.nextToken();
    string tokenUpper = toUpperCase(token);
    if (tokenUpper == "REM") return new RemStmt(scanner);
    if (tokenUpper == "LET") return new LetStmt(scanner);
    if (tokenUpper == "PRINT") return new PrintStmt(scanner);
    if (tokenUpper == "INPUT") return new InputStmt(scanner);
    if (tokenUpper == "GOTO") return new GotoStmt(scanner);
    if (tokenUpper == "IF") return new IfStmt(scanner);
    if (tokenUpper == "END") return new EndStmt(scanner);
    else {
        scanner.saveToken(token);
        return new LetStmt(scanner);
    }
    error("Unrecognized statement: " + token);
    return NULL;
}

/*
 * Implementation notes: readE
 * Usage: exp = readE(scanner, prec);
 * ----------------------------------
 * The implementation of readE uses precedence to resolve the ambiguity in
 * the grammar.  At each level, the parser reads operators and subexpressions
 * until it finds an operator whose precedence is greater than the prevailing
 * one.  When a higher-precedence operator is found, readE calls itself
 * recursively to read that subexpression as a unit.
 */

Expression *readE(TokenScanner & scanner, int prec) {
   Expression *exp = readT(scanner);
   string token;
   while (true) {
      token = scanner.nextToken();
      int tprec = precedence(token);
      if (tprec <= prec) break;
      Expression *rhs = readE(scanner, tprec);
      exp = new CompoundExp(token, exp, rhs);
   }
   scanner.saveToken(token);
   return exp;
}

/*
 * Implementation notes: readT
 * ---------------------------
 * This function scans a term, which is either an double, an identifier,
 * or a parenthesized subexpression.
 */

Expression *readT(TokenScanner & scanner) {
    scanner.scanNumbers();
   string token = scanner.nextToken();
   TokenType type = scanner.getTokenType(token);
   if (type == WORD) return new IdentifierExp(token);
    if (type == NUMBER) return new ConstantExp(stringToReal(token));
   if (token != "(") error("Unexpected token \"" + token + "\"");
   Expression *exp = readE(scanner, 0);
   if (scanner.nextToken() != ")") {
      error("Unbalanced parentheses");
   }
   return exp;
}

/*
 * Implementation notes: precedence
 * --------------------------------
 * This function checks the token against each of the defined operators
 * and returns the appropriate precedence value.
 */

int precedence(string token) {
   //if (token == "=") return 1;
   if (token == "+" || token == "-") return 2;
   if (token == "*" || token == "/") return 3;
   return 0;
}
