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


class ModifiesStorage {
public:
    void writeModifiesS(StmtNum num, std::vector<Ent> entities);
    void writeModifiesP(ProcName name, std::vector<Ent> entities);

    std::unordered_set<Ent> getModifiesS(StmtNum num);
    std::unordered_set<Ent> getModifiesP(ProcName name);

private:
    std::unordered_map<StmtNum, std::unordered_set<Ent>> stmtNum_entities;
    std::unordered_map<Ent, std::unordered_set<StmtNum>> entities_stmtNum;

    std::unordered_map<ProcName, std::unordered_set<Ent>> procName_entities;
    std::unordered_map<Ent, std::unordered_set<ProcName>> entities_procName;


};
