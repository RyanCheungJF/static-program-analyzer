#pragma once

#include <vector>

#include "ASTNode.h"
#include "Statement.h"

class StatementList : public ASTNode {
public:
    std::vector<std::unique_ptr<Statement>> statements;

    StatementList();
    StatementList(std::vector<std::unique_ptr<Statement>> statements);

    StmtNum getStmtNumForStmtIdx(int idx);

    void accept(ASTVisitor* visitor) override;
};