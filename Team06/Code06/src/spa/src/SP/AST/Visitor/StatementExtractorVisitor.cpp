#include "StatementExtractorVisitor.h"

StatementExtractorVisitor::StatementExtractorVisitor(WritePKB* writePKB) {
	writeApi = writePKB;
}

void StatementExtractorVisitor::visitProgram(Program* program) {}
void StatementExtractorVisitor::visitProcedure(Procedure* procedure) {}
void StatementExtractorVisitor::visitStatementList(StatementList* statementList) {}

void StatementExtractorVisitor::visitReadStatement(ReadStatement* readStatement) {
	writeApi->setStatement("read", readStatement->statementNumber);
}

void StatementExtractorVisitor::visitPrintStatement(PrintStatement* printStatement) {
	writeApi->setStatement("print", printStatement->statementNumber);
}

void StatementExtractorVisitor::visitAssignStatement(AssignStatement* assignStatement) {
	writeApi->setStatement("assign", assignStatement->statementNumber);
}

void StatementExtractorVisitor::visitCallStatement(CallStatement* callStatement) {
	writeApi->setStatement("call", callStatement->statementNumber);
}

void StatementExtractorVisitor::visitIfStatement(IfStatement* ifStatement) {
	writeApi->setStatement("if", ifStatement->statementNumber);
}

void StatementExtractorVisitor::visitWhileStatement(WhileStatement* whileStatement) {
	writeApi->setStatement("while", whileStatement->statementNumber);
}

void StatementExtractorVisitor::visitExpression(Expression* variable) {};
void StatementExtractorVisitor::visitConditionalExpression(ConditionalExpression* conditionalExpression) {};