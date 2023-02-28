#pragma once

#include "../../../PKB/WritePKB.h"
#include "../../../utils/AppConstants.h"
#include "ASTVisitor.h"
#include "ASTVisitorUtils.h"
#include <iostream>

class StatementExtractorVisitor : public ASTVisitor {
private:
  WritePKB *writeApi;

public:
  StatementExtractorVisitor(WritePKB *writePKB);

  void visitProgram(Program *program) override;
  void visitProcedure(Procedure *procedure) override;
  void visitStatementList(StatementList *statement) override;

  void visitReadStatement(ReadStatement *readStatement) override;
  void visitPrintStatement(PrintStatement *printStatement) override;
  void visitAssignStatement(AssignStatement *assignStatement) override;
  void visitCallStatement(CallStatement *callStatement) override;
  void visitIfStatement(IfStatement *IfStatement) override;
  void visitWhileStatement(WhileStatement *whileStatement) override;

  void visitExpression(Expression *expression) override;
  void visitConditionalExpression(
      ConditionalExpression *conditionalExpression) override;
};