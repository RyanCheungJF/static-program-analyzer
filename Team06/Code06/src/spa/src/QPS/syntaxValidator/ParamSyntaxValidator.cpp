#include "ParamSyntaxValidator.h"

ParamSyntaxValidator::ParamSyntaxValidator() {}

bool ParamSyntaxValidator::validate(Parameter& p) {
    if (p.getType() == ParameterType::UNKNOWN) {
        throw SyntaxException();
    }
    return true;
}
