/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"



Program::Program() = default;

Program::~Program() = default;

void Program::clear() {
    // Replace this stub with your own code
    //todo
    while (!lines.empty()) {
        removeSourceLine(lines.begin()->first);
    }
}

void Program::addSourceLine(int lineNumber, const std::string &line,Statement *stmt) {
    // Replace this stub with your own code
    //todo
    if(!lines.count(lineNumber)) {
        lines.insert({lineNumber,{line,stmt}});
    } else {
        removeSourceLine(lineNumber);
        lines.insert({lineNumber,{line,stmt}});
    }
}

void Program::removeSourceLine(int lineNumber) {
    // Replace this stub with your own code
    //todo
    if(lines.count(lineNumber)) {
        delete lines.at(lineNumber).second;
        lines.erase(lineNumber);
    }
}

std::string Program::getSourceLine(int lineNumber) {
    // Replace this stub with your own code
    //todo
    if(lines.count(lineNumber)) {
        return lines.at(lineNumber).first;
    } else {
        return "";
    }
}


//void Program::removeSourceLine(int lineNumber) {

Statement *Program::getParsedStatement(int lineNumber) {
   // Replace this stub with your own code
   //todo
    if(lines.count(lineNumber)) {
        return lines.at(lineNumber).second;
    } else {
        return NULL;
    }
}

int Program::getFirstLineNumber() {
    // Replace this stub with your own code
    //todo
    if(!lines.empty()) {
        return lines.begin()->first;
    } else {
        return -1;
    }
}

int Program::getNextLineNumber(int lineNumber) {
    // Replace this stub with your own code
    //todo
    auto out=lines.upper_bound(lineNumber);
    if(out !=lines.end()) {
        return out->first;
    } else {
        return -1;
    }
}
void Program::run(EvalState &state) {
    isEnd=false;
    next_line=getFirstLineNumber();
    current_line=getFirstLineNumber();
    while(!(isEnd||current_line==-1)) {
        next_line=getNextLineNumber(current_line);
        lines.at(current_line).second->execute(state,*this);
        current_line=next_line;
    }
}
void Program::end() {
    isEnd=true;
}

void Program::list() {
    for(auto &l:lines) {
        std::cout<<l.second.first<<std::endl;
    }
}

void Program::gotoLine(int line_num) {
    if(lines.count(line_num)) {
        next_line=line_num;
    } else {
        error("LINE NUMBER ERROR");
    }
}

//more func to add
//todo


