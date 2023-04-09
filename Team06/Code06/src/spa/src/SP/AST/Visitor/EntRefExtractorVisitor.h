#pragma once

#include <iostream>
#include <vector>

#include "../../../PKB/WritePKB.h"
#include "ASTVisitor.h"
#include "ASTVisitorUtils.h"

class EntRefExtractorVisitor : public ASTVisitor {
private:
    WritePKB* writeApi;
    void containerStmtHelper(ConditionalExpression* condExpr, StmtNum stmtNum, std::unordered_set<Ent>& variables,
                             std::unordered_set<Const>& constants);

public:
    explicit EntRefExtractorVisitor(WritePKB* writePKB);

    void visitProgram(Program* program) override;
    void visitProcedure(Procedure* procedure) override;
    void visitStatementList(StatementList* statement) override;

    void visitReadStatement(ReadStatement* readStatement) override;
    void visitPrintStatement(PrintStatement* printStatement) override;
    void visitAssignStatement(AssignStatement* assignStatement) override;
    void visitCallStatement(CallStatement* callStatement) override;
    void visitIfStatement(IfStatement* ifStatement) override;
    void visitWhileStatement(WhileStatement* whileStatement) override;

    void visitExpression(Expression* expression) override;
    void visitConditionalExpression(ConditionalExpression* conditionalExpression) override;
};