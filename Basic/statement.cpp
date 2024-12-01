/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"


/* Implementation of the Statement class */

Statement::Statement() = default;

Statement::~Statement() = default;

//todo

REM_STMT::REM_STMT(TokenScanner &scanner) {
  canDirectExecute=false;
  while (scanner.hasMoreTokens()) {
    scanner.nextToken();
  }
};

REM_STMT::~REM_STMT() = default;


void REM_STMT::execute(EvalState &state, Program &program) {
  return;
};



LET_STMT::LET_STMT(TokenScanner &scanner) {
    std::string temp=scanner.nextToken();
    if(isVarName(temp)) {
       var=temp;
    } else {
      error("SYNTAX ERROR5");
    }
    scanner.verifyToken("=");
    exp = readE(scanner);
    canDirectExecute=true;
};

LET_STMT::~LET_STMT() {
  delete exp;
}

void LET_STMT::execute(EvalState &state, Program &program) {
  state.setValue(var,exp->eval(state));
}

PRINT_STMT::PRINT_STMT(TokenScanner &scanner) {
  exp = readE(scanner);
  canDirectExecute=true;
}

PRINT_STMT::~PRINT_STMT() {
  delete exp;
}

void PRINT_STMT::execute(EvalState &state, Program &program) {
  std::cout<<exp->eval(state)<<std::endl;
}

INPUT_STMT::INPUT_STMT(TokenScanner &scanner) {
  std::string temp=scanner.nextToken();
  if(isVarName(temp)) {
    var=temp;
  } else {
    error("SYNTAX ERROR");
  }
  canDirectExecute=true;
}

INPUT_STMT::~INPUT_STMT() = default;

void INPUT_STMT::execute(EvalState &state, Program &program) {
  while(true) {
    std::cout<<" ? ";
    std::string input;
    getline(std::cin, input);
    if(isNum(input)) {
      state.setValue(var,strToInt(input));
      break;
    }
    if(isNumNeg(input)) {
      state.setValue(var,-strToInt(input.substr(1,input.size()-1)));
      break;
    }
    std::cout<<"INVALID NUMBER"<<std::endl;
    if(std::cin.eof()) {
      exit(0);
    }
  }
}


  END_STMT::END_STMT(TokenScanner &scanner) {
    canDirectExecute=false;
  };


  END_STMT::~END_STMT() {

  };


  void END_STMT::execute(EvalState &state, Program &program) {
    program.end();
  };

  GOTO_STMT::GOTO_STMT(TokenScanner &scanner) {
    std::string token=scanner.nextToken();
    if(isNum(token)) {
      line_num=strToInt(token);
    } else {
      error("SYNTAX ERROR6");
    }
    canDirectExecute=false;
  }

  GOTO_STMT::~GOTO_STMT() = default;

  void GOTO_STMT::execute(EvalState &state, Program &program) {
    program.gotoLine(line_num);
  }

  IF_STMT::IF_STMT(TokenScanner &scanner) {
    lhs=readE(scanner);
    cmp=scanner.nextToken();
    if(!(cmp=="<"||cmp==">"||cmp=="=")) {
      error("SYNTAX ERROR4");
    }
    rhs=readE(scanner);
    scanner.verifyToken("THEN");
    std::string token = scanner.nextToken();
    if(isNum(token)){
      line_num=strToInt(token);
    }
    canDirectExecute=false;
  }

  IF_STMT::~IF_STMT(){
    delete lhs;
    delete rhs;
  }

  void IF_STMT::execute(EvalState &state, Program &program) {
    int lvalue=lhs->eval(state);
    int rvalue=rhs->eval(state);
    if(cmp=="<") {
      if(lvalue<rvalue) {
        program.gotoLine(line_num);
      }
    }
    if(cmp=="=") {
      if(lvalue==rvalue) {
        program.gotoLine(line_num);
      }
    }
    if(cmp==">") {
      if(lvalue>rvalue) {
        program.gotoLine(line_num);
      }
    }
  };










