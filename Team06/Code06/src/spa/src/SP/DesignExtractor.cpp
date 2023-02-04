#include "DesignExtractor.h"

DesignExtractor::DesignExtractor() : ASTroot() {}

DesignExtractor::DesignExtractor(std::unique_ptr<Program> root) {
	ASTroot = std::move(root);
}

void DesignExtractor::extractRelationships(Program* program) {
	FollowsExtractor followsExtractor;
	// starts from root to extract out relationships
	program->accept(&followsExtractor);
};

void DesignExtractor::extractEntities() {
	StatementExtractorVisitor statementVisitor;
	for (const auto& procedure : ASTroot->procedureList) {
		procedure->accept(&statementVisitor);
		for (const auto& statement : procedure->statementList->statements) {
			statement->accept(&statementVisitor);
			if (auto i = dynamic_cast<IfStatement*>(statement.get()) || dynamic_cast<WhileStatement*>(statement.get())) {
				recurseStatementHelper(statement.get(), &statementVisitor);
			}
		}
	}
}

void DesignExtractor::recurseStatementHelper(Statement* recurseStmt, StatementExtractorVisitor* statementVisitor) {
	if (auto i = dynamic_cast<IfStatement*>(recurseStmt)) {
		for (const auto& statement : i->thenStmtList->statements) {
			statement->accept(statementVisitor);
			if (auto i = dynamic_cast<IfStatement*>(statement.get()) || dynamic_cast<WhileStatement*>(statement.get())) {
				recurseStatementHelper(statement.get(), statementVisitor);
			}
		}
		for (const auto& statement : i->elseStmtList->statements) {
			statement->accept(statementVisitor);
			if (auto i = dynamic_cast<IfStatement*>(statement.get()) || dynamic_cast<WhileStatement*>(statement.get())) {
				recurseStatementHelper(statement.get(), statementVisitor);
			}
		}
	}
	else if (auto i = dynamic_cast<WhileStatement*>(recurseStmt)) {
		for (const auto& statement : i->stmtList->statements) {
			statement->accept(statementVisitor);
			if (auto i = dynamic_cast<IfStatement*>(statement.get()) || dynamic_cast<WhileStatement*>(statement.get())) {
				recurseStatementHelper(statement.get(), statementVisitor);
			}
		}
	}
}
