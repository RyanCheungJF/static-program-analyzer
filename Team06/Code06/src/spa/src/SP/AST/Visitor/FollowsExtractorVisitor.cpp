#include "FollowsExtractorVisitor.h"

FollowsExtractorVisitor::FollowsExtractorVisitor(WritePKB* writePKB) {
	writeApi = writePKB;
}

void FollowsExtractorVisitor::visitProgram(Program *program) {}
void FollowsExtractorVisitor::visitProcedure(Procedure* procedure) {}

void FollowsExtractorVisitor::visitStatementList(StatementList *statementList) {
	// For Follows
	for (StmtNum i = 0; i < statementList->statements.size() - 1; i++) {
		writeApi->setFollows(statementList->statements[i]->statementNumber, statementList->statements[i + 1]->statementNumber);
	}
	// For FollowsT
	std::vector<std::pair<StmtNum, StmtNum>> followsTransitiveVector;
	for (StmtNum i = 0; i < statementList->statements.size(); i++) {
		for (StmtNum j = i + 1; j < statementList->statements.size(); j++) {
			auto followsTransitivePair = std::make_pair(statementList->statements[i]->statementNumber, statementList->statements[j]->statementNumber);
			followsTransitiveVector.push_back(followsTransitivePair);
		}
	}
	writeApi->setFollowsT(followsTransitiveVector);
}

void FollowsExtractorVisitor::visitReadStatement(ReadStatement *readStatement) {}
void FollowsExtractorVisitor::visitPrintStatement(PrintStatement *printStatement) {}
void FollowsExtractorVisitor::visitAssignStatement(AssignStatement *assignStatement) {}
void FollowsExtractorVisitor::visitCallStatement(CallStatement *callStatement) {}
void FollowsExtractorVisitor::visitIfStatement(IfStatement *ifStatement) {}
void FollowsExtractorVisitor::visitWhileStatement(WhileStatement *whileStatement) {}
void FollowsExtractorVisitor::visitExpression(Expression *variable) {}
void FollowsExtractorVisitor::visitConditionalExpression(ConditionalExpression *conditionalExpression) {}
