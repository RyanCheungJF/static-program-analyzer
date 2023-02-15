#pragma once
#include<vector>
#include<utility>
#include<string>
#include<unordered_set>

typedef int StmtNum;
typedef std::string ProcName;
typedef std::string Ent;

class ModifiesUsesStorage {
public:
    virtual bool exists(StmtNum num, Ent var) = 0;
    virtual bool exists(ProcName proc, Ent var) = 0;
    virtual std::unordered_set<Ent> getEnt(StmtNum num) = 0;
    virtual std::unordered_set<Ent> getEnt(ProcName proc) = 0;
    virtual std::unordered_set<StmtNum> getStmtsFromEnt(Ent var) = 0;
    virtual std::unordered_set<ProcName> getProcsFromEnt(Ent var) = 0;
    virtual std::pair<std::vector<StmtNum>, std::vector<std::string>> getAllStmtEntPairs() = 0;
    virtual std::pair<std::vector<std::string>, std::vector<std::string>> getAllProcEntPairs() = 0;
};