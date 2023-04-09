#pragma once

#include <vector>

#include "ASTNode.h"
#include "Procedure.h"

class Program : public ASTNode {
public:
    std::vector<std::unique_ptr<Procedure>> procedureList;

    Program();
    explicit Program(std::vector<std::unique_ptr<Procedure>> procedureList);

    void accept(ASTVisitor* visitor) override;
};