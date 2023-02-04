//
// Created by Faruq on 31/1/23.
//

#include "Query.h"

vector<string> Query::process() {
    return {"hello", "world"};
}

Query::Query()
{
}

Query::Query(const Query& q)
{
    relations = q.relations;
    selectParameters = q.selectParameters;
    patterns = q.patterns;
}

Query::Query(const vector<Parameter>& ss, const vector<Relationship>& rs, const vector<Pattern>& ps)
{
    selectParameters = ss;
    relations = rs;
    patterns = ps;
}

vector<Parameter*> Query::getAllUncheckedSynonyms()
{
    vector<Parameter*> synonyms;
    for (int i = 0; i < selectParameters.size(); i++) {
        if (selectParameters.at(i).isUncheckedSynonym()) {
            synonyms.push_back(&selectParameters.at(i));
        }
    }
    for (int i = 0; i < relations.size(); i++) {
        vector<Parameter*> relSyns = relations.at(i).getAllUncheckedSynonyms();
        for (Parameter* paramP : relSyns) {
            synonyms.push_back(paramP);
        }
    }
    for (int i = 0; i < patterns.size(); i++) {
        Parameter* entRef = patterns.at(i).getEntRef();
        if (entRef->getType() != ParameterType::SYNONYM) {
            continue;
        }
        synonyms.push_back(entRef);
    }
    return synonyms;
}

vector<Parameter> Query::getAllSynAssigns()
{
    vector<Parameter> synAssigns;
    for (int i = 0; i < patterns.size(); i++) {
        synAssigns.push_back(patterns.at(i).getSynAssign());
    }
    return synAssigns;
}
