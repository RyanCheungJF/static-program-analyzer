#pragma once

#include "ASTNode.h"
#include "StatementList.h"

class Procedure : public ASTNode {
public:
    ProcName procedureName;
    std::unique_ptr<StatementList> statementList;

    Procedure();
    Procedure(ProcName procedureName, std::unique_ptr<StatementList> statementList);

    Statement* getLastStatement() const;
    StmtNum getLastStatementNumber() const;
    StmtNum getFirstStatementNumber() const;
    std::vector<std::unique_ptr<Statement>>& getStatements() const;

    void accept(ASTVisitor* visitor) override;
};