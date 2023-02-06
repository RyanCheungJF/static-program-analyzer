#include "StatementExtractorVisitor.h"

StatementExtractorVisitor::StatementExtractorVisitor(WritePKB* writePKB) {
	writeApi = writePKB;
}

void StatementExtractorVisitor::visitProgram(Program* program) {}

void StatementExtractorVisitor::visitProcedure(Procedure* procedure) {}

void StatementExtractorVisitor::visitStatementList(StatementList* statementList) {}

void StatementExtractorVisitor::visitStatement(Statement* statement) {}

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
	std::vector<int> statementNumbers;
	int start = ifStatement->statementNumber;
	int end = ifStatement->elseStmtList->statements.back()->statementNumber;

	for (int i = start; i <= end; i++) {
		statementNumbers.push_back(i);
	}

	writeApi->setStatement("if", statementNumbers);
}

void StatementExtractorVisitor::visitWhileStatement(WhileStatement* whileStatement) {
	std::vector<int> statementNumbers;
	int start = whileStatement->statementNumber;
	int end = whileStatement->stmtList->statements.back()->statementNumber;

	for (int i = start; i <= end; i++) {
		statementNumbers.push_back(i);
	}

	writeApi->setStatement("while", statementNumbers);
}

void StatementExtractorVisitor::visitExpression(Expression* variable) {};

void StatementExtractorVisitor::visitConditionalExpression(ConditionalExpression* conditionalExpression) {};