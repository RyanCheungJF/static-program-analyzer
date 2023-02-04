#include "DesignExtractor.h"

DesignExtractor::DesignExtractor() : ASTroot() {}

DesignExtractor::DesignExtractor(std::unique_ptr<Program> root) {
	ASTroot = std::move(root);
}

void DesignExtractor::extractRelationships(Program* program) {
	
	// starts from root to extract out relationships
	
};

void DesignExtractor::extractEntities() {
	//StatementExtractorVisitor statementVisitor;
}