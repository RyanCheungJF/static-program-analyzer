#pragma once

#include <memory>
#include "ASTNode.h"
#include "StatementListNode.h"
#include "../Visitor/ASTVisitor.h"

class ProcedureNode : public ASTNode, public std::enable_shared_from_this<ProcedureNode> {
private:
	StatementListNode statementListNode;
public:
	ProcedureNode();

	void accept(std::shared_ptr<ASTVisitor> visitor);
};