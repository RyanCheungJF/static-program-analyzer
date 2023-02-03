#pragma once

#include <memory>
#include <string>
#include <vector>
#include "ASTNode.h"
#include "Procedure.h"

class Program : public ASTNode {
public:
	std::vector<std::unique_ptr<Procedure>> procedureList;

	void accept(ASTVisitor* visitor) const override;
};