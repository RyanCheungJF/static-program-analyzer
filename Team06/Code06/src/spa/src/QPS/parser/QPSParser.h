//
// Created by Faruq on 31/1/23.
//

#ifndef SPA_QPSPARSER_H
#define SPA_QPSPARSER_H
#include "DeclarationParser.h"
#include "ParserUtil.h"
#include "QPS/entities/Query.h"
#include "SelectQueryParser.h"
#include "exceptions/SemanticException.h"
using namespace std;

class QPSParser {
public:
    QPSParser();
    vector<Query> parse(string);
    vector<string> splitQuery(string);
    void checkSynonyms(Query*, VariableStore);
};

#endif // SPA_QPSPARSER_H
