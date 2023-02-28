#pragma once

#include "ASTNode.h"
#include "Procedure.h"
#include <vector>

class Program : public ASTNode {
public:
  std::vector<std::unique_ptr<Procedure>> procedureList;

  Program();
  Program(std::vector<std::unique_ptr<Procedure>> procedureList);

  void accept(ASTVisitor *visitor) override;
};