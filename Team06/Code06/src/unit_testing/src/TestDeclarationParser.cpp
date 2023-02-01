//
// Created by Faruq on 30/1/23.
//
#include "catch.hpp"
#include "qps/parser/DeclarationParser.h"
#include <iostream>
using namespace std;

TEST_CASE("parseDeclaration / stores two stmt declaration into store / store contains 2 declarations of same type") {
    string s = "stmt s1, s2; call cl; assign a;";
    VariableStore store = parseDeclarations(s);
    cout<< "store:\n" << store.toString();
}