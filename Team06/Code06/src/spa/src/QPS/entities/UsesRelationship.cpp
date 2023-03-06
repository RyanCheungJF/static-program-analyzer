#include "UsesRelationship.h"

UsesRelationship::UsesRelationship(vector<Parameter>& ps) {
    if (!validateSyntaxStmtProcEntity(ps)) {
        throw SyntaxException();
    }
    type = RelationshipType::USES;
    params = ps;
}

bool UsesRelationship::validateParams() {
    if (params[0].getValue() == "_" || (!Parameter::isStatementRef(params[0]) && !Parameter::isProcedure(params[0]))) {
        return false;
    }
    if (!Parameter::isEntityRef(params[1])) {
        return false;
    }
    return true;
}
