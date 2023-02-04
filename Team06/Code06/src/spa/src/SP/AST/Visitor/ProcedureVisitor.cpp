#include "ProcedureVisitor.h"

ProcedureVisitor::ProcedureVisitor() {}

void ProcedureVisitor::visitProgram(const Program *program) {}
void ProcedureVisitor::visitProcedure(const Procedure *procedure) {
	int firstStatementNumber = procedure->statementList.get()->statements.front()->statementNumber;
	// need to recurse down to check if last statement is if/ while
	// if (dynamic_cast<IfStatement*>(procedure->statementList.get()->statements.back())) {}
	int lastStatementNumber = procedure->statementList.get()->statements.back()->statementNumber;
	for (int i = firstStatementNumber; i < lastStatementNumber; i++) {
		// write to pkb procedure and line number
	}
}
void ProcedureVisitor::visitStatementList(const StatementList* statementList) {}
void ProcedureVisitor::visitStatement(const Statement* statement) {}
void ProcedureVisitor::visitReadStatement(const ReadStatement* readStatement) {}
void ProcedureVisitor::visitPrintStatement(const PrintStatement* printStatement) {}
void ProcedureVisitor::visitAssignStatement(const AssignStatement* assignStatement) {}
void ProcedureVisitor::visitCallStatement(const CallStatement* callStatement) {}
void ProcedureVisitor::visitIfStatement(const IfStatement* IfStatement) {}
void ProcedureVisitor::visitWhileStatement(const WhileStatement* whileStatement) {}
void ProcedureVisitor::visitExpression(const Expression* variable) {}
void ProcedureVisitor::visitConditionalExpression(const ConditionalExpression* conditionalExpression) {}