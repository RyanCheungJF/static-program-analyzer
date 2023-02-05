#pragma once

#include "ASTNode.h"

typedef int StmtNum;

class Statement : public ASTNode {
public:
    StmtNum statementNumber;
};