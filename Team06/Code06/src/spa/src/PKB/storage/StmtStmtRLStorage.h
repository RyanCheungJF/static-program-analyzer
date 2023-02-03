#pragma once
#include<vector>
#include<utility>
#include<string>

typedef int StmtNum;

class StmtStmtRLStorage {
public:
    virtual void write(StmtNum stmtNum1, StmtNum stmtNum2);
    virtual bool exists(StmtNum stmtNum1, StmtNum stmtNum2);
    virtual std::vector<StmtNum> getRightWildcard(StmtNum leftStmtNum);
    virtual std::vector<StmtNum> getLeftWildcard(StmtNum rightStmtNum);
};