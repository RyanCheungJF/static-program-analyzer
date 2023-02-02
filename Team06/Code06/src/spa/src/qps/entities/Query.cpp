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
}

Query::Query(const vector<Parameter>& ps, const vector<Relationship>& rs)
{
    relations = rs;
    selectParameters = ps;
}
