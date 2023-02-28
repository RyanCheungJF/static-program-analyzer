#pragma once

#include "ASTNode.h"
#include "Statement.h"
#include <vector>

class StatementList : public ASTNode {
public:
  std::vector<std::unique_ptr<Statement>> statements;

  StatementList();
  StatementList(std::vector<std::unique_ptr<Statement>> statements);

  void accept(ASTVisitor *visitor) override;
};