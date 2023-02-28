#include "ParentExtractorVisitor.h"

ParentExtractorVisitor::ParentExtractorVisitor(WritePKB *writePKB) {
  writeApi = writePKB;
}

void ParentExtractorVisitor::visitProgram(Program *program) {}
void ParentExtractorVisitor::visitProcedure(Procedure *procedure) {}
void ParentExtractorVisitor::visitStatementList(StatementList *statementList) {}
void ParentExtractorVisitor::visitReadStatement(ReadStatement *readStatement) {}
void ParentExtractorVisitor::visitPrintStatement(
    PrintStatement *printStatement) {}
void ParentExtractorVisitor::visitAssignStatement(
    AssignStatement *assignStatement) {}
void ParentExtractorVisitor::visitCallStatement(CallStatement *callStatement) {}

void ParentExtractorVisitor::visitIfStatement(IfStatement *ifStatement) {
  // For Parent
  for (auto const &i : ifStatement->thenStmtList->statements) {
    writeApi->setParent(ifStatement->statementNumber, i->statementNumber);
  }
  for (auto const &i : ifStatement->elseStmtList->statements) {
    writeApi->setParent(ifStatement->statementNumber, i->statementNumber);
  }
  // For ParentT
  std::unordered_set<StmtNum> parentTSet;
  StmtNum start = ifStatement->statementNumber + 1;
  StmtNum end = visitIfStatementHelper(ifStatement);
  for (StmtNum i = start; i <= end; i++) {
    parentTSet.insert(i);
  }

  writeApi->setParentT(ifStatement->statementNumber, parentTSet);
}

void ParentExtractorVisitor::visitWhileStatement(
    WhileStatement *whileStatement) {
  // For Parent
  for (auto const &i : whileStatement->stmtList->statements) {
    writeApi->setParent(whileStatement->statementNumber, i->statementNumber);
  }
  // For ParentT
  std::unordered_set<StmtNum> parentTSet;
  StmtNum start = whileStatement->statementNumber + 1;
  StmtNum end = visitWhileStatementHelper(whileStatement);
  for (StmtNum i = start; i <= end; i++) {
    parentTSet.insert(i);
  }

  writeApi->setParentT(whileStatement->statementNumber, parentTSet);
}

void ParentExtractorVisitor::visitExpression(Expression *variable) {}
void ParentExtractorVisitor::visitConditionalExpression(
    ConditionalExpression *conditionalExpression) {}