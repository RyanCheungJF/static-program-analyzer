#include "catch.hpp"
#include "../../../spa/src/SP/AST/Node/Program.h"
#include "../../../spa/src/SP/AST/Node/Procedure.h"
#include "../../../spa/src/SP/AST/Node/StatementList.h"
#include "../../../spa/src/SP/AST/Node/Statement.h"
#include "../../../spa/src/SP/AST/Visitor/FollowsExtractor.h"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/ReadPKB.h"

TEST_CASE("Valid Source Program") {
    std::unique_ptr<Procedure> up1 = std::make_unique<Procedure>(Procedure());
    std::unique_ptr<Procedure> up2 = std::make_unique<Procedure>(Procedure());

    for (int i = 1; i <= 4; i ++) {
        Statement s;
        s.lineNumber = i;
        std::unique_ptr<Statement> u = std::make_unique<Statement>(s);
        up1->statementList->statements.push_back(std::move(u));
    }

    for (int i = 10; i < 12; i ++) {
        Statement s;
        s.lineNumber = i;
        std::unique_ptr<Statement> u = std::make_unique<Statement>(s);
        up2->statementList->statements.push_back(std::move(u));
    }

    Program program;
    program.procedureList.push_back(std::move(up1));
    program.procedureList.push_back(std::move(up2));

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    Follows f;
    pkb.followsApi = &f;
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    FollowsExtractor fe;
    fe.writeApi = &writePkb;
    fe.visitProgram(&program);

    bool res = true;
    res = res && readPkb.checkFollows(1, 2);
    res = res && readPkb.checkFollows(2, 3);
    res = res && readPkb.checkFollows(3, 4);

    res = res && readPkb.checkFollows(10, 11);

    REQUIRE(res);
}