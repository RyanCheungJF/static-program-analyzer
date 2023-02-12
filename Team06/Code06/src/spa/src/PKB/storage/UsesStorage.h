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
    void write(StmtNum num, Ent ent);
    bool exists(StmtNum num, Ent ent);
    std::unordered_set<Ent> getLeftWildcard(StmtNum followee); // ie all entities that fulfills a Uses relationship

private:
    std::unordered_map<StmtNum, std::unordered_set<Ent>> stmtNum_entities;
    std::unordered_map<Ent, std::unordered_set<StmtNum>> entities_stmtNum;


};
