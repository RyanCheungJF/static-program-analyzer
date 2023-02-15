#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
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
public:
	DesignExtractor();
	DesignExtractor(std::unique_ptr<Program>, WritePKB* writePKB, ReadPKB* readPKB);

	void populatePKB();
};