#pragma once
#include <deque>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "../../../src/utils/AppConstants.h"

class ModifiesUsesStorage {
public:
    void writeS(StmtNum num, std::unordered_set<Ent> entities);
    void writeP(ProcName proc, std::unordered_set<Ent> entities);

    bool exists(StmtNum num, Ent var);
    bool exists(ProcName proc, Ent var);
    std::unordered_set<Ent> getEnt(StmtNum num);
    std::unordered_set<Ent> getEnt(ProcName proc);
    std::unordered_set<StmtNum> getStmtsFromEnt(Ent var);
    std::unordered_set<ProcName> getProcsFromEnt(Ent var);
    std::pair<std::vector<StmtNum>, std::vector<std::string>> getAllStmtEntPairs();
    std::pair<std::vector<std::string>, std::vector<std::string>> getAllProcEntPairs();

private:
    std::unordered_map<StmtNum, std::unordered_set<Ent>> stmtNum_entities;
    std::unordered_map<Ent, std::unordered_set<StmtNum>> entities_stmtNum;

    std::unordered_map<ProcName, std::unordered_set<Ent>> procName_entities;
    std::unordered_map<Ent, std::unordered_set<ProcName>> entities_procName;
};