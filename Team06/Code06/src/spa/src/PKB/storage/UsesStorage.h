#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <iostream>
#include <deque>

typedef std::string Ent;
typedef std::string StmtType;
typedef std::string ProcName;
typedef int StmtNum;


class UsesStorage {
public:
    void writeUsesS(StmtNum num, std::unordered_set<Ent> entities);
    void writeUsesP(ProcName name, std::unordered_set<Ent> entities);

    std::unordered_set<Ent> getUsesS(StmtNum num);
    std::unordered_set<Ent> getUsesP(ProcName name);

private:
    std::unordered_map<StmtNum, std::unordered_set<Ent>> stmtNum_entities;
    std::unordered_map<Ent, std::unordered_set<StmtNum>> entities_stmtNum;

    std::unordered_map<ProcName, std::unordered_set<Ent>> procName_entities;
    std::unordered_map<Ent, std::unordered_set<ProcName>> entities_procName;


};
