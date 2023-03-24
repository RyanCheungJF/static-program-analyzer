//
// Created by Faruq on 31/1/23.
//

#ifndef SPA_QUERY_H
#define SPA_QUERY_H
#include <string>
#include <vector>

#include "PKB/ReadPKB.h"
#include "Parameter.h"
#include "Pattern.h"
#include "QueryDB.h"
#include "Relationship.h"
#include "Comparison.h"
#include "Table.h"
#include "exceptions/SyntaxException.h"

using namespace std;

class Query {
public:
    vector<string> evaluate(ReadPKB&);
    Query();
    Query(const Query&);
    Query(vector<Parameter>&, vector<shared_ptr<Relationship>>&, vector<Pattern>&, vector<Comparison>&);
    vector<shared_ptr<Relationship>> relations;
    vector<Parameter> selectParameters;
    vector<Pattern> patterns;
    vector<Comparison> comparisons;
    vector<Parameter*> getAllUncheckedSynonyms();
    bool validateAllParameters();
};

#endif // SPA_QUERY_H
