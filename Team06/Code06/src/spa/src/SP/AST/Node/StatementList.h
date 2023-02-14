#pragma once

#include <string>
#include <vector>
#include <memory>
#include "ASTNode.h"
#include "Statement.h"

class StatementList : public ASTNode {
public:
	std::vector<std::unique_ptr<Statement>> statements;

	StatementList();
	StatementList(std::vector<std::unique_ptr<Statement>> statements);

	void accept(ASTVisitor* visitor) override;
};