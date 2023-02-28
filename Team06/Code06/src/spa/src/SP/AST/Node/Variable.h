#pragma once

#include "Expression.h"
#include <iostream>

class Variable : public Expression {
public:
  Ent name;

  Variable();
  Variable(Ent name);

  void accept(ASTVisitor *visitor) override;
};