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

void visitCondExprHelper(ConditionalExpression* e, std::unordered_set<Ent>& variables, std::unordered_set<int>& constants) {
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

void visitExprHelper(Expression* e, std::unordered_set<Ent>& variables, std::unordered_set<int>& constants) {
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

void recurseStatementHelper(Statement* recurseStmt, ASTVisitor* visitor) {
	if (auto i = CAST_TO(IfStatement, recurseStmt)) {
		i->thenStmtList->accept(visitor);
		for (const auto& statement : i->thenStmtList->statements) {
			statement->accept(visitor);
			if (auto i = CAST_TO(IfStatement, statement.get()) || CAST_TO(WhileStatement, statement.get())) {
				recurseStatementHelper(statement.get(), visitor);
			}
		}
		i->elseStmtList->accept(visitor);
		for (const auto& statement : i->elseStmtList->statements) {
			statement->accept(visitor);
			if (auto i = CAST_TO(IfStatement, statement.get()) || CAST_TO(WhileStatement, statement.get())) {
				recurseStatementHelper(statement.get(), visitor);
			}
		}
	}
	else if (auto i = CAST_TO(WhileStatement, recurseStmt)) {
		i->stmtList->accept(visitor);
		for (const auto& statement : i->stmtList->statements) {
			statement->accept(visitor);
			if (auto i = CAST_TO(IfStatement, statement.get()) || CAST_TO(WhileStatement, statement.get())) {
				recurseStatementHelper(statement.get(), visitor);
			}
		}
	}
}

bool isContainerStatement(Statement* statement) {
	return CAST_TO(IfStatement, statement) || CAST_TO(WhileStatement, statement);
}

void populateRemainingTables(WritePKB* writePKB, ReadPKB* readPKB) {
	populateUsesModifies(writePKB, readPKB);
}

void populateUsesModifies(WritePKB* writePKB, ReadPKB* readPKB) {
	// First handle the call statements
	auto callStatements = readPKB->getCallStatements();
	for (auto i : callStatements) {
		/* Could be possible I handled the call statement in the recursion, so I check if there's anything.
		   If one of them is empty, it means I handled it in the recursion. Only if both are empty, means I might not
		   have handled it.*/
		if (readPKB->getUsesS(i.first).empty() && readPKB->getModifiesS(i.first).empty()) {
			handleCallStmt(writePKB, readPKB, i);
		}
	}

	// Now handle the container statement (if & while)
	auto containerStatements = readPKB->getWhileStatementNumbers();
	containerStatements.merge(readPKB->getIfStatementNumbers());
	for (StmtNum i : containerStatements) {
		auto usesVariables = readPKB->getUsesS(i);
		auto modifiesVariables = readPKB->getModifiesS(i);
		auto containedStatements = readPKB->getContainedStatements(i);
		for (StmtNum j : containedStatements) {
			usesVariables.merge(readPKB->getUsesS(j));
			modifiesVariables.merge(readPKB->getModifiesS(j));
		}
		writePKB->setUsesS(i, usesVariables);
		writePKB->setModifiesS(i, modifiesVariables);
	}

	// Lastly handle the procedures
	auto procedureNames = readPKB->getAllProcedureNames();
	for (ProcName p : procedureNames) {
		auto procedureStmtNum = readPKB->getProcedureStatementNumbers(p);
		std::unordered_set<Ent> usesVariables;
		std::unordered_set<Ent> modifiesVariables;
		for (StmtNum s : procedureStmtNum) {
			usesVariables.merge(readPKB->getUsesS(s));
			modifiesVariables.merge(readPKB->getModifiesS(s));
		}
		writePKB->setUsesP(p, usesVariables);
		writePKB->setModifiesP(p, modifiesVariables);
	}
}

std::vector<std::unordered_set<Ent>> handleCallStmt(WritePKB* writePKB, ReadPKB* readPKB, std::pair<StmtNum, ProcName> callStmt) {
	auto procedureStmtNum = readPKB->getProcedureStatementNumbers(callStmt.second);
	std::unordered_set<Ent> currUsesVariables;
	std::unordered_set<Ent> currModifiesVariables;
	for (StmtNum sn : procedureStmtNum) {
		if (readPKB->checkStatement(AppConstants::CALL, sn)) {
			std::unordered_set<Ent> moreUsesVariables;
			if (readPKB->getUsesS(readPKB->getCallStmt(sn).first).empty()) {
				moreUsesVariables = handleCallStmt(writePKB, readPKB, readPKB->getCallStmt(sn))[0];
			} else {
				// If I handled the call statement before, just read from it.
				moreUsesVariables = readPKB->getUsesS(readPKB->getCallStmt(sn).first);
			}
			currUsesVariables.merge(moreUsesVariables);

			std::unordered_set<Ent> moreModifiesVariables;
			if (readPKB->getModifiesS(readPKB->getCallStmt(sn).first).empty()) {
				moreModifiesVariables = handleCallStmt(writePKB, readPKB, readPKB->getCallStmt(sn))[1];
			}
			else {
				// If I handled the call statement before, just read from it.
				moreModifiesVariables = readPKB->getModifiesS(readPKB->getCallStmt(sn).first);
			}
			currModifiesVariables.merge(moreModifiesVariables);
		}
		else {
			currUsesVariables.merge(readPKB->getUsesS(sn));
			currModifiesVariables.merge(readPKB->getModifiesS(sn));
		}
	}
	writePKB->setUsesS(callStmt.first, currUsesVariables);
	writePKB->setModifiesS(callStmt.first, currModifiesVariables);
	return std::vector{ currUsesVariables, currModifiesVariables };
}