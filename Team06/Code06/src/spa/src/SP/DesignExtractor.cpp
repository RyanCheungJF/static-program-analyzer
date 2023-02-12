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
	std::vector<ASTVisitor*> visitors{ &FollowsExtractorVisitor(writePkb),
									   &ParentExtractorVisitor(writePkb) };

	for (auto& visitor : visitors) {
		for (const auto& procedure : ASTroot->procedureList) {
			procedure->statementList->accept(visitor);
			for (const auto& statement : procedure->statementList->statements) {
				statement->accept(visitor);
				if (auto i = CAST_TO(IfStatement, statement.get()) || CAST_TO(WhileStatement, statement.get())) {
					recurseStatementHelper(statement.get(), visitor);
				}
			}
		}
	}
}

void DesignExtractor::extractEntities() {
	std::vector<ASTVisitor*> visitors{ &StatementExtractorVisitor(writePkb), 
									   &ProcedureExtractorVisitor(writePkb), 
									   &EntRefExtractorVisitor(writePkb) };

	for (auto& visitor : visitors) {
		for (const auto& procedure : ASTroot->procedureList) {
			procedure->accept(visitor);
			for (const auto& statement : procedure->statementList->statements) {
				statement->accept(visitor);
				if (auto i = CAST_TO(IfStatement, statement.get()) || CAST_TO(WhileStatement, statement.get())) {
					recurseStatementHelper(statement.get(), visitor);
				}
			}
		}
	}
}