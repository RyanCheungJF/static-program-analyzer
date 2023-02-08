#include "FollowsExtractorVisitor.h"

FollowsExtractorVisitor::FollowsExtractorVisitor(WritePKB* writePKB) {
	writeApi = writePKB;
}

void FollowsExtractorVisitor::visitProgram(Program *program) {}
void FollowsExtractorVisitor::visitProcedure(Procedure* procedure) {}

void FollowsExtractorVisitor::visitStatementList(StatementList *statementList) {
	std::vector<std::pair<int, int>> followsVector;
	for (int i = 0; i < statementList->statements.size() - 1; i++) {
		//auto followsPair = std::make_pair(statementList->statements[i]->statementNumber, statementList->statements[i + 1]->statementNumber);
		//followsVector.push_back(followsPair);
		writeApi->setFollows(statementList->statements[i]->statementNumber, statementList->statements[i + 1]->statementNumber);
	}
	//writeApi->setFollowsT(followsVector);
}

void FollowsExtractorVisitor::visitReadStatement(ReadStatement *readStatement) {}
void FollowsExtractorVisitor::visitPrintStatement(PrintStatement *printStatement) {}
void FollowsExtractorVisitor::visitAssignStatement(AssignStatement *assignStatement) {}
void FollowsExtractorVisitor::visitCallStatement(CallStatement *callStatement) {}
void FollowsExtractorVisitor::visitIfStatement(IfStatement *ifStatement) {}
void FollowsExtractorVisitor::visitWhileStatement(WhileStatement *whileStatement) {}
void FollowsExtractorVisitor::visitExpression(Expression *variable) {}
void FollowsExtractorVisitor::visitConditionalExpression(ConditionalExpression *conditionalExpression) {}
