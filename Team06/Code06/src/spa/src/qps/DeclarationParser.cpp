//
// Created by Faruq on 27/1/23.
//

#include "DeclarationParser.h"
#include <stdio.h>
#include <string>
#include <map>

map<string, string> parse(string declarations) {
    // The input to this will be of the form "stmt s1, s2; call c; print p;"
    std::map<std::string, string> variableStore;
    int index = declarations.find(";");
    string currentDeclaration = declarations.substr(0, index);
    string next = "";
    if (index + 2 >= declarations.length()) {
        next = declarations.substr(index + 2, declarations.length());
    }
    while(currentDeclaration != "") {
        pair<string, string> declarationPair = extractDeclaration(currentDeclaration);
        variableStore.insert(declarationPair);
        index = next.find(";");
        if (index == -1) {
            // TODO: kill myself cos i hate C.
        }
        currentDeclaration = next.substr(0, index);
        if (index + 2 >= next.length()) {
            next = next.substr(index + 2, next.length());
        } else {
            next = ""
        }

    }


}