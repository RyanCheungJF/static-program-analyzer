//
// Created by Faruq on 31/1/23.
//

#include "QPSParser.h"

QPSParser::QPSParser() {
    // Do something lmao idek
}

vector<Query> QPSParser::parse(string qpsQuery) {
    // split the code
    vector<string> queryStatements = splitQuery(qpsQuery);
    SelectQueryParser selectQueryParser;
    vector<Query> queryVec;
    VariableStore vStore;
    vector<string> declarations;
    for (string queryStatement:queryStatements) {
        if (isDeclaration(queryStatement)) {
            declarations.push_back(queryStatement);
        } else {
            Query query = selectQueryParser.parse(queryStatement);
            queryVec.push_back(query);
        }
    }
    vStore = parseDeclarations(declarations);
    // need to do it this way cuz dealing with pointer
    for (int i = 0; i < queryVec.size(); i++) {
        checkSynonyms(&queryVec.at(i), vStore);
    }

    return queryVec;
}

vector<string> QPSParser::splitQuery(string qpsQuery) {
    string delimiter = ";";
    vector<string> clauses;
    int start = 0;
    while (start != qpsQuery.size()) {
        string clause;
        tie(clause, start) = extractSubStringUntilDelimiter(qpsQuery, start, delimiter);
        clause = trim(clause);
        clauses.push_back(clause);
    }
    return clauses;
}

void QPSParser::checkSynonyms(Query* query, VariableStore varStore)
{
    vector<Parameter*> synPs = query->getAllUncheckedSynonyms();
    for (Parameter* synP : synPs) {
        if (!varStore.updateSynonym(synP)) {
            throw Exception();
        }
    }
    vector<Parameter> synAssigns = query->getAllSynAssigns();
    for (Parameter p : synAssigns) {
        if (!varStore.hasVariable(p)) {
            throw Exception();
        }
        if (!(varStore.getType(p) == ParameterType::ASSIGN)) {
            throw Exception();
        }
    }
}
