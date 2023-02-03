#include "catch.hpp"
#include "../../../spa/src/SP/AST/Node/Program.h"
#include "../../../spa/src/SP/AST/Node/Procedure.h"
#include "../../../spa/src/SP/AST/Node/StatementList.h"
#include "../../../spa/src/SP/AST/Node/Statement.h"
#include "../../../spa/src/SP/AST/Visitor/FollowsExtractor.h"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"

TEST_CASE("Valid Source Program") {
    Procedure proc1;
    Procedure proc2;

    for (int i = 1; i < 6; i ++) {
        Statement s;
        s.lineNumber = i;
        unique_ptr<Statement> u = std::make_unique<Statement>(s);
        proc1.statementList->statements.push_back(std::move(u));
    }

    for (int i = 6; i <= 10; i + 2) {
        Statement s;
        s.lineNumber = i;
        unique_ptr<Statement> u = std::make_unique<Statement>(s);
        proc1.statementList->statements.push_back(std::move(u));
    }

    Program p;


    REQUIRE(true);
}