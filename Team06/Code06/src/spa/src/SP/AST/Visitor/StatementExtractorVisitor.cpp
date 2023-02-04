#include "StatementExtractorVisitor.h"

void StatementExtractorVisitor::visitProgram(const Program* program) {}

void StatementExtractorVisitor::visitProcedure(const Procedure* procedure) {}

void StatementExtractorVisitor::visitStatementList(const StatementList* statementList) {}

void StatementExtractorVisitor::visitStatement(const Statement* statement) {}

void StatementExtractorVisitor::visitReadStatement(const ReadStatement* readStatement) {}

void StatementExtractorVisitor::visitPrintStatement(const PrintStatement* printStatement) {}

void StatementExtractorVisitor::visitAssignStatement(const AssignStatement* assignStatement) {}

void StatementExtractorVisitor::visitCallStatement(const CallStatement* callStatement) {}

void StatementExtractorVisitor::visitIfStatement(const IfStatement* ifStatement) {}

void StatementExtractorVisitor::visitWhileStatement(const WhileStatement* whileStatement) {}

void StatementExtractorVisitor::visitExpression(const Expression* variable) {};

void StatementExtractorVisitor::visitConditionalExpression(const ConditionalExpression* conditionalExpression) {};