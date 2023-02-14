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
    FollowsExtractorVisitor followsExtractor = FollowsExtractorVisitor(writePkb);
    ParentExtractorVisitor parentExtractor = ParentExtractorVisitor(writePkb);
    StatementExtractorVisitor statementExtractor = StatementExtractorVisitor(writePkb);
    ProcedureExtractorVisitor procedureExtractor = ProcedureExtractorVisitor(writePkb);
    EntRefExtractorVisitor entRefExtractor = EntRefExtractorVisitor(writePkb);
	std::vector<ASTVisitor*> visitors{ &followsExtractor,
									   &parentExtractor,
									   &statementExtractor,
									   &procedureExtractor,
									   &entRefExtractor };

	for (auto& visitor : visitors) {
		for (const auto& procedure : ASTroot->procedureList) {
			procedure->statementList->accept(visitor);
			for (const auto& statement : procedure->statementList->statements) {
				statement->accept(visitor);
				if (isContainerStatement(statement.get())) {
					std::cout << "IS a container " << statement.get()->statementNumber << std::endl;
					recurseStatementHelper(statement.get(), visitor);
				}
				std::cout << "Not a container " << statement.get()->statementNumber << std::endl;
			}
		}
	}
}