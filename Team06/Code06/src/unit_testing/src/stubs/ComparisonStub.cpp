#include "ComparisonStub.h"

ComparisonStub::ComparisonStub() {}

ComparisonStub::ComparisonStub(ComparisonOperator o, Parameter leftP, Parameter rightP) {
    op = o;
    leftParam = leftP;
    rightParam = rightP;
}

Parameter ComparisonStub::getLeftParam() {
    return leftParam;
}

Parameter ComparisonStub::getRightParam() {
    return rightParam;
}

ComparisonOperator ComparisonStub::getOperator() {
    return op;
}
