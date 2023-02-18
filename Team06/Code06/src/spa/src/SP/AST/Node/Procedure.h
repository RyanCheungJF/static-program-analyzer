#pragma once

#include "ASTNode.h"
#include "StatementList.h"

class Procedure : public ASTNode {
public:

    std::string procedureName;
	std::unique_ptr<StatementList> statementList;

	Procedure();
	Procedure(std::string procedureName, std::unique_ptr<StatementList> statementList);

	void accept(ASTVisitor* visitor) override;
};