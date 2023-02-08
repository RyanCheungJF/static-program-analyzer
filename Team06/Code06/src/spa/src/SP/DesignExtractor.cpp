#include "DesignExtractor.h"

DesignExtractor::DesignExtractor() : ASTroot() {}

DesignExtractor::DesignExtractor(std::unique_ptr<Program> root, WritePKB* writePKB) {
	ASTroot = std::move(root);
	writePkb = writePKB;
}

void DesignExtractor::extractRelationships() {
	FollowsExtractor followsExtractor(writePkb);
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
	EntRefExtractorVisitor entRefVisitor(writePkb);

	//std::vector<ASTVisitor> { statementVisitor, procedureVisitor };

	for (const auto& procedure : ASTroot->procedureList) {
		procedure->accept(&statementVisitor);
		procedure->accept(&procedureVisitor);
		for (const auto& statement : procedure->statementList->statements) {
			statement->accept(&statementVisitor);
			statement->accept(&entRefVisitor);
			if (auto i = dynamic_cast<IfStatement*>(statement.get()) || dynamic_cast<WhileStatement*>(statement.get())) {
				recurseStatementHelper(statement.get(), &statementVisitor);
				recurseStatementHelper(statement.get(), &entRefVisitor);
			}
		}
	}
}


void DesignExtractor::recurseStatementHelper(Statement* recurseStmt, ASTVisitor* visitor) {
	if (auto i = dynamic_cast<IfStatement*>(recurseStmt)) {
		i->thenStmtList->accept(visitor);
		for (const auto& statement : i->thenStmtList->statements) {
			statement->accept(visitor);
			if (auto i = dynamic_cast<IfStatement*>(statement.get()) || dynamic_cast<WhileStatement*>(statement.get())) {
				recurseStatementHelper(statement.get(), visitor);
			}
		}
		i->elseStmtList->accept(visitor);
		for (const auto& statement : i->elseStmtList->statements) {
			statement->accept(visitor);
			if (auto i = dynamic_cast<IfStatement*>(statement.get()) || dynamic_cast<WhileStatement*>(statement.get())) {
				recurseStatementHelper(statement.get(), visitor);
			}
		}
	}
	else if (auto i = dynamic_cast<WhileStatement*>(recurseStmt)) {
		i->stmtList->accept(visitor);
		for (const auto& statement : i->stmtList->statements) {
			statement->accept(visitor);
			if (auto i = dynamic_cast<IfStatement*>(statement.get()) || dynamic_cast<WhileStatement*>(statement.get())) {
				recurseStatementHelper(statement.get(), visitor);
			}
		}
	}
}