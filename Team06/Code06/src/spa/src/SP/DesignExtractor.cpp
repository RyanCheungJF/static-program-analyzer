#include "DesignExtractor.h"

DesignExtractor::DesignExtractor() : ASTroot() {}

DesignExtractor::DesignExtractor(std::unique_ptr<Program> root, WritePKB* writePKB) {
	ASTroot = std::move(root);
	writePkb = writePKB;
}

void DesignExtractor::populatePKB() {
	extractEntities();
	extractRelationships();
}

void DesignExtractor::extractRelationships() {
	FollowsExtractorVisitor followsExtractor(writePkb);
	ParentExtractorVisitor parentExtractor(writePkb);

	for (const auto& procedure : ASTroot->procedureList) {
		procedure->statementList->accept(&followsExtractor);
		for (const auto& statement : procedure->statementList->statements) {
			statement->accept(&followsExtractor);
			statement->accept(&parentExtractor);
			if (auto i = dynamic_cast<IfStatement*>(statement.get()) || dynamic_cast<WhileStatement*>(statement.get())) {
				recurseStatementHelper(statement.get(), &followsExtractor);
				recurseStatementHelper(statement.get(), &parentExtractor);
			}
		}
	}
}

void DesignExtractor::extractEntities() {
	StatementExtractorVisitor statementVisitor(writePkb);
	ProcedureExtractorVisitor procedureVisitor(writePkb);
	EntRefExtractorVisitor entRefVisitor(writePkb);

	//std::vector<ASTVisitor*> { &statementVisitor, &procedureVisitor };

	for (const auto& procedure : ASTroot->procedureList) {
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
			if (auto i = CAST_TO(IfStatement, statement.get()) || dynamic_cast<WhileStatement*>(statement.get())) {
				recurseStatementHelper(statement.get(), visitor);
			}
		}
	}
}