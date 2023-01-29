#pragma once

#include <string>
#include <vector>
#include <memory>
#include "ASTNode.h"
#include "Statement.h"
#include "../Visitor/ASTVisitor.h"

class StatementList : public ASTNode {
public:
	std::vector<std::unique_ptr<Statement>> statements;

	StatementList() = default;

	void accept(ASTVisitor* visitor) const override;
};