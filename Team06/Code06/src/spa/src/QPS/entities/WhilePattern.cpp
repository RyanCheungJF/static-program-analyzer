//
// Created by Faruq on 1/3/23.
//

#include "WhilePattern.h"

WhilePattern::WhilePattern(Parameter ps, Parameter er, string p) {
    patternSyn = ps;
    entRef = er;
    pattern = p;
}

bool WhilePattern::validateParams() {
    return Pattern::validateParams() && pattern == "_";
}