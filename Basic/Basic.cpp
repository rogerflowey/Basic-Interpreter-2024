/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"


/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

/* Main program */

int main() {

    //freopen("testpoint.txt","r",stdin);

    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if(input.empty()) {
                continue;
            }
            processLine(input, program, state);
            if(std::cin.eof()) {
                error("QUIT");
            }
        } catch (ErrorException &ex) {
            if(ex.getMessage().at(0)=='S'&&ex.getMessage().at(1)=='Y') {
                std::cout<<"SYNTAX ERROR"<<std::endl;
            } else {
                std::cout << ex.getMessage() << std::endl;
            }
            if(std::cin.eof() || ex.getMessage()=="QUIT") {
                program.clear();
                exit(0);
            }
        }

    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.setInput(line);
    scanner.addWordCharacters("0123456789");


    //todo
    int line_num=-1;
    Statement *statement=nullptr;
    std::string token=scanner.nextToken();
    //std::cout<<token<<scanner.getTokenType(token)<<std::endl;
    if(isNum(token)) {
        line_num=stringToInteger(token);
        if(!scanner.hasMoreTokens()) {
            program.removeSourceLine(line_num);
            return;
        }
        token=scanner.nextToken();
    }
    if(scanner.getTokenType(token)==WORD) {
        //control commands
        if(line_num==-1) {
            if(token=="RUN") {
                if(scanner.hasMoreTokens()) {
                    error("SYNTAX ERROR2");
                }
                program.run(state);
                return;
            } else if(token=="LIST") {
                if(scanner.hasMoreTokens()) {
                    error("SYNTAX ERROR2");
                }
                program.list();
                return;
            } else if(token=="CLEAR") {
                if(scanner.hasMoreTokens()) {
                    error("SYNTAX ERROR2");
                }
                program.clear();
                state.Clear();
                return;
            } else if(token=="QUIT") {
                if(scanner.hasMoreTokens()) {
                    error("SYNTAX ERROR2");
                }
                error("QUIT");
                return;
            } else if(token=="HELP") {
                if(scanner.hasMoreTokens()) {
                    error("SYNTAX ERROR2");
                }
                std::cout<<"I don't know what can I help you,really"<<std::endl;
                return;
            }
        }

        //statements

        if(token=="REM") {
            statement = new REM_STMT(scanner);
        } else if(token=="LET") {
            statement = new LET_STMT(scanner);
        }  else if(token=="PRINT") {
            statement = new PRINT_STMT(scanner);
        }   else if(token=="INPUT") {
            statement = new INPUT_STMT(scanner);
        }   else if(token=="END") {
            statement = new END_STMT(scanner);
        }   else if(token=="GOTO") {
            statement = new GOTO_STMT(scanner);
        }   else if(token=="IF") {
            statement = new IF_STMT(scanner);
        }   else {
            error("SYNTAX ERROR1");
            return;
        }
        if(scanner.hasMoreTokens()) {
            delete statement;
            error("SYNTAX ERROR2");
        }
        if(line_num==-1) {
            if(statement->canDirectExecute) {
                try {
                    statement->execute(state,program);
                    delete statement;
                    statement=nullptr;
                } catch (ErrorException &ex) {
                    delete statement;
                    if(ex.getMessage().at(0)=='S'&&ex.getMessage().at(1)=='Y') {
                        std::cout<<"SYNTAX ERROR"<<std::endl;
                    } else {
                        std::cout << ex.getMessage() << std::endl;
                    }
                    if(std::cin.eof() || ex.getMessage()=="QUIT") {
                        program.clear();
                        exit(0);
                    }
                }
            } else {
                delete statement;
                error("SYNTAX ERROR3");
            }
        } else {
            program.addSourceLine(line_num,line,statement);
        }

    } else {
        error("SYNTAX ERROR");
    }



}

