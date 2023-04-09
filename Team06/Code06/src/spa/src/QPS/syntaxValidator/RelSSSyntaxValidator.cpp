#include "RelSSSyntaxValidator.h"

bool RelSSSyntaxValidator::validate(Relationship& rel) {
    vector<Parameter> ps = rel.getParameters();
    if (ps.size() != 2) {
        throw SyntaxException();
    }
    if (!ps[0].isSyntacticStatementRef()) {
        throw SyntaxException();
    }
    if (!ps[1].isSyntacticStatementRef()) {
        throw SyntaxException();
    }
    return true;
}

RelSSSyntaxValidator::RelSSSyntaxValidator() {}
