#include "NextRelationship.h"

NextRelationship::NextRelationship(vector<Parameter>& ps) {
    if (!validateSyntaxStmtStmt(ps)) {
        throw SyntaxException();
    }
    type = RelationshipType::NEXT;
    params = ps;
}

bool NextRelationship::validateParams() {
    if (!Parameter::isStatementRef(params[0])) {
        return false;
    }
    if (!Parameter::isStatementRef(params[1])) {
        return false;
    }
    return true;
}
