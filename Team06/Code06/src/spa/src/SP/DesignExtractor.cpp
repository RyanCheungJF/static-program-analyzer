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
	//StatementExtractorVisitor statementVisitor;
}