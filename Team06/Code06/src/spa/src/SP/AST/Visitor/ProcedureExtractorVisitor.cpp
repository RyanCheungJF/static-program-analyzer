#include "ProcedureExtractorVisitor.h"

ProcedureExtractorVisitor::ProcedureExtractorVisitor(WritePKB* writePKB) {
	writeApi = writePKB;
}

void ProcedureExtractorVisitor::visitProgram(Program *program) {}

void ProcedureExtractorVisitor::visitProcedure(Procedure *procedure) {
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
void ProcedureExtractorVisitor::visitStatementList(StatementList *statementList) {}
void ProcedureExtractorVisitor::visitReadStatement(ReadStatement* readStatement) {}
void ProcedureExtractorVisitor::visitPrintStatement(PrintStatement *printStatement) {}
void ProcedureExtractorVisitor::visitAssignStatement(AssignStatement *assignStatement) {}
void ProcedureExtractorVisitor::visitCallStatement(CallStatement *callStatement) {}
void ProcedureExtractorVisitor::visitIfStatement(IfStatement *ifStatement) {}
void ProcedureExtractorVisitor::visitWhileStatement(WhileStatement* whileStatement) {}
void ProcedureExtractorVisitor::visitExpression(Expression* variable) {}
void ProcedureExtractorVisitor::visitConditionalExpression(ConditionalExpression* conditionalExpression) {}

int ProcedureExtractorVisitor::visitIfStatementHelper(IfStatement* ifStatement) {
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

int ProcedureExtractorVisitor::visitWhileStatementHelper(WhileStatement* whileStatement) {
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