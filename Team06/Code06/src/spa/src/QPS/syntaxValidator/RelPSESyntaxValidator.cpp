#include "RelPSESyntaxValidator.h"

bool RelPSESyntaxValidator::validate(Relationship& rel) {
    vector<Parameter> ps = rel.getParameters();
    if (ps.size() != 2) {
        throw SyntaxException();
    }
    if (!Parameter::isSyntacticStatementRef(ps[0]) && !Parameter::isProcedure(ps[0])) {
        throw SyntaxException();
    }
    if (!Parameter::isSyntacticEntityRef(ps[1])) {
        throw SyntaxException();
    }
    return true;
}

RelPSESyntaxValidator::RelPSESyntaxValidator() {}
