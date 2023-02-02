//
// Created by Faruq on 31/1/23.
//

#ifndef SPA_QUERY_H
#define SPA_QUERY_H
#include <string>
#include "Relationship.h"
#include "Parameter.h"

using namespace std;


class Query {
public:
    vector<string> process();
    Query();
    Query(const Query&);
    Query(const vector<Parameter>&, const vector<Relationship>&);
    vector<Relationship> relations;
    vector<Parameter> selectParameters;
};


#endif //SPA_QUERY_H
