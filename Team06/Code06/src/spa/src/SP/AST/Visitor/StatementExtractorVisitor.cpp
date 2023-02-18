#include "StatementExtractorVisitor.h"

StatementExtractorVisitor::StatementExtractorVisitor(WritePKB* writePKB) {
	writeApi = writePKB;
}

void StatementExtractorVisitor::visitProgram(Program* program) {}
void StatementExtractorVisitor::visitProcedure(Procedure* procedure) {}
void StatementExtractorVisitor::visitStatementList(StatementList* statementList) {}

void StatementExtractorVisitor::visitReadStatement(ReadStatement* readStatement) {
	writeApi->setStatement(READ, readStatement->statementNumber);
}

void StatementExtractorVisitor::visitPrintStatement(PrintStatement* printStatement) {
	writeApi->setStatement(PRINT, printStatement->statementNumber);
}

void StatementExtractorVisitor::visitAssignStatement(AssignStatement* assignStatement) {
	writeApi->setStatement(ASSIGN, assignStatement->statementNumber);
}

void StatementExtractorVisitor::visitCallStatement(CallStatement* callStatement) {
	writeApi->setStatement(CALL, callStatement->statementNumber);
}

void StatementExtractorVisitor::visitIfStatement(IfStatement* ifStatement) {
	writeApi->setStatement(IF, ifStatement->statementNumber);
}

void StatementExtractorVisitor::visitWhileStatement(WhileStatement* whileStatement) {
	writeApi->setStatement(WHILE, whileStatement->statementNumber);
}

void StatementExtractorVisitor::visitExpression(Expression* variable) {}
void StatementExtractorVisitor::visitConditionalExpression(ConditionalExpression* conditionalExpression) {}