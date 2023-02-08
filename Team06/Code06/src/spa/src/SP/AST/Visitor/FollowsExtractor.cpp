#include "FollowsExtractor.h"

FollowsExtractor::FollowsExtractor(WritePKB* writePKB) {
	writeApi = writePKB;
}

void FollowsExtractor::visitProgram(Program *program) {}
void FollowsExtractor::visitProcedure(Procedure* procedure) {}

void FollowsExtractor::visitStatementList(StatementList *statementList) {
	std::vector<std::pair<int, int>> followsVector;
	for (int i = 0; i < statementList->statements.size() - 1; i++) {
		//auto followsPair = std::make_pair(statementList->statements[i]->statementNumber, statementList->statements[i + 1]->statementNumber);
		//followsVector.push_back(followsPair);
		writeApi->setFollows(statementList->statements[i]->statementNumber, statementList->statements[i + 1]->statementNumber);
	}
	//writeApi->setFollowsT(followsVector);
}

void FollowsExtractor::visitReadStatement(ReadStatement *readStatement) {}
void FollowsExtractor::visitPrintStatement(PrintStatement *printStatement) {}
void FollowsExtractor::visitAssignStatement(AssignStatement *assignStatement) {}
void FollowsExtractor::visitCallStatement(CallStatement *callStatement) {}
void FollowsExtractor::visitIfStatement(IfStatement *ifStatement) {}
void FollowsExtractor::visitWhileStatement(WhileStatement *whileStatement) {}
void FollowsExtractor::visitExpression(Expression *variable) {}
void FollowsExtractor::visitConditionalExpression(ConditionalExpression *conditionalExpression) {}
