#pragma once

#include <memory>
#include <string>
#include <vector>
#include "ASTNode.h"
#include "Procedure.h"
#include "../Visitor/ASTVisitor.h"

class Program : public ASTNode {
public:
	std::vector<std::shared_ptr<Procedure>> procedureList;
	std::string fileName;

	Program(std::vector<Procedure> procedureList, std::string fileName);

	void accept(ASTVisitor visitor);
};