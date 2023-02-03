#pragma once
#include<vector>
#include<utility>
#include<string>

typedef int StmtNum;
typedef string Ent;

class StmtEntRLStorage {
public:
    virtual void write(StmtNum stmtNum, Ent ent);
    virtual bool exists(StmtNum stmtNum1, Ent ent);
    virtual std::vector<Ent> getEnts(StmtNum stmtNum);
    virtual std::vector<StmtNum> getStmts(Ent ent);
};