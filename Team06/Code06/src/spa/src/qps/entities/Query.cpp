//
// Created by Faruq on 31/1/23.
//

#include "Query.h"

vector<string> Query::evaluate() {
    // I am going to assume here that since the object has been created it means that the variables are correctly
    // instantiated.
    QueryDB queryDb = QueryDB();
    bool isFalseQuery = false;
    for(Relationship relation: relations) {
        // Run an PKB API call for each relationship.
        // Taking the example of select s1 follows(s1, s2)
        vector<vector<string>> response = {{"1", "2"}};
                // PKB.getRelation(relation);
        Table table(relation.getParameters(), response);
        if(response.empty()) {
            isFalseQuery = true;
        }
        // This will remove wild cards and FIXED INT from the table.
        table = table.extractDesignEntities();
        queryDb.insertTable(table);
    }
    if (queryDb.hasParameter(selectParameters[0])) {
        return queryDb.fetch(selectParameters[0]);
    } else {
        if (isFalseQuery) {
            return {};
        } else {
            return {"v", "v"};
            //PKB.getAll(selectParameters[0]);
        }
    }
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
