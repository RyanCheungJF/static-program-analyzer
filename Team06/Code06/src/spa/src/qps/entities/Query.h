//
// Created by Faruq on 31/1/23.
//

#ifndef SPA_QUERY_H
#define SPA_QUERY_H
#include <string>
#include <vector>
#include "Relationship.h"
#include "Parameter.h"
#include "QueryDB.h"
#include "Table.h"
#include "Pattern.h"
#include "PKB/ReadPKB.h"

using namespace std;


class Query {
public:
    vector<string> evaluate(ReadPKB&);
    Query();
    Query(const Query&);
    Query(const vector<Parameter>&, const vector<Relationship>&, const vector<Pattern>&);
    vector<Relationship> relations;
    vector<Parameter> selectParameters;
    vector<Pattern> patterns;
    vector<Parameter*> getAllUncheckedSynonyms();
    vector<Parameter> getAllSynAssigns();
};


#endif //SPA_QUERY_H
