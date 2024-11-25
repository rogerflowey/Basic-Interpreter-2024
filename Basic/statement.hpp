/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include <string>
#include <sstream>
#include "evalstate.hpp"
#include "exp.hpp"
#include "Utils/tokenScanner.hpp"
#include "program.hpp"
#include "parser.hpp"
#include "Utils/error.hpp"
#include "Utils/strlib.hpp"

class Program;

/*Statement
 * Class:
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
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

    virtual void execute(EvalState &state, Program &program) = 0;


    bool canDirectExecute;
};


/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

class REM_STMT : public Statement{

public:

    REM_STMT(TokenScanner &scanner);



    ~REM_STMT() override;



    void execute(EvalState &state, Program &program) override;


};

class LET_STMT : public Statement{

public:
    LET_STMT(TokenScanner &scanner);


    ~LET_STMT() override;


    void execute(EvalState &state, Program &program) override;



private:
    std::string var;
    Expression *exp;
};

class PRINT_STMT : public Statement{

public:
  PRINT_STMT(TokenScanner &scanner);


  ~PRINT_STMT() override;


  void execute(EvalState &state, Program &program) override;



private:
  Expression *exp;
};

class INPUT_STMT : public Statement{

public:
  INPUT_STMT(TokenScanner &scanner);


  ~INPUT_STMT() override;


  void execute(EvalState &state, Program &program) override;



private:
  std::string var;
};

class END_STMT : public Statement{

public:
  END_STMT(TokenScanner &scanner);


  ~END_STMT() override;


  void execute(EvalState &state, Program &program) override;



private:
};

class GOTO_STMT : public Statement{

public:
  GOTO_STMT(TokenScanner &scanner);


  ~GOTO_STMT() override;


  void execute(EvalState &state, Program &program) override;



private:
  int line_num;
};

class IF_STMT : public Statement{

public:
  IF_STMT(TokenScanner &scanner);


  ~IF_STMT() override;


  void execute(EvalState &state, Program &program) override;



private:
  Expression *lhs;
  std::string cmp;
  Expression *rhs;
  int line_num;
};
#endif
