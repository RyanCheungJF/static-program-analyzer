#include "NextTRelationship.h"

NextTRelationship::NextTRelationship(vector<Parameter>& ps) {
    if (!validateSyntaxStmtStmt(ps)) {
        throw SyntaxException();
    }
    type = RelationshipType::NEXTT;
    params = ps;
}

bool NextTRelationship::validateParams() {
    if (!Parameter::isStatementRef(params[0])) {
        return false;
    }
    if (!Parameter::isStatementRef(params[1])) {
        return false;
    }
    return true;
}
