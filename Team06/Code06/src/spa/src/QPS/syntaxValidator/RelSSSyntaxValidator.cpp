#include "RelSSSyntaxValidator.h"

bool RelSSSyntaxValidator::validate(Relationship& rel) {
    vector<Parameter> ps = rel.getParameters();
    if (ps.size() != 2) {
        throw SyntaxException();
    }
    if (!Parameter::isSyntacticStatementRef(ps[0])) {
        throw SyntaxException();
    }
    if (!Parameter::isSyntacticStatementRef(ps[1])) {
        throw SyntaxException();
    }
    return true;
}

RelSSSyntaxValidator::RelSSSyntaxValidator() {}
