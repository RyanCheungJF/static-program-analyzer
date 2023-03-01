#include "AssignPattern.h"

AssignPattern::AssignPattern(Parameter ps, Parameter er, string p) {
    patternSyn = ps;
    entRef = er;
    pattern = p;
}

bool AssignPattern::validateParams() {
    return Pattern::validateParams();
}
