#pragma once

#include "../../../src/utils/AppConstants.h"

class StmtStorage {
public:
    virtual void writeStatement(Stmt s, StmtNum line);
    virtual bool checkStatement(Stmt stmt, StmtNum num);
    virtual std::unordered_set<StmtNum>* getStatementNumbers(Stmt s);
    virtual std::unordered_set<Stmt>* getStatementType(StmtNum num);

private:
    /*
     * Structure of table
     *  stmt | statementNumbers
     *  "if" | [1, 15, ...]
     *  "while" | [7, 11, ...]
     *  ... "assign", "call", "print", "read"
     */
    std::unordered_map<Stmt, std::unordered_set<StmtNum>> stmt_stmtNum;
    std::unordered_map<StmtNum, std::unordered_set<Stmt>> stmtNum_stmt;

    std::unordered_set<StmtNum> emptyStmtNum;
    std::unordered_set<Stmt> emptyStmt;
};
