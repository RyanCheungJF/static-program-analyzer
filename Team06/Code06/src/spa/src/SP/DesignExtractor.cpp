#include "DesignExtractor.h"

DesignExtractor::DesignExtractor() : ASTroot() {}

DesignExtractor::DesignExtractor(std::unique_ptr<Program> root, WritePKB* writePKB) {
	ASTroot = std::move(root);
	writePkb = writePKB;
}

void DesignExtractor::extractRelationships() {
	FollowsExtractor followsExtractor;
	for (const auto& procedure : ASTroot->procedureList) {
		procedure->statementList->accept(&followsExtractor);
		for (const auto& statement : procedure->statementList->statements) {
			statement->accept(&followsExtractor);
			if (auto i = dynamic_cast<IfStatement*>(statement.get()) || dynamic_cast<WhileStatement*>(statement.get())) {
				recurseStatementHelper(statement.get(), &followsExtractor);
			}
		}
	}
};

void DesignExtractor::extractEntities() {
	StatementExtractorVisitor statementVisitor(writePkb);
	ProcedureExtractorVisitor procedureVisitor(writePkb);

	//std::vector<ASTVisitor> { statementVisitor, procedureVisitor };

	for (const auto& procedure : ASTroot->procedureList) {
		procedure->accept(&statementVisitor);
		procedure->accept(&procedureVisitor);
		for (const auto& statement : procedure->statementList->statements) {
			statement->accept(&statementVisitor);
			if (auto i = dynamic_cast<IfStatement*>(statement.get()) || dynamic_cast<WhileStatement*>(statement.get())) {
				recurseStatementHelper(statement.get(), &statementVisitor);
			}
		}
	}
}

void DesignExtractor::recurseStatementHelper(Statement* recurseStmt, ASTVisitor* statementVisitor) {
	if (auto i = dynamic_cast<IfStatement*>(recurseStmt)) {
		i->thenStmtList->accept(statementVisitor);
		for (const auto& statement : i->thenStmtList->statements) {
			statement->accept(statementVisitor);
			if (auto i = dynamic_cast<IfStatement*>(statement.get()) || dynamic_cast<WhileStatement*>(statement.get())) {
				recurseStatementHelper(statement.get(), statementVisitor);
			}
		}
		i->elseStmtList->accept(statementVisitor);
		for (const auto& statement : i->elseStmtList->statements) {
			statement->accept(statementVisitor);
			if (auto i = dynamic_cast<IfStatement*>(statement.get()) || dynamic_cast<WhileStatement*>(statement.get())) {
				recurseStatementHelper(statement.get(), statementVisitor);
			}
		}
	}
	else if (auto i = dynamic_cast<WhileStatement*>(recurseStmt)) {
		i->stmtList->accept(statementVisitor);
		for (const auto& statement : i->stmtList->statements) {
			statement->accept(statementVisitor);
			if (auto i = dynamic_cast<IfStatement*>(statement.get()) || dynamic_cast<WhileStatement*>(statement.get())) {
				recurseStatementHelper(statement.get(), statementVisitor);
			}
		}
	}
}