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

Expression *readE(TokenScanner &scanner, int prec,bool with_neg) {
    Expression *exp = readT(scanner,with_neg);
    std::string token;
    while (true) {
        token = scanner.nextToken();
        int newPrec = precedence(token);
        if (newPrec <= prec) break;
        Expression *rhs = readE(scanner, newPrec,false);
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

Expression *readT(TokenScanner &scanner,bool with_neg) {
    std::string token = scanner.nextToken();
    TokenType type = scanner.getTokenType(token);
    if (isNum(token)) return new ConstantExp(strToInt(token));
    if (isWord(token)) return new IdentifierExp(token);
    if (token == "-" && with_neg){
      token=scanner.nextToken();
      if(isNum(token)) {
        return new ConstantExp(-strToInt(token));
      } else {
        error("SYNTAX ERROR11");
      }
    };
    if (token != "(") error("SYNTAX ERROR7");
    Expression *exp = readE(scanner,0,true);
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
  if(token.empty()) {
    return false;
  }
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
    return isNum(token.substr(1,token.size()-1));
  } else {
    return isNum(token);
  }
}

bool isVarName(std::string token) {
  if(token.empty()) {
    return false;
  }
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
  if(token.empty()) {
    return false;
  }
  for(auto &c:token) {
    if(!isalnum(c)) {
      return false;
    }
  }
  return true;
}

int strToInt(std::string token) {
  if(token.empty()) {
    assert(false);
  }
  int out=0;
  for(auto &c:token) {
    out*=10;
    out+=c-'0';
  }
  return out;
}