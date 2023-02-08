#pragma once
#include<unordered_set>
#include<utility>
#include<string>

typedef int StmtNum;

class StmtStmtRLStorage {
public:
    virtual bool exists(StmtNum stmtNum1, StmtNum stmtNum2) = 0;
    virtual std::unordered_set<StmtNum> getRightWildcard(StmtNum leftStmtNum) = 0;
    virtual std::unordered_set<StmtNum> getLeftWildcard(StmtNum rightStmtNum) = 0;
    virtual std::pair<std::vector<StmtNum>, std::vector<StmtNum>> getAllPairs() = 0;
};