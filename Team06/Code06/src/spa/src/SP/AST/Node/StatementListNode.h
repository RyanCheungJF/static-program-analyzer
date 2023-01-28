#pragma once

#include <memory>
#include <string>
#include <vector>
#include "ASTNode.h"
#include "StatementNode.h"
#include "../Visitor/ASTVisitor.h"

class StatementListNode : public ASTNode, public std::enable_shared_from_this<StatementListNode> {
private:
	vector<std::shared_ptr<StatementNode>> statementList = {};
public:
	StatementListNode();

	void accept(std::shared_ptr<ASTVisitor> visitor);
};