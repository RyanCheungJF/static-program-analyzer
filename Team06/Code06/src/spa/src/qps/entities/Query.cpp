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
}

Query::Query(const vector<Parameter>& ps, const vector<Relationship>& rs)
{
    relations = rs;
    selectParameters = ps;
    string s;
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
    return synonyms;
}
