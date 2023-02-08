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
	void recurseStatementHelper(Statement* statement, ASTVisitor* visitor);
public:
	std::unique_ptr<Program> ASTroot;
	WritePKB* writePkb;

	DesignExtractor();
	DesignExtractor(std::unique_ptr<Program>, WritePKB* writePKB);

	void extractRelationships();
	void extractEntities();
};