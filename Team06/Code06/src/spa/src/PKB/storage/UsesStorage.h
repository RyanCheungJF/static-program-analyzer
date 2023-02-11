#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <iostream>
#include <deque>

typedef std::string Ent;
typedef std::string StmtType;
typedef std::string ProcedureName;
typedef int StmtNum;

struct usesCallsHashFunction
{
    size_t operator()(const std::pair<std::string, int>& x) const
    {
        std::size_t h1 = std::hash<std::string>{}(x.first);
        std::size_t h2 = std::hash<double>{}(x.second);
        return h1 ^ h2;
    }
};

class UsesStorage {
public:
    virtual void writeUsesProcedure(ProcedureName name, std::vector<Ent> entities);
    virtual void writeUsesStmtnum(StmtNum num, std::vector<Ent> entities);

    virtual std::unordered_set<Ent> getUsesProcedureEntities(ProcedureName name);
    virtual std::unordered_set<Ent> getUsesStmtnumEntities(StmtNum num);

private:
    std::unordered_map<ProcedureName, std::unordered_set<Ent>> procName_ent;
    std::unordered_map<StmtNum, std::unordered_set<Ent>> stmtNum_ent;
};
