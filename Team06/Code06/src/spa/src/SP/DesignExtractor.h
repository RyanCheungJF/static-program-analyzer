#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "AST/Node/Program.h"
#include "AST/Visitor/FollowsExtractorVisitor.h"
#include "AST/Visitor/ProcedureExtractorVisitor.h"
#include "AST/Visitor/StatementExtractorVisitor.h"
#include "AST/Visitor/EntRefExtractorVisitor.h"
#include "AST/Visitor/ParentExtractorVisitor.h"
#include "../PKB/WritePKB.h"

class DesignExtractor {
private:
	std::unique_ptr<Program> ASTroot;
	WritePKB* writePkb;

	void recurseStatementHelper(Statement* statement, ASTVisitor* visitor);
	void extractRelationships();
	void extractEntities();
public:
	DesignExtractor();
	DesignExtractor(std::unique_ptr<Program>, WritePKB* writePKB);

	void populatePKB();
};