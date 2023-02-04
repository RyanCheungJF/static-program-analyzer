#include "FollowsExtractor.h"

FollowsExtractor::FollowsExtractor() {}

void FollowsExtractor::visitProgram(const Program *program) {}
void FollowsExtractor::visitProcedure(const Procedure* procedure) {}
void FollowsExtractor::visitStatementList(const StatementList *statementList) {
	for (int i = 0; i < statementList->statements.size(); i++) {
		for (int j = i + 1; j < statementList->statements.size(); j++) {
			// write that follows(i, j)
		}
	}
}
void FollowsExtractor::visitStatement(const Statement *statement) {}
void FollowsExtractor::visitReadStatement(const ReadStatement *readStatement) {}
void FollowsExtractor::visitPrintStatement(const PrintStatement *printStatement) {}
void FollowsExtractor::visitAssignStatement(const AssignStatement *assignStatement) {}
void FollowsExtractor::visitCallStatement(const CallStatement *callStatement) {}
void FollowsExtractor::visitIfStatement(const IfStatement *ifStatement) {}
void FollowsExtractor::visitWhileStatement(const WhileStatement *whileStatement) {}
void FollowsExtractor::visitExpression(const Expression *variable) {}
void FollowsExtractor::visitConditionalExpression(const ConditionalExpression *conditionalExpression) {}
