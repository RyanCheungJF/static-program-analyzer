#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <iostream>

typedef std::string ProcName;
typedef int StmtNum;

class CallStorage {
public:
    virtual void writeCall(StmtNum callLine, ProcName callee);
    virtual std::vector<std::pair<StmtNum, ProcName>> getCallStatements();
    
private:
    /*
     * Structure of table example below
     *  call | callee
     *  11 | "proc2"
     *  28 | "proc3"
     */
    std::unordered_map<StmtNum, std::string> callLine_callee;
};
