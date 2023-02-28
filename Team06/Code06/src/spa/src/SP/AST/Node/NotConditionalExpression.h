#pragma once

#include "ASTNode.h"
#include "ConditionalExpression.h"

class NotConditionalExpression : public ConditionalExpression {
public:
  Operator op = "!";
  std::unique_ptr<ConditionalExpression> condExpr;

  NotConditionalExpression();
  NotConditionalExpression(std::unique_ptr<ConditionalExpression> condExpr);

  void accept(ASTVisitor *visitor) override;
};