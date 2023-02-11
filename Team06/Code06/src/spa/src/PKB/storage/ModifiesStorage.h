#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <iostream>
#include <deque>

typedef std::string Ent;
typedef std::string ProcedureName;
typedef int StmtNum;

class ModifiesStorage {
public:
    virtual void writeModifiesProcedure(ProcedureName name, std::vector<Ent> entities);
    virtual void writeModifiesStmtnum(StmtNum num, std::vector<Ent> entities);

    virtual std::unordered_set<Ent> getModifiesProcedure(ProcedureName name);
    virtual std::unordered_set<Ent> getModifiesStmtnum(StmtNum num);

private:
    std::unordered_map<ProcedureName, std::unordered_set<Ent>> procName_ent;
    std::unordered_map<StmtNum, std::unordered_set<Ent>> stmtNum_ent;
};
