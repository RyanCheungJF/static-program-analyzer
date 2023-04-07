#ifndef SPA_QPS_RELATIONSHIP_H
#define SPA_QPS_RELATIONSHIP_H
#include <string>
#include <unordered_set>
#include <vector>

#include "../syntaxValidator/SyntaxValidator.h"
#include "./utils/AppConstants.h"
#include "Parameter.h"

using namespace std;

enum class RelationshipType {
    FOLLOWS,
    FOLLOWST,
    PARENT,
    PARENTT,
    USES,
    MODIFIES,
    NEXT,
    NEXTT,
    CALLS,
    CALLST,
    AFFECTS,
    AFFECTST,
    UNKNOWN
};

class Relationship {
public:
    static shared_ptr<Relationship> makeRelationship(string type, vector<Parameter> params);
    bool validateParams();
    RelationshipType type;
    vector<Parameter> params;

    Relationship();
    Relationship(const Relationship&);
    Relationship(RelationshipType, vector<Parameter>&);
    vector<Parameter*> getAllUncheckedSynonyms();
    vector<Parameter> getParameters() const;
    vector<ParameterType> getParameterTypes();
    vector<std::string> getParameterValues();
    RelationshipType getType() const;
    bool isTransitive();
    double getPriority();
    bool operator==(const Relationship&) const;
    bool operator>(const Relationship&) const;
    bool operator<(const Relationship&) const;
    double calcPriority();

private:
    double evalPriority;
    static const unordered_map<string, RelationshipType> stringToTypeMap;
    static const unordered_map<RelationshipType, shared_ptr<SyntaxValidator<Relationship>>> typeToSyntaxValidatorMap;
    static const unordered_map<RelationshipType, vector<unordered_set<ParameterType>>> typeToParameterTypes;
    static const unordered_map<RelationshipType, int> typeToPriority;
    static const unordered_set<ParameterType> stmtRefs;
    static const unordered_set<ParameterType> entityRefs;
    static const unordered_set<ParameterType> procedureRefs;
    static const unordered_set<ParameterType> procOrStmtRefs;
    static RelationshipType stringToType(string);
    static const unordered_set<RelationshipType> transitiveRelationships;
};

template <> struct std::hash<Relationship> {
    std::size_t operator()(Relationship const& rs) const {
        std::size_t h1 = std::hash<RelationshipType>{}(rs.getType());
        std::size_t h2 = std::hash<Parameter>{}(rs.getParameters()[0]);
        std::size_t h3 = std::hash<Parameter>{}(rs.getParameters()[1]);
        return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
    }
};

#endif // !SPA_QPS_RELATIONSHIP_H
