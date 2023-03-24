#include "Comparison.h"

Comparison::Comparison() {}

Comparison::Comparison(ComparisonOperator o, Parameter leftP, Parameter rightP) {
    op = o;
    leftParam = leftP;
    rightParam = rightP;
}

Parameter Comparison::getLeftParam() {
    return leftParam;
}

Parameter Comparison::getRightParam() {
    return rightParam;
}

ComparisonOperator Comparison::getOperator() {
    return op;
}
