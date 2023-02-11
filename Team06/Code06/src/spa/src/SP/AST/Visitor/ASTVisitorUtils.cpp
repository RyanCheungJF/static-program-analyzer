#include "ASTVisitorUtils.h"

int visitIfStatementHelper(IfStatement* ifStatement) {
	auto statementList = ifStatement->elseStmtList.get();

	if (CAST_TO(IfStatement, statementList->statements.back().get())) {
		return visitIfStatementHelper(CAST_TO(IfStatement, statementList->statements.back().get()));
	}
	else if (CAST_TO(WhileStatement, statementList->statements.back().get())) {
		return visitWhileStatementHelper(CAST_TO(WhileStatement, statementList->statements.back().get()));
	}
	else {
		return statementList->statements.back().get()->statementNumber;
	}
}

int visitWhileStatementHelper(WhileStatement* whileStatement) {
	auto statementList = whileStatement->stmtList.get();
	if (CAST_TO(IfStatement, statementList->statements.back().get())) {
		return visitIfStatementHelper(CAST_TO(IfStatement, statementList->statements.back().get()));
	}
	else if (CAST_TO(WhileStatement, statementList->statements.back().get())) {
		return visitWhileStatementHelper(CAST_TO(WhileStatement, statementList->statements.back().get()));
	}
	else {
		return statementList->statements.back().get()->statementNumber;
	}
}

void visitCondExprHelper(ConditionalExpression* e, std::unordered_set<std::string>& variables, std::unordered_set<int>& constants) {
	if (auto i = CAST_TO(NotConditionalExpression, e)) {
		visitCondExprHelper(i->condExpr.get(), variables, constants);
	}
	else if (auto i = CAST_TO(BinaryConditionalExpression, e)) {
		visitCondExprHelper(i->lhs.get(), variables, constants);
		visitCondExprHelper(i->rhs.get(), variables, constants);
	}
	else if (auto i = CAST_TO(RelationalExpression, e)) {
		visitExprHelper(i->lhs.get(), variables, constants);
		visitExprHelper(i->rhs.get(), variables, constants);
	}
}

void visitExprHelper(Expression* e, std::unordered_set<std::string>& variables, std::unordered_set<int>& constants) {
	if (auto i = CAST_TO(MathExpression, e)) {
		visitExprHelper(i->lhs.get(), variables, constants);
		visitExprHelper(i->rhs.get(), variables, constants);
	}
	else if (auto i = CAST_TO(Constant, e)) {
		constants.insert(i->value);
	}
	else if (auto i = CAST_TO(Variable, e)) {
		variables.insert(i->name);
	}
}