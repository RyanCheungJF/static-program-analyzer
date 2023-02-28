#pragma once

#include "AST/Visitor/FollowsExtractorVisitor.h"
#include "AST/Visitor/ProcedureExtractorVisitor.h"
#include "AST/Visitor/StatementExtractorVisitor.h"
#include "AST/Visitor/EntRefExtractorVisitor.h"
#include "AST/Visitor/ParentExtractorVisitor.h"
#include "AST/Visitor/ASTVisitorUtils.h"
#include "../PKB/WritePKB.h"
#include "../PKB/ReadPKB.h"

class DesignExtractor {
private:
	std::unique_ptr<Program> ASTroot;
	WritePKB* writePkb;
	ReadPKB* readPkb;

	void extractInfo();
	void extractCFG();
public:
	DesignExtractor();
	DesignExtractor(std::unique_ptr<Program>, WritePKB* writePKB, ReadPKB* readPKB);

	void populatePKB();
};