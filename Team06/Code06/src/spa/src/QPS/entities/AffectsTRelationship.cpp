#include "AffectsTRelationship.h"

AffectsTRelationship::AffectsTRelationship(vector<Parameter>& ps) {
    if (!validateSyntaxStmtStmt(ps)) {
        throw SyntaxException();
    }
    type = RelationshipType::AFFECTST;
    params = ps;
}

bool AffectsTRelationship::validateParams() {
    if (params[0].getType() != ParameterType::ASSIGN && !(Parameter::isFixedIntOrWildCard(params[0]))) {
        return false;
    }
    if (params[1].getType() != ParameterType::ASSIGN && !(Parameter::isFixedIntOrWildCard(params[1]))) {
        return false;
    }
    return true;
}