//
// Created by Faruq on 30/1/23.
//
#include "QPS/parser/DeclarationParser.h"
#include "catch.hpp"
using namespace std;

TEST_CASE("parseDeclaration / stores two stmt declaration into store / store "
          "contains 2 declarations of same type") {
    vector<string> declarations = {"stmt s1, s2", "call cl", "assign a"};
    VariableStore store = parseDeclarations(declarations);
    // change this to an assertion
}

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

TEST_CASE("parseDeclaration / invalid spaces in design entity "
    " / throws Syntax exception") {
    vector<string> declarations = { "st mt s", "assign s" };
    CHECK_THROWS_AS(parseDeclarations(declarations), SyntaxException);
}

TEST_CASE("parseDeclaration / invalid spaces in synonym name "
    " / throws Syntax exception") {
    vector<string> declarations = { "stmt s s", "assign s" };
    CHECK_THROWS_AS(parseDeclarations(declarations), SyntaxException);
}