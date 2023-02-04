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

	ProcedureVisitor procedureVisitor;

	for (int i = 0; i < this->ASTroot->procedureList.size(); i++) {
		procedureVisitor.visitProcedure(this->ASTroot->procedureList[i].get());
	}
}

