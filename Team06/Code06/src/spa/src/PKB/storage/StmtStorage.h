#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

typedef std::string Stmt;
typedef int StmtNum;

class StmtStorage {
public:
    virtual void writeStatement(Stmt s, std::vector<StmtNum> lines);
    virtual void writeStatement(Stmt s, StmtNum line);
    virtual bool checkStatement(Stmt stmt, StmtNum num);
    virtual std::unordered_set<StmtNum> getStatementNumbers(Stmt s);
private:
    std::unordered_map<Stmt, std::unordered_set<StmtNum>> stmt_stmtNum;
    std::unordered_map<StmtNum, std::unordered_set<Stmt>> stmtNum_stmt;
};
