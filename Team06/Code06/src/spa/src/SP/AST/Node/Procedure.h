#pragma once

#include "ASTNode.h"
#include "StatementList.h"
#include "../Visitor/ASTVisitor.h"

class Procedure : public ASTNode {
public:
	StatementList statementList;

	Procedure(StatementList statementList);

	void accept(ASTVisitor visitor);
};