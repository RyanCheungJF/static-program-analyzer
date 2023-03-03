//# pragma once
#include "../../Code06/src/spa/src/qps/entities/Relationship.h"
#include "../../../src/spa/src/utils/AppConstants.h"


using namespace std;

class NextRelationshipStub : public Relationship {
public:
    NextRelationshipStub(vector<Parameter>& ps);
    bool validateParams();
private:
    bool validateSyntax(vector<Parameter>&);
};
