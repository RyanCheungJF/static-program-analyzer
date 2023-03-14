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

RelationshipType Relationship::getType() {
    return type;
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
}

Relationship::Relationship(RelationshipType t, vector<Parameter>& ps) {
    type = t;
    params = ps;
}

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

const unordered_map<RelationshipType, vector<unordered_set<ParameterType>>> Relationship::typeToParameterTypes = {
    {RelationshipType::FOLLOWS,
     {
         {ParameterType::STMT, ParameterType::READ, ParameterType::PRINT, ParameterType::WHILE, ParameterType::IF,
          ParameterType::ASSIGN, ParameterType::FIXED_INT, ParameterType::WILDCARD, ParameterType::CALL},
         {ParameterType::STMT, ParameterType::READ, ParameterType::PRINT, ParameterType::WHILE, ParameterType::IF,
          ParameterType::ASSIGN, ParameterType::FIXED_INT, ParameterType::WILDCARD, ParameterType::CALL},
     }},
    {RelationshipType::FOLLOWST,
     {
         {ParameterType::STMT, ParameterType::READ, ParameterType::PRINT, ParameterType::WHILE, ParameterType::IF,
          ParameterType::ASSIGN, ParameterType::FIXED_INT, ParameterType::WILDCARD, ParameterType::CALL},
         {ParameterType::STMT, ParameterType::READ, ParameterType::PRINT, ParameterType::WHILE, ParameterType::IF,
          ParameterType::ASSIGN, ParameterType::FIXED_INT, ParameterType::WILDCARD, ParameterType::CALL},
     }},
    {RelationshipType::PARENT,
     {
         {ParameterType::STMT, ParameterType::READ, ParameterType::PRINT, ParameterType::WHILE, ParameterType::IF,
          ParameterType::ASSIGN, ParameterType::FIXED_INT, ParameterType::WILDCARD, ParameterType::CALL},
         {ParameterType::STMT, ParameterType::READ, ParameterType::PRINT, ParameterType::WHILE, ParameterType::IF,
          ParameterType::ASSIGN, ParameterType::FIXED_INT, ParameterType::WILDCARD, ParameterType::CALL},
     }},
    {RelationshipType::PARENTT,
     {
         {ParameterType::STMT, ParameterType::READ, ParameterType::PRINT, ParameterType::WHILE, ParameterType::IF,
          ParameterType::ASSIGN, ParameterType::FIXED_INT, ParameterType::WILDCARD, ParameterType::CALL},
         {ParameterType::STMT, ParameterType::READ, ParameterType::PRINT, ParameterType::WHILE, ParameterType::IF,
          ParameterType::ASSIGN, ParameterType::FIXED_INT, ParameterType::WILDCARD, ParameterType::CALL},
     }},
    {RelationshipType::USES,
     {
         {ParameterType::STMT, ParameterType::READ, ParameterType::PRINT, ParameterType::WHILE, ParameterType::IF,
          ParameterType::ASSIGN, ParameterType::FIXED_INT, ParameterType::CALL, ParameterType::PROCEDURE,
          ParameterType::FIXED_STRING},
         {ParameterType::VARIABLE, ParameterType::FIXED_STRING, ParameterType::WILDCARD},
     }},
    {RelationshipType::MODIFIES,
     {
         {ParameterType::STMT, ParameterType::READ, ParameterType::PRINT, ParameterType::WHILE, ParameterType::IF,
          ParameterType::ASSIGN, ParameterType::FIXED_INT, ParameterType::CALL, ParameterType::PROCEDURE,
          ParameterType::FIXED_STRING},
         {ParameterType::VARIABLE, ParameterType::FIXED_STRING, ParameterType::WILDCARD},
     }},
    {RelationshipType::NEXT,
     {
         {ParameterType::STMT, ParameterType::READ, ParameterType::PRINT, ParameterType::WHILE, ParameterType::IF,
          ParameterType::ASSIGN, ParameterType::FIXED_INT, ParameterType::WILDCARD, ParameterType::CALL},
         {ParameterType::STMT, ParameterType::READ, ParameterType::PRINT, ParameterType::WHILE, ParameterType::IF,
          ParameterType::ASSIGN, ParameterType::FIXED_INT, ParameterType::WILDCARD, ParameterType::CALL},
     }},
    {RelationshipType::NEXTT,
     {
         {ParameterType::STMT, ParameterType::READ, ParameterType::PRINT, ParameterType::WHILE, ParameterType::IF,
          ParameterType::ASSIGN, ParameterType::FIXED_INT, ParameterType::WILDCARD, ParameterType::CALL},
         {ParameterType::STMT, ParameterType::READ, ParameterType::PRINT, ParameterType::WHILE, ParameterType::IF,
          ParameterType::ASSIGN, ParameterType::FIXED_INT, ParameterType::WILDCARD, ParameterType::CALL},
     }},
    {RelationshipType::CALLS,
     {
         {ParameterType::WILDCARD, ParameterType::PROCEDURE, ParameterType::FIXED_STRING},
         {ParameterType::WILDCARD, ParameterType::PROCEDURE, ParameterType::FIXED_STRING},
     }},
    {RelationshipType::CALLST,
     {
         {ParameterType::WILDCARD, ParameterType::PROCEDURE, ParameterType::FIXED_STRING},
         {ParameterType::WILDCARD, ParameterType::PROCEDURE, ParameterType::FIXED_STRING},
     }},
    {RelationshipType::AFFECTS,
     {
         {ParameterType::WILDCARD, ParameterType::ASSIGN, ParameterType::FIXED_STRING},
         {ParameterType::WILDCARD, ParameterType::ASSIGN, ParameterType::FIXED_STRING},
     }},
    {RelationshipType::AFFECTST,
     {
         {ParameterType::WILDCARD, ParameterType::ASSIGN, ParameterType::FIXED_STRING},
         {ParameterType::WILDCARD, ParameterType::ASSIGN, ParameterType::FIXED_STRING},
     }},
};
