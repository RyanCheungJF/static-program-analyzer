#include "RelPSESyntaxValidator.h"

bool RelPSESyntaxValidator::validate(Relationship& rel) {
    vector<Parameter> ps = rel.getParameters();
    if (ps.size() != 2) {
        throw SyntaxException();
    }
    if (!ps[0].isSyntacticStatementRef() && !ps[0].isProcedure()) {
        throw SyntaxException();
    }
    if (!ps[1].isSyntacticEntityRef()) {
        throw SyntaxException();
    }
    return true;
}

RelPSESyntaxValidator::RelPSESyntaxValidator() {}
