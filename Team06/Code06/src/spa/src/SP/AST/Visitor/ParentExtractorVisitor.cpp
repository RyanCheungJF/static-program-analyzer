#include "ParentExtractorVisitor.h"

ParentExtractorVisitor::ParentExtractorVisitor(WritePKB* writePKB) {
	writeApi = writePKB;
}

void ParentExtractorVisitor::visitProgram(Program* program) {}
void ParentExtractorVisitor::visitProcedure(Procedure* procedure) {}
void ParentExtractorVisitor::visitStatementList(StatementList* statementList) {}
void ParentExtractorVisitor::visitReadStatement(ReadStatement* readStatement) {}
void ParentExtractorVisitor::visitPrintStatement(PrintStatement* printStatement) {}
void ParentExtractorVisitor::visitAssignStatement(AssignStatement* assignStatement) {}
void ParentExtractorVisitor::visitCallStatement(CallStatement* callStatement) {}

void ParentExtractorVisitor::visitIfStatement(IfStatement* ifStatement) {
	// For Parent
	std::vector<StmtNum> parentVector;
	for (auto const& i : ifStatement->thenStmtList->statements) {
		parentVector.push_back(i->statementNumber);
	}
	for (auto const& i : ifStatement->elseStmtList->statements) {
		parentVector.push_back(i->statementNumber);
	}
	// For ParentT
	std::vector<StmtNum> parentTVector;
	StmtNum start = ifStatement->statementNumber + 1;
	StmtNum end = visitIfStatementHelper(ifStatement);
	for (StmtNum i = start; i <= end; i++) {
		parentTVector.push_back(i);
	}

	//writeApi->setParent(ifStatement->statementNumber, parentVector);
	//writeApi->setParentT(ifStatement->statementNumber, parentTVector);
}

void ParentExtractorVisitor::visitWhileStatement(WhileStatement* whileStatement) {
	// For Parent
	std::vector<StmtNum> parentVector;
	for (auto const& i : whileStatement->stmtList->statements) {
		parentVector.push_back(i->statementNumber);
	}
	// For ParentT
	std::vector<StmtNum> parentTVector;
	StmtNum start = whileStatement->statementNumber + 1;
	StmtNum end = visitWhileStatementHelper(whileStatement);
	for (StmtNum i = start; i <= end; i++) {
		parentTVector.push_back(i);
	}

	//writeApi->setParent(whileStatement->statementNumber, parentVector);
	//writeApi->setParentT(whileStatement->statementNumber, parentTVector);
}

void ParentExtractorVisitor::visitExpression(Expression* variable) {}
void ParentExtractorVisitor::visitConditionalExpression(ConditionalExpression* conditionalExpression) {}