#include "FollowsExtractor.h"

FollowsExtractor::FollowsExtractor() {}

void FollowsExtractor::visitProgram(const Program *program) {}
void FollowsExtractor::visitProcedure(const Procedure* procedure) {}
void FollowsExtractor::visitStatementList(const StatementList *statementList) {
	std::vector<std::pair<int, int>> followsVector;
	for (int i = 0; i < statementList->statements.size() - 1; i++) {
		std::cout << "Statement " << statementList->statements[i]->statementNumber << " follows " << statementList->statements[i + 1]->statementNumber << std::endl;
		auto followsPair = std::make_pair(statementList->statements[i]->statementNumber, statementList->statements[i + 1]->statementNumber);
		followsVector.push_back(followsPair);
	}
	// writeApi->setFollowsT(followsVector);
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
