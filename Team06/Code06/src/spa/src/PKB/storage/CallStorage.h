#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <iostream>

typedef std::string Stmt;
typedef int StmtNum;

class CallStorage {
public:
    virtual void writeCall(Stmt caller, Stmt callee);
    
private:
    /*
     * Structure of table example below
     *  caller | callee
     *  "proc1" | {"proc2", "proc3"}
     *  "proc2" | {"proc3"}
     */
    std::unordered_map<Stmt, std::unordered_set<Stmt>> caller_callee;
};
