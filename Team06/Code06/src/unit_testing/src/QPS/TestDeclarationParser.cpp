#include "QPS/parser/DeclarationParser.h"
#include "catch.hpp"
using namespace std;

TEST_CASE("parseDeclaration / cannot store declaration with same name / throws "
          "Semantic exception") {
    vector<string> declarations = {"stmt s, s"};
    CHECK_THROWS_AS(parseDeclarations(declarations), SemanticException);
}

TEST_CASE("parseDeclaration / cannot store declaration of diff types with same "
          "name / throws Semantic exception") {
    vector<string> declarations = {"stmt s", "assign s"};
    CHECK_THROWS_AS(parseDeclarations(declarations), SemanticException);
}

TEST_CASE("parseDeclaration / invalid spaces in synonym name "
          " / throws Syntax exception") {
    vector<string> declarations = {"stmt s s", "assign s"};
    CHECK_THROWS_AS(parseDeclarations(declarations), SyntaxException);
}