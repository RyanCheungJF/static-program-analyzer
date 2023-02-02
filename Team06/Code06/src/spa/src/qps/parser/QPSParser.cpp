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
    SelectQueryParser selectClauseParser = SelectQueryParser();
    vector<Query> queryVec;
    VariableStore vStore = VariableStore();
    for (string queryStatement:queryStatements) {
        if (isSelectClause(queryStatement)) {
            // Assuming return type of the query parser is a Query
            // Currently its a different type. hence the error.
            // queryVec.push_back(selectClauseParser.parse(queryStatement));
        } else {
            // Prolly make this a class method so I have to instantiate a version of this.
            // Not sure if this is needed but this is in the case we have more than one var declaration clauses.
            // Like multiline.
            // vStore.addDeclarations(parseDeclarations(queryStatement));
        }
    }

    // Check Parameters to make sure they are variables that has been declared.
    for (Query query: queryVec) {
        checkParameters(query, vStore);
    }

    return queryVec;
}

void checkParameters(Query q, VariableStore v) {
    1 + 1;
}

vector<string> QPSParser::splitQuery(string qpsQuery) {
    return {"", ""};
}