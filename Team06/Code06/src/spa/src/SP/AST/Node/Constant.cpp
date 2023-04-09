#include "Constant.h"

Constant::Constant() : value() {}

Constant::Constant(Const value) : value(std::move(value)) {}