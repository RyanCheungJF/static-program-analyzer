#include "RelEESyntaxValidator.h"

bool RelEESyntaxValidator::validate(Relationship& rel) {
    vector<Parameter> ps = rel.getParameters();
    if (ps.size() != 2) {
        throw SyntaxException();
    }
    if (!ps[0].isSyntacticEntityRef()) {
        throw SyntaxException();
    }
    if (!ps[1].isSyntacticEntityRef()) {
        throw SyntaxException();
    }
    return true;
}

RelEESyntaxValidator::RelEESyntaxValidator() {}
