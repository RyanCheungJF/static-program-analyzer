#pragma once

#include <string>
#include <vector>
#include "ASTNode.h"
#include "Statement.h"
#include "../Visitor/ASTVisitor.h"

class StatementList : public ASTNode {
public:
	std::vector<Statement> statementList;

	StatementList(std::vector<Statement> statementList);

	void accept(ASTVisitor visitor);
};