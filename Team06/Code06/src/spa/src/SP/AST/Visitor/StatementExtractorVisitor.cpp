#include "StatementExtractorVisitor.h"

void StatementExtractorVisitor::visitProgram(const Program* program) {}

void StatementExtractorVisitor::visitProcedure(const Procedure* procedure) {}

void StatementExtractorVisitor::visitStatementList(const StatementList* statementList) {}

void StatementExtractorVisitor::visitStatement(const Statement* statement) {}

void StatementExtractorVisitor::visitReadStatement(const ReadStatement* readStatement) {
	std::cout << "read";
	std::cout << readStatement->statementNumber;
	std::cout << readStatement->varName << std::endl;
}

void StatementExtractorVisitor::visitPrintStatement(const PrintStatement* printStatement) {
	std::cout << "print";
	std::cout << printStatement->statementNumber;
	std::cout << printStatement->varName << std::endl;
}

void StatementExtractorVisitor::visitAssignStatement(const AssignStatement* assignStatement) {
	std::cout << "assign";
	std::cout << assignStatement->statementNumber;
	std::cout << assignStatement->varName << std::endl;
}

void StatementExtractorVisitor::visitCallStatement(const CallStatement* callStatement) {
	std::cout << "call";
	std::cout << callStatement->statementNumber;
	std::cout << callStatement->procName << std::endl;
}

void StatementExtractorVisitor::visitIfStatement(const IfStatement* ifStatement) {
	std::cout << "if";
	std::cout << ifStatement->statementNumber;
	std::cout << ifStatement->statementNumber << std::endl;
}

void StatementExtractorVisitor::visitWhileStatement(const WhileStatement* whileStatement) {
	std::cout << "while";
	std::cout << whileStatement->statementNumber << std::endl;
}

void StatementExtractorVisitor::visitExpression(const Expression* variable) {};

void StatementExtractorVisitor::visitConditionalExpression(const ConditionalExpression* conditionalExpression) {};