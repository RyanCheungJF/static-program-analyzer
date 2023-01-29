#pragma once

#include <string>
#include "ASTNode.h"
#include "StatementList.h"
//#include "../Visitor/ASTVisitor.h"

class Procedure : public ASTNode {
public:
	std::string procedureName;
	std::unique_ptr<StatementList> statementList;

	Procedure();

	Procedure(std::string procedureName);

	void accept(ASTVisitor* visitor) const override;
};