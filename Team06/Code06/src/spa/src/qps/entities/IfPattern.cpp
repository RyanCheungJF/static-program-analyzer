#include "IfPattern.h"

IfPattern::IfPattern(Parameter ps, Parameter er, string p, Parameter ip) {
    patternSyn = ps;
    entRef = er;
    pattern = p;
    ifsParam = ip;
}

bool IfPattern::validateParams() {
    return Pattern::validateParams() && pattern == "_" && ifsParam.getType() == ParameterType::WILDCARD;
}