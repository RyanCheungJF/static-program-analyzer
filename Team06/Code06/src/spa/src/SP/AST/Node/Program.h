#pragma once

#include <memory>
#include <string>
#include <vector>
#include "ASTNode.h"
#include "Procedure.h"
#include "../Visitor/ASTVisitor.h"

class Program : public ASTNode {
public:
	std::vector<std::unique_ptr<Procedure>> procedureList;
	std::string fileName;

	Program();

	Program(std::string fileName);

	void accept(ASTVisitor* visitor) const override;
};