#include "ProcedureVisitor.h"

ProcedureVisitor::ProcedureVisitor() {}

int ProcedureVisitor::visitIfStatementHelper(const IfStatement* ifStatement) {
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

int ProcedureVisitor::visitWhileStatementHelper(const WhileStatement* whileStatement) {
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

void ProcedureVisitor::visitProgram(const Program *program) {}

void ProcedureVisitor::visitProcedure(const Procedure *procedure) {
	int firstStatementNumber = procedure->statementList.get()->statements.front()->statementNumber;
	int lastStatementNumber = procedure->statementList.get()->statements.back()->statementNumber;

	// if its an if or while statement, we recurse down further
	if (dynamic_cast<IfStatement*>(procedure->statementList.get()->statements.back().get())) {
		lastStatementNumber = visitIfStatementHelper(dynamic_cast<IfStatement*>(procedure->statementList.get()->statements.back().get()));
	}
	if (dynamic_cast<WhileStatement*>(procedure->statementList.get()->statements.back().get())) {
		lastStatementNumber = visitWhileStatementHelper(dynamic_cast<WhileStatement*>(procedure->statementList.get()->statements.back().get()));
	}
	std::cout << "First Number! " << firstStatementNumber << std::endl;
	std::cout << "Last Number! " << lastStatementNumber << std::endl;
	for (int i = firstStatementNumber; i <= lastStatementNumber; i++) {
		// setProcedure(procedure->procedureName, i);
	}
}
void ProcedureVisitor::visitStatementList(const StatementList *statementList) {}

void ProcedureVisitor::visitStatement(const Statement *statement) {}

void ProcedureVisitor::visitReadStatement(const ReadStatement* readStatement) {}

void ProcedureVisitor::visitPrintStatement(const PrintStatement *printStatement) {}

void ProcedureVisitor::visitAssignStatement(const AssignStatement *assignStatement) {}

void ProcedureVisitor::visitCallStatement(const CallStatement *callStatement) {}

void ProcedureVisitor::visitIfStatement(const IfStatement *ifStatement) {}

void ProcedureVisitor::visitWhileStatement(const WhileStatement* whileStatement) {}

void ProcedureVisitor::visitExpression(const Expression* variable) {}

void ProcedureVisitor::visitConditionalExpression(const ConditionalExpression* conditionalExpression) {}