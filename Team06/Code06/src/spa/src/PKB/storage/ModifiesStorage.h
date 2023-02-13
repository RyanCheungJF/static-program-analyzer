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
    void write(StmtNum num, std::vector<Ent> entities);

    std::unordered_set<Ent> getModifiesS(StmtNum num);

private:
    std::unordered_map<StmtNum, std::unordered_set<Ent>> stmtNum_entities;
    std::unordered_map<Ent, std::unordered_set<StmtNum>> entities_stmtNum;


};
