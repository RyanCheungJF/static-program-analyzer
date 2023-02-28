#pragma once

#include "Expression.h"
#include <iostream>

class Constant : public Expression {
public:
  Const value;

  Constant();
  Constant(Const value);

  void accept(ASTVisitor *visitor) override;
};