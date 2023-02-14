//
// Created by Faruq on 31/1/23.
//

#include "Query.h"

vector<string> Query::evaluate(ReadPKB& readPKB) {
    // I am going to assume here that since the object has been created it means that the variables are correctly
    // instantiated.
    QueryDB queryDb = QueryDB();
    bool isFalseQuery = false;
    for(Relationship relation: relations) {
        // Run an PKB API call for each relationship.
        // Taking the example of select s1 follows(s1, s2)
        vector<vector<string>> response = readPKB.findRelationship(relation);
        Table table(relation.getParameters(), response);
        if(response.empty()) {
            isFalseQuery = true;
        }
        // This will remove wild cards and FIXED INT from the table.
        table = table.extractDesignEntities();
        queryDb.insertTable(table);
    }

    for(Pattern pattern: patterns) {
        // Run an PKB API call for each relationship.
        // Taking the example of select s1 follows(s1, s2)
        vector<vector<string>> response = readPKB.findPattern(pattern);
        Parameter synAssign = pattern.getSynAssign();
        Parameter *entRef = pattern.getEntRef();
        vector<Parameter> headers{ synAssign, *entRef };
        Table table(headers, response);
        if(response.empty()) {
            isFalseQuery = true;
        }
        // This will remove wild cards and FIXED INT from the table.
        table = table.extractDesignEntities();
        queryDb.insertTable(table);
    }
    if (isFalseQuery) {
        return {};
    if (queryDb.hasParameter(selectParameters[0])) {
        return queryDb.fetch(selectParameters[0]);
    } else {
        vector<string> res = readPKB.findDesignEntities(selectParameters[0]);
        return res;
    }
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
