/*
 * File: parser.cpp
 * ----------------
 * Implements the parser.h interface.
 */

#include "parser.hpp"


/*
 * Implementation notes: parseExp
 * ------------------------------
 * This code just reads an expression and then checks for extra tokens.
 */

Expression *parseExp(TokenScanner &scanner) {
    Expression *exp = readE(scanner);
    if (scanner.hasMoreTokens()) {
        error("parseExp: Found extra token: " + scanner.nextToken());
    }
    return exp;
}

/*
 * Implementation notes: readE
 * Usage: exp = readE(scanner, prec);
 * ----------------------------------
 * This version of readE uses precedence to resolve the ambiguity in
 * the grammar.  At each recursive level, the parser reads operators and
 * subexpressions until it finds an operator whose precedence is greater
 * than the prevailing one.  When a higher-precedence operator is found,
 * readE calls itself recursively to read in that subexpression as a unit.
 */

Expression *readE(TokenScanner &scanner, int prec) {
    Expression *exp = readT(scanner);
    std::string token;
    while (true) {
        token = scanner.nextToken();
        int newPrec = precedence(token);
        if (newPrec <= prec) break;
        Expression *rhs = readE(scanner, newPrec);
        exp = new CompoundExp(token, exp, rhs);
    }
    scanner.saveToken(token);
    return exp;
}

/*
 * Implementation notes: readT
 * ---------------------------
 * This function scans a term, which is either an integer, an identifier,
 * or a parenthesized subexpression.
 */

Expression *readT(TokenScanner &scanner) {
    std::string token = scanner.nextToken();
    TokenType type = scanner.getTokenType(token);
    if (isNum(token)) return new ConstantExp(stringToInteger(token));
    if (isWord(token)) return new IdentifierExp(token);
    if (token == "-") return new CompoundExp(token, new ConstantExp(0), readT(scanner));
    if (token != "(") error("SYNTAX ERROR7");
    Expression *exp = readE(scanner);
    if (scanner.nextToken() != ")") {
        error("SYNTAX ERROR8"  );
    }
    return exp;
}

/*
 * Implementation notes: precedence
 * --------------------------------
 * This function checks the token against each of the defined operators
 * and returns the appropriate precedence value.
 */

int precedence(std::string token) {
    if (token == "+" || token == "-") return 2;
    if (token == "*" || token == "/") return 3;
    return 0;
}


bool isNum(std::string token) {
  for(auto &c:token) {
    if(!(c>='0'&&c<='9')) {
      return false;
    }
  }
  return true;
}

bool isNumNeg(std::string token) {
  if(token.empty()) {
    return false;
  }
  if(token.at(0)=='-') {
    return isNum(token.substr(1,token.size()));
  } else {
    return isNum(token);
  }
}

bool isVarName(std::string token) {
  for(auto &c:token) {
    if(!isalnum(c)) {
      return false;
    }
  }
  const static std::string reserved[]={"LET","REM","PRINT","INPUT","END","GOTO","IF","THEN","RUN","LIST","CLEAR","QUIT","HELP"};
  for(auto &k:reserved) {
    if(token.find(k)!=std::string::npos) {
      return false;
    }
  }
  return true;
}

bool isWord(std::string token) {
  for(auto &c:token) {
    if(!isalnum(c)) {
      return false;
    }
  }
  return true;
}