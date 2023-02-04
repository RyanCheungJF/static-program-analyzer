#pragma once

#include "ASTNode.h"

typedef int StmtNum;

class Statement : public ASTNode {
public:
    Statement();
    virtual void accept(ASTVisitor* visitor) const override;
    StmtNum lineNumber;

};