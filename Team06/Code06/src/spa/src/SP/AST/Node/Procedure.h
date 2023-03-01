#pragma once

#include "ASTNode.h"
#include "StatementList.h"

class Procedure : public ASTNode {
public:
    ProcName procedureName;
    std::unique_ptr<StatementList> statementList;

    Procedure();
    Procedure(ProcName procedureName, std::unique_ptr<StatementList> statementList);

    Statement* getLastStatement();
    StmtNum getLastStatementNumber();
    StmtNum getFirstStatementNumber();

    void accept(ASTVisitor* visitor) override;
};