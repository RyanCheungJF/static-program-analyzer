#include "FollowsVisitor.h"

FollowsVisitor::FollowsVisitor() {}

void FollowsVisitor::visitProgram(const Program *program) {}
void FollowsVisitor::visitProcedure(const Procedure* procedure) {}
void FollowsVisitor::visitStatementList(const StatementList *statementList) {
	for (int i = 0; i < statementList->statements.size(); i++) {
		for (int j = i + 1; j < statementList->statements.size(); j++) {
			// write that follows(i, j)
		}
	}
}
void FollowsVisitor::visitStatement(const Statement *statement) {}
void FollowsVisitor::visitReadStatement(const ReadStatement *readStatement) {}
void FollowsVisitor::visitPrintStatement(const PrintStatement *printStatement) {}
void FollowsVisitor::visitAssignStatement(const AssignStatement *assignStatement) {}
void FollowsVisitor::visitCallStatement(const CallStatement *callStatement) {}
void FollowsVisitor::visitIfStatement(const IfStatement *ifStatement) {}
void FollowsVisitor::visitWhileStatement(const WhileStatement *whileStatement) {}
void FollowsVisitor::visitExpression(const Expression *variable) {}
void FollowsVisitor::visitConditionalExpression(const ConditionalExpression *conditionalExpression) {}