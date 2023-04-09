#pragma once

#include <iostream>

#include "Expression.h"

class Constant : public Expression {
public:
    Const value;

    Constant();
    explicit Constant(Const value);
};