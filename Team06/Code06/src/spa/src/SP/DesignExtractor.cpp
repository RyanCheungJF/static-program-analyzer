#include "DesignExtractor.h"

DesignExtractor::DesignExtractor() : ASTroot() {}

DesignExtractor::DesignExtractor(std::unique_ptr<Program> root, WritePKB* writePKB, ReadPKB* readPKB) {
	ASTroot = std::move(root);
	writePkb = writePKB;
	readPkb = readPKB;
}

void DesignExtractor::populatePKB() {
	extractInfo();
	populateRemainingTables(writePkb, readPkb);
}

void DesignExtractor::extractInfo() {
	std::vector<ASTVisitor*> visitors{ &FollowsExtractorVisitor(writePkb),
									   &ParentExtractorVisitor(writePkb),
									   &StatementExtractorVisitor(writePkb),
									   &ProcedureExtractorVisitor(writePkb),
									   &EntRefExtractorVisitor(writePkb) };

	for (auto& visitor : visitors) {
		for (const auto& procedure : ASTroot->procedureList) {
			procedure->statementList->accept(visitor);
			for (const auto& statement : procedure->statementList->statements) {
				statement->accept(visitor);
				if (isContainerStatement(statement.get())) {
					recurseStatementHelper(statement.get(), visitor);
				}
			}
		}
	}
}