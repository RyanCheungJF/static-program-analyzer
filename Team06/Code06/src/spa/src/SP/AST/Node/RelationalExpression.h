#pragma once

#include <vector>

#include "ConditionalExpression.h"
#include "Expression.h"

class RelationalExpression : public ConditionalExpression {
public:
    Operator relationalOperator;
    std::unique_ptr<Expression> lhs;
    std::unique_ptr<Expression> rhs;

    RelationalExpression();
    RelationalExpression(Operator relationalOperator, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);

    void accept(ASTVisitor* visitor) override;
};