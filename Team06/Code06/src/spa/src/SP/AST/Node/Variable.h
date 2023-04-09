#pragma once

#include <iostream>

#include "Expression.h"

class Variable : public Expression {
public:
    Ent name;

    Variable();
    explicit Variable(Ent name);
};