#pragma once

#include <vector>

#include "ASTNode.h"
#include "Statement.h"

class StatementList : public ASTNode {
public:
    std::vector<std::unique_ptr<Statement>> statements;

    StatementList();
    explicit StatementList(std::vector<std::unique_ptr<Statement>> statements);

    Statement* getLastStatement() const;
    StmtNum getLastStatementNumber() const;
    Statement* getStmtForStmtIdx(int idx) const;
    StmtNum getStmtNumForStmtIdx(int idx) const;

    void accept(ASTVisitor* visitor) override;
};