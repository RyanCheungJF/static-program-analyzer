#include "DeclarationParser.h"

using namespace std;

vector<Parameter> declarationToParameter(string s) {
    // The string should look something like "stmt s1, s2" at this point
    vector<Parameter> paramList;
    if (isDesignEntity(s)) {
        string designEntity, synonyms;
        tie(designEntity, synonyms) = extractDesignEntity(s);
        vector<string> synonymList = stringToWordListByDelimiter(synonyms, ",");
        for (const string& synonym : synonymList) {
            if (!isSynonym(synonym)) {
                throw SyntaxException();
            }
            Parameter p = Parameter::makeParameter(synonym, designEntity);
            paramList.push_back(p);
        }
    }
    else {
        // is not a design entity
        throw SyntaxException();
    }
    return paramList;
}

VariableStore parseDeclarations(vector<string> declarations) {
    // The string should look something like "stmt s1, s2; call cl; assign a1,
    // a2;" at this point
    VariableStore store = VariableStore();
    for (string declaration : declarations) {
        // extract each declaration and insert it to the store.
        vector<Parameter> paramList = declarationToParameter(declaration);
        for (Parameter parameter : paramList) {
            store.insertVariable(parameter);
        }
    }
    return store;
}