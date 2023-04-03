#include "Relationship.h"

#include "../syntaxValidator/RelEESyntaxValidator.h"
#include "../syntaxValidator/RelPSESyntaxValidator.h"
#include "../syntaxValidator/RelSSSyntaxValidator.h"

shared_ptr<Relationship> Relationship::makeRelationship(string type, vector<Parameter> params) {
    RelationshipType rType = stringToType(type);
    if (rType == RelationshipType::UNKNOWN) {
        throw SyntaxException();
    }
    shared_ptr<Relationship> relptr = make_shared<Relationship>(Relationship(rType, params));
    shared_ptr<SyntaxValidator<Relationship>> validator = Relationship::typeToSyntaxValidatorMap.at(rType);
    validator->validate(*relptr);
    return relptr;
}

Relationship::Relationship(const Relationship& r) {
    type = r.type;
    params = r.params;
    evalPriority = getPriority();
}

vector<Parameter*> Relationship::getAllUncheckedSynonyms() {
    vector<Parameter*> synonyms;
    for (int i = 0; i < params.size(); i++) {
        if (params.at(i).isUncheckedSynonym()) {
            synonyms.push_back(&params.at(i));
        }
    }
    return synonyms;
}

vector<Parameter> Relationship::getParameters() const {
    return params;
}

vector<ParameterType> Relationship::getParameterTypes() {
    vector<ParameterType> types;
    for (auto param : params) {
        types.push_back(param.getType());
    }

    return types;
}

vector<std::string> Relationship::getParameterValues() {
    vector<std::string> values;
    for (auto param : params) {
        values.push_back(param.getValue());
    }

    return values;
}

bool Relationship::operator==(const Relationship& r) const {
    return type == r.type && params == r.params;
}

bool Relationship::operator>(const Relationship& r) const {
    return evalPriority > r.evalPriority;
}

bool Relationship::operator<(const Relationship& r) const {
    return evalPriority < r.evalPriority;
}

RelationshipType Relationship::getType() const {
    return type;
}

double Relationship::getPriority() {
    int wildcardCounter = 0;
    int stmtCounter = 0;
    int othersCounter = 0; //subtype of stmt or procedure

    for (int i = 0; i < params.size(); i++) {
        if (params.at(i).isFixedValue()) {
            continue; // in PKB, (int, int) calls are the lowest priority
        } else if (params.at(i).isWildcard()) {
            wildcardCounter++;
        } else if (params.at(i).isStmt()) {
            stmtCounter++;
        } else {
            othersCounter++;
        }
    }

    //todo: check with shee hui if this is ok or we have to replace the magic values with AppConstant
    if (transitiveRelationships.find(type) != transitiveRelationships.end()) {
        if (wildcardCounter == 2) {
            return 0;
        } else if (stmtCounter == 2) {
            return 1;
        } else if (othersCounter == 2) {
            return 2;
        } else if (stmtCounter == 1 || othersCounter == 1) {
            return 3;
        }
        return 4;
    } else {
        if (wildcardCounter == 2) {
            return 5;
        } else if (stmtCounter == 2) {
            return 6;
        } else if (othersCounter == 2) {
            return 7;
        } else if (stmtCounter == 1 || othersCounter == 1) {
            return 8;
        }
        return 9;
    }
}

bool Relationship::validateParams() {
    for (int i = 0; i < params.size(); i++) {
        if (Relationship::typeToParameterTypes.at(type)[i].count(params[i].getType()) == 0) {
            return false;
        }
    }
    return true;
}

Relationship::Relationship() {
    type = RelationshipType::UNKNOWN;
    evalPriority = 0;
}

Relationship::Relationship(RelationshipType t, vector<Parameter>& ps) {
    type = t;
    params = ps;
    evalPriority = getPriority();
}

// TODO: update this to throw error if not found
RelationshipType Relationship::stringToType(string s) {
    auto iter = Relationship::stringToTypeMap.find(s);
    if (iter == stringToTypeMap.end()) {
        return RelationshipType::UNKNOWN;
    }
    return iter->second;
}

const unordered_map<string, RelationshipType> Relationship::stringToTypeMap = {
    {AppConstants::FOLLOWS, RelationshipType::FOLLOWS}, {AppConstants::FOLLOWST, RelationshipType::FOLLOWST},
    {AppConstants::PARENT, RelationshipType::PARENT},   {AppConstants::PARENTT, RelationshipType::PARENTT},
    {AppConstants::USES, RelationshipType::USES},       {AppConstants::MODIFIES, RelationshipType::MODIFIES},
    {AppConstants::NEXT, RelationshipType::NEXT},       {AppConstants::NEXTT, RelationshipType::NEXTT},
    {AppConstants::CALLS, RelationshipType::CALLS},     {AppConstants::CALLST, RelationshipType::CALLST},
    {AppConstants::AFFECTS, RelationshipType::AFFECTS}, {AppConstants::AFFECTST, RelationshipType::AFFECTST}};

shared_ptr<SyntaxValidator<Relationship>> relEESynVal = make_shared<RelEESyntaxValidator>(RelEESyntaxValidator());
shared_ptr<SyntaxValidator<Relationship>> relSSSynVal = make_shared<RelSSSyntaxValidator>(RelSSSyntaxValidator());
shared_ptr<SyntaxValidator<Relationship>> relPSESynVal = make_shared<RelPSESyntaxValidator>(RelPSESyntaxValidator());

const unordered_map<RelationshipType, shared_ptr<SyntaxValidator<Relationship>>>
    Relationship::typeToSyntaxValidatorMap = {
        {RelationshipType::FOLLOWS, relSSSynVal}, {RelationshipType::FOLLOWST, relSSSynVal},
        {RelationshipType::PARENT, relSSSynVal},  {RelationshipType::PARENTT, relSSSynVal},
        {RelationshipType::USES, relPSESynVal},   {RelationshipType::MODIFIES, relPSESynVal},
        {RelationshipType::NEXT, relSSSynVal},    {RelationshipType::NEXTT, relSSSynVal},
        {RelationshipType::CALLS, relEESynVal},   {RelationshipType::CALLST, relEESynVal},
        {RelationshipType::AFFECTS, relSSSynVal}, {RelationshipType::AFFECTST, relSSSynVal},
};

const unordered_set<ParameterType> Relationship::stmtRefs = {
    ParameterType::STMT,   ParameterType::READ,      ParameterType::PRINT,    ParameterType::WHILE, ParameterType::IF,
    ParameterType::ASSIGN, ParameterType::FIXED_INT, ParameterType::WILDCARD, ParameterType::CALL};
const unordered_set<ParameterType> Relationship::entityRefs = {ParameterType::VARIABLE, ParameterType::FIXED_STRING,
                                                               ParameterType::WILDCARD};
const unordered_set<ParameterType> Relationship::procedureRefs = {ParameterType::WILDCARD, ParameterType::PROCEDURE,
                                                                  ParameterType::FIXED_STRING};
const unordered_set<ParameterType> Relationship::procOrStmtRefs = {
    ParameterType::STMT,      ParameterType::READ,        ParameterType::PRINT,     ParameterType::WHILE,
    ParameterType::IF,        ParameterType::ASSIGN,      ParameterType::FIXED_INT, ParameterType::CALL,
    ParameterType::PROCEDURE, ParameterType::FIXED_STRING};

const unordered_map<RelationshipType, vector<unordered_set<ParameterType>>> Relationship::typeToParameterTypes = {
    {RelationshipType::FOLLOWS,
     {
         stmtRefs,
         stmtRefs,
     }},
    {RelationshipType::FOLLOWST,
     {
         stmtRefs,
         stmtRefs,
     }},
    {RelationshipType::PARENT,
     {
         stmtRefs,
         stmtRefs,
     }},
    {RelationshipType::PARENTT,
     {
         stmtRefs,
         stmtRefs,
     }},
    {RelationshipType::USES, {procOrStmtRefs, entityRefs}},
    {RelationshipType::MODIFIES, {procOrStmtRefs, entityRefs}},
    {RelationshipType::NEXT,
     {
         stmtRefs,
         stmtRefs,
     }},
    {RelationshipType::NEXTT,
     {
         stmtRefs,
         stmtRefs,
     }},
    {RelationshipType::CALLS,
     {
         procedureRefs,
         procedureRefs,
     }},
    {RelationshipType::CALLST,
     {
         procedureRefs,
         procedureRefs,
     }},
    {RelationshipType::AFFECTS,
     {
         stmtRefs,
         stmtRefs,
     }},
    {RelationshipType::AFFECTST,
     {
         stmtRefs,
         stmtRefs,
     }},
};

const unordered_set<RelationshipType> Relationship::transitiveRelationships = {
        RelationshipType::AFFECTST, RelationshipType::NEXTT, RelationshipType::CALLST,
        RelationshipType::FOLLOWST, RelationshipType::PARENTT,
};
