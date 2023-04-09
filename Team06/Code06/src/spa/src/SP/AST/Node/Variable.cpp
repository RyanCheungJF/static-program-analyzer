#include "Variable.h"

Variable::Variable() : name() {}

Variable::Variable(Ent name) : name(std::move(name)) {}