#include "FollowsExtractor.h"

FollowsExtractor::FollowsExtractor() {};

void FollowsExtractor::visitProgram(const Program* program) {
	for (auto const& proc : program->procedureList) {
		proc.get()->accept(this);
	};
};

void FollowsExtractor::visitProcedure(const Procedure* procedure) {
	procedure->statementList.get()->accept(this);
};

void FollowsExtractor::visitStatementList(const StatementList* statementList) {
	// within a statement list, pairs of statements have a follows relation
	for (int i = 0; i < statementList->statements.size() - 1; i++) {
		// write relationship to PKB
        // write(statementList->statements[i], statementList->statements[i + 1])
        writeApi->setFollows(statementList->statements[i]->lineNumber, statementList->statements[i + 1]->lineNumber);
	};
};

void FollowsExtractor::visitStatement(const Statement* statement) {};

void FollowsExtractor::visitReadStatement(const ReadStatement* readStatement) {};

void FollowsExtractor::visitPrintStatement(const PrintStatement* printStatement) {};

void FollowsExtractor::visitAssignStatement(const AssignStatement* assignStatement) {};

void FollowsExtractor::visitCallStatement(const CallStatement* callStatement) {};

void FollowsExtractor::visitIfStatement(const IfStatement* ifStatement) {
	// if statement has statement lists for if and else condition
	// ifStatement->thenStatementList.accept(this);
	// ifStatement->elseStatementList.accept(this);
};

void FollowsExtractor::visitWhileStatement(const WhileStatement* whileStatement) {
	// whileStatement->statementList.accept(this);
};

void FollowsExtractor::visitExpression(const Expression* variable) {};

void FollowsExtractor::visitConditionalExpression(const ConditionalExpression* conditionalExpression) {};