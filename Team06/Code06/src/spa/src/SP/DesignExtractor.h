#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "AST/Node/Program.h"
#include "AST/Visitor/FollowsExtractor.h"
#include "AST/Visitor/StatementExtractorVisitor.h"

class DesignExtractor {
public:
	std::unique_ptr<Program> ASTroot;

	DesignExtractor();
	DesignExtractor(std::unique_ptr<Program>);
	
	void extractRelationships(Program* program);
	void extractEntities();
};