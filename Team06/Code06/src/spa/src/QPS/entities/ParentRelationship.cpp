#include "ParentRelationship.h"

ParentRelationship::ParentRelationship(vector<Parameter>& ps) {
    if (!validateSyntaxStmtStmt(ps)) {
        throw SyntaxException();
    }
    type = RelationshipType::PARENT;
    params = ps;
}

bool ParentRelationship::validateParams() {
    if (!Parameter::isStatementRef(params[0])) {
        return false;
    }
    if (!Parameter::isStatementRef(params[1])) {
        return false;
    }
    return true;
}
