#pragma once

#include "ASTNode.h"

class Statement : public ASTNode {
public:
  StmtNum statementNumber;
};