#include "ASTVisitorUtils.h"

int visitIfStatementHelper(IfStatement* ifStatement) {
	auto statementList = ifStatement->elseStmtList.get();

	if (CAST_TO(IfStatement, statementList->statements.back().get())) {
		return visitIfStatementHelper(dynamic_cast<IfStatement*>(statementList->statements.back().get()));
	}
	else if (CAST_TO(WhileStatement, statementList->statements.back().get())) {
		return visitWhileStatementHelper(dynamic_cast<WhileStatement*>(statementList->statements.back().get()));
	}
	else {
		return statementList->statements.back().get()->statementNumber;
	}
}

int visitWhileStatementHelper(WhileStatement* whileStatement) {
	auto statementList = whileStatement->stmtList.get();
	if (CAST_TO(IfStatement, statementList->statements.back().get())) {
		return visitIfStatementHelper(dynamic_cast<IfStatement*>(statementList->statements.back().get()));
	}
	else if (CAST_TO(WhileStatement, statementList->statements.back().get())) {
		return visitWhileStatementHelper(dynamic_cast<WhileStatement*>(statementList->statements.back().get()));
	}
	else {
		return statementList->statements.back().get()->statementNumber;
	}
}
