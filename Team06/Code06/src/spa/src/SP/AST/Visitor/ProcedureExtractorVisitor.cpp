#include "ProcedureExtractorVisitor.h"

ProcedureExtractorVisitor::ProcedureExtractorVisitor(WritePKB* writePKB) {
	writeApi = writePKB;
}

int ProcedureExtractorVisitor::visitIfStatementHelper(const IfStatement* ifStatement) {
	auto statementList = ifStatement->elseStmtList.get();

	if (dynamic_cast<IfStatement*>(statementList->statements.back().get())) {
		return visitIfStatementHelper(dynamic_cast<IfStatement*>(statementList->statements.back().get()));
	}
	else if (dynamic_cast<WhileStatement*>(statementList->statements.back().get())) {
		return visitWhileStatementHelper(dynamic_cast<WhileStatement*>(statementList->statements.back().get()));
	}
	else {
		return statementList->statements.back().get()->statementNumber;
	}
}

int ProcedureExtractorVisitor::visitWhileStatementHelper(const WhileStatement* whileStatement) {
	auto statementList = whileStatement->stmtList.get();
	if (dynamic_cast<IfStatement*>(statementList->statements.back().get())) {
		return visitIfStatementHelper(dynamic_cast<IfStatement*>(statementList->statements.back().get()));
	}
	else if (dynamic_cast<WhileStatement*>(statementList->statements.back().get())) {
		return visitWhileStatementHelper(dynamic_cast<WhileStatement*>(statementList->statements.back().get()));
	}
	else {
		return statementList->statements.back().get()->statementNumber;
	}
}

void ProcedureExtractorVisitor::visitProgram(const Program *program) {}

void ProcedureExtractorVisitor::visitProcedure(const Procedure *procedure) {
	int firstStatementNumber = procedure->statementList->statements.front()->statementNumber;
	int lastStatementNumber = procedure->statementList->statements.back()->statementNumber;

	// if its an if or while statement, we recurse down further
	if (dynamic_cast<IfStatement*>(procedure->statementList->statements.back().get())) {
		lastStatementNumber = visitIfStatementHelper(dynamic_cast<IfStatement*>(procedure->statementList->statements.back().get()));
	}
	if (dynamic_cast<WhileStatement*>(procedure->statementList->statements.back().get())) {
		lastStatementNumber = visitWhileStatementHelper(dynamic_cast<WhileStatement*>(procedure->statementList->statements.back().get()));
	}
	std::vector<int> statementNumbers;
	for (int i = firstStatementNumber; i <= lastStatementNumber; i++) {
		statementNumbers.push_back(i);
	}
	writeApi->setProcedure(procedure->procedureName, statementNumbers);
}
void ProcedureExtractorVisitor::visitStatementList(const StatementList *statementList) {}

void ProcedureExtractorVisitor::visitStatement(const Statement *statement) {}

void ProcedureExtractorVisitor::visitReadStatement(const ReadStatement* readStatement) {}

void ProcedureExtractorVisitor::visitPrintStatement(const PrintStatement *printStatement) {}

void ProcedureExtractorVisitor::visitAssignStatement(const AssignStatement *assignStatement) {}

void ProcedureExtractorVisitor::visitCallStatement(const CallStatement *callStatement) {}

void ProcedureExtractorVisitor::visitIfStatement(const IfStatement *ifStatement) {}

void ProcedureExtractorVisitor::visitWhileStatement(const WhileStatement* whileStatement) {}

void ProcedureExtractorVisitor::visitExpression(const Expression* variable) {}

void ProcedureExtractorVisitor::visitConditionalExpression(const ConditionalExpression* conditionalExpression) {}