# include "NextRelationshipStub.h"

NextRelationshipStub::NextRelationshipStub(vector<Parameter> &ps) {
    type = RelationshipType::NEXT;
    params = ps;
}

bool NextRelationshipStub::validateParams() {
    return true;
}

bool NextRelationshipStub::validateSyntax(vector<Parameter> &) {
    return true;
}

