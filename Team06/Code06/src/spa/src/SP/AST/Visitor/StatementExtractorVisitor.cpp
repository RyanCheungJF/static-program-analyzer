#include "StatementExtractorVisitor.h"

StatementExtractorVisitor::StatementExtractorVisitor(WritePKB *writePKB) {
  writeApi = writePKB;
}

void StatementExtractorVisitor::visitProgram(Program *program) {}
void StatementExtractorVisitor::visitProcedure(Procedure *procedure) {}
void StatementExtractorVisitor::visitStatementList(
    StatementList *statementList) {}

void StatementExtractorVisitor::visitReadStatement(
    ReadStatement *readStatement) {
  writeApi->setStatement(AppConstants::READ, readStatement->statementNumber);
}

void StatementExtractorVisitor::visitPrintStatement(
    PrintStatement *printStatement) {
  writeApi->setStatement(AppConstants::PRINT, printStatement->statementNumber);
}

void StatementExtractorVisitor::visitAssignStatement(
    AssignStatement *assignStatement) {
  writeApi->setStatement(AppConstants::ASSIGN,
                         assignStatement->statementNumber);
}

void StatementExtractorVisitor::visitCallStatement(
    CallStatement *callStatement) {
  writeApi->setStatement(AppConstants::CALL, callStatement->statementNumber);
}

void StatementExtractorVisitor::visitIfStatement(IfStatement *ifStatement) {
  writeApi->setStatement(AppConstants::IF, ifStatement->statementNumber);
}

void StatementExtractorVisitor::visitWhileStatement(
    WhileStatement *whileStatement) {
  writeApi->setStatement(AppConstants::WHILE, whileStatement->statementNumber);
}

void StatementExtractorVisitor::visitExpression(Expression *variable) {}
void StatementExtractorVisitor::visitConditionalExpression(
    ConditionalExpression *conditionalExpression) {}