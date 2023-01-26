//
// Created by Faruq on 27/1/23.
//

#ifndef SPA_DECLARATIONPARSER_H
#define SPA_DECLARATIONPARSER_H
#include <map>
#include <string>
using namespace std;


class DeclarationParser {

    public:
        map<string, string> parse(string item);
};


#endif //SPA_DECLARATIONPARSER_H
