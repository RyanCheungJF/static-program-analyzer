#pragma once

#include <memory>
#include <string>
#include <vector>
#include "ASTNode.h"
#include "ProcedureNode.h"
#include "../Visitor/ASTVisitor.h"

class ProgramNode : public ASTNode, public std::enable_shared_from_this<ProgramNode> {
private:
	vector<std::shared_ptr<ProcedureNode>> procedureList = {};
	std::string fileName = "";
public:
	ProgramNode();

	void accept(std::shared_ptr<ASTVisitor> visitor);
};