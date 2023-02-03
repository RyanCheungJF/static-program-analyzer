#pragma once

#include "ASTNode.h"

typedef int StmtNum;

class Statement : public ASTNode {
public:
    void accept(ASTVisitor* visitor) const override;
    StmtNum lineNumber;

};