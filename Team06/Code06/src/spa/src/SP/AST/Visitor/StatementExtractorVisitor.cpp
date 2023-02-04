#include "StatementExtractorVisitor.h"

StatementExtractorVisitor::StatementExtractorVisitor(WritePKB* writePKB) {
	writeApi = writePKB;
}

void StatementExtractorVisitor::visitProgram(const Program* program) {}

void StatementExtractorVisitor::visitProcedure(const Procedure* procedure) {}

void StatementExtractorVisitor::visitStatementList(const StatementList* statementList) {}

void StatementExtractorVisitor::visitStatement(const Statement* statement) {}

void StatementExtractorVisitor::visitReadStatement(const ReadStatement* readStatement) {
	writeApi->setStatement("read", readStatement->statementNumber);
}

void StatementExtractorVisitor::visitPrintStatement(const PrintStatement* printStatement) {
	writeApi->setStatement("print", printStatement->statementNumber);
}

void StatementExtractorVisitor::visitAssignStatement(const AssignStatement* assignStatement) {
	writeApi->setStatement("assign", assignStatement->statementNumber);
}

void StatementExtractorVisitor::visitCallStatement(const CallStatement* callStatement) {
	writeApi->setStatement("call", callStatement->statementNumber);
}

void StatementExtractorVisitor::visitIfStatement(const IfStatement* ifStatement) {
	std::vector<int> statementNumbers;
	int start = ifStatement->statementNumber;
	int end = ifStatement->elseStmtList->statements.back()->statementNumber;

	for (int i = start; i <= end; i++) {
		statementNumbers.push_back(i);
	}

	writeApi->setStatement("if", statementNumbers);
}

void StatementExtractorVisitor::visitWhileStatement(const WhileStatement* whileStatement) {
	std::vector<int> statementNumbers;
	int start = whileStatement->statementNumber;
	int end = whileStatement->stmtList->statements.back()->statementNumber;

	for (int i = start; i <= end; i++) {
		statementNumbers.push_back(i);
	}

	writeApi->setStatement("while", statementNumbers);
}

void StatementExtractorVisitor::visitExpression(const Expression* variable) {};

void StatementExtractorVisitor::visitConditionalExpression(const ConditionalExpression* conditionalExpression) {};