////
//// Created by Faruq on 26/1/23.
////
//#include<stdio.h>
//#include <iostream>
//#include <string>
//#include <vector>
//#include <map>
// I AM LEAVING THIS HERE BECAUSE IT HAS A BIT OF BOILERPLATE CODE THAT WE PLANNED
//using namespace std;
//
//int QueryParse () {
//
//    return 0;
//}
//
//class Variable {
//    public:
//        virtual int doSomething() = 0;
//};
//
//class Statement : public Variable {
//
//};
//
//int isVariableDeclaration(string query) {
//    // do something with query
//    return 0;
//}
//
//int isSelectClause(string query) {
//
//}
//
//int main() {
//    // Here perhaps we assume that each query is going to be multi line?
//    // should that be the case then we need to split by line and then evaluate accordingly.
//    // store the variables that we have created.
//    string query = "statement s1;\nselect s1 such that Follows(5,s1);";
//    string[] splitquery = query.split("\n");
//    std::map<std::string, string> variableStore;
//    QueryParser parser = new QueryParser;
//    QueryValidator validator = new QueryValidator;
//
//    for query in splitquery {
//        output = validator(query)
//        try {
//            if (isVariableDeclaration(query)) {
//                parseVariableDeclaration(query)
//                // evaluate variable declaration
//                // store in variableStore
//            } else if (isSelectClause(query)) {
//                // evaluate select clause
//            }
//        } catch {
//            throw syntax Error
//        }
//    }
//    string variableDeclaration = "variable v;";
//}