#include "Variable.h"

Variable::Variable() {}

Variable::Variable(Ent name) {
    this->name = name;
}

void Variable::accept(ASTVisitor* visitor) {
    visitor->visitExpression(this);
};
