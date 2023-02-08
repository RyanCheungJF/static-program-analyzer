#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "AST/Node/Program.h"
#include "AST/Visitor/FollowsExtractor.h"
#include "AST/Visitor/ProcedureExtractorVisitor.h"
#include "AST/Visitor/StatementExtractorVisitor.h"
#include "AST/Visitor/EntRefExtractorVisitor.h"
#include "../PKB/WritePKB.h"

class DesignExtractor {
public:
	std::unique_ptr<Program> ASTroot;
	WritePKB* writePkb;

	DesignExtractor();
	DesignExtractor(std::unique_ptr<Program>, WritePKB* writePKB);

	void extractRelationships();
	void extractEntities();
	void recurseStatementHelper(Statement* statement, ASTVisitor* visitor);
};