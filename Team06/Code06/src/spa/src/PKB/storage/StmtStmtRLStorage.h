#pragma once
#include<vector>
#include<utility>
#include<string>

typedef int StmtNum;

class StmtStmtRLStorage {
public:
    virtual void write(StmtNum stmtNum1, StmtNum stmtNum2) = 0;
    virtual bool exists(StmtNum stmtNum1, StmtNum stmtNum2) = 0;
    virtual std::vector<StmtNum> getRightWildcard(StmtNum leftStmtNum) = 0;
    virtual std::vector<StmtNum> getLeftWildcard(StmtNum rightStmtNum) = 0;
    virtual std::pair<std::vector<StmtNum>, std::vector<StmtNum>> getAllPairs() = 0;
};