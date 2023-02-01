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
private:
    Relationship r;
    Parameter selectParameter;
};


#endif //SPA_QUERY_H
