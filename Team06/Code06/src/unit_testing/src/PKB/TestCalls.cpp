#include "../../../spa/src/PKB/ReadPKB.h"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../utils/utils.h"
#include "catch.hpp"

using namespace unit_testing_utils;
using namespace std;

TEST_CASE("CallsStorage: writeP") {
    RelationshipStorage<Ent, Ent> cs;
    ProcName caller = "proc0";

    SECTION("getCallees(procName): empty storage") {
        std::unordered_set<ProcName> res = cs.getRightItems(caller);
        REQUIRE(res.empty());
    }

    ProcName proc1 = "proc1";
    ProcName proc2 = "proc2";

    std::unordered_set<ProcName> callees = {proc1, proc2};
    cs.write(caller, callees);

    SECTION("getCallees(procName): non-empty storage") {
        std::unordered_set<ProcName> res1 = cs.getRightItems("nonExistent");
        REQUIRE(res1.empty());

        std::unordered_set<ProcName> res2 = cs.getRightItems(caller);
        REQUIRE(unit_testing_utils::equals({"proc1", "proc2"}, res2));

        std::unordered_set<ProcName> res3 = cs.getLeftItems(caller);
        REQUIRE(unit_testing_utils::equals({}, res3));

        std::unordered_set<ProcName> res4 = cs.getLeftItems(proc1);
        REQUIRE(unit_testing_utils::equals({caller}, res4));
    }
}

TEST_CASE("findRelationship(shared_ptr<Relationship> rs), Calls and CallsT") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setCalls("proc1", {"proc2"});
    writePkb.setCalls("proc2", {"proc3"});

    writePkb.setCallsT("proc1", {"proc2", "proc3"});
    writePkb.setCallsT("proc2", {"proc3"});

    SECTION("Calls(fixed_string, wildcard)") {
        std::vector<Parameter> params1 = {Parameter("proc1", ParameterType::FIXED_STRING),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::CALLS, params1);

        auto res = readPkb.findRelationship(rs1);
        vector<vector<string>> expected = {{"proc1", "proc2"}};
        REQUIRE(res == expected);
    }

    SECTION("Calls(fixed_string, fixed_string)") {
        std::vector<Parameter> params1 = {Parameter("proc2", ParameterType::FIXED_STRING),
                                          Parameter("proc3", ParameterType::FIXED_STRING)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::CALLS, params1);

        auto res = readPkb.findRelationship(rs1);
        vector<vector<string>> expected = {{"proc2", "proc3"}};
        REQUIRE(equals(res, expected));
    }

    SECTION("Calls(proc synonym, fixed_string)") {
        std::vector<Parameter> params1 = {Parameter("p", ParameterType::PROCEDURE),
                                          Parameter("proc3", ParameterType::FIXED_STRING)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::CALLS, params1);

        auto res = readPkb.findRelationship(rs1);
        vector<vector<string>> expected = {{"proc2", "proc3"}};
        REQUIRE(equals(res, expected));
    }

    SECTION("Calls(proc synonym, wildcard)") {
        std::vector<Parameter> params1 = {Parameter("p", ParameterType::PROCEDURE),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::CALLS, params1);

        auto res = readPkb.findRelationship(rs1);
        vector<vector<string>> expected = {{"proc1", "proc2"}, {"proc2", "proc3"}};
        REQUIRE(equals(res, expected));
    }

    SECTION("Calls*(fixed_string, wildcard)") {
        std::vector<Parameter> params1 = {Parameter("proc1", ParameterType::FIXED_STRING),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::CALLST, params1);

        auto res = readPkb.findRelationship(rs1);
        vector<vector<string>> expected = {{"proc1", "proc2"}, {"proc1", "proc3"}};
        REQUIRE(equals(res, expected));
    }

    SECTION("Calls*(fixed_string, proc synonym)") {
        std::vector<Parameter> params1 = {Parameter("proc1", ParameterType::FIXED_STRING),
                                          Parameter("p", ParameterType::PROCEDURE)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::CALLST, params1);

        auto res = readPkb.findRelationship(rs1);
        vector<vector<string>> expected = {{"proc1", "proc2"}, {"proc1", "proc3"}};
        REQUIRE(equals(res, expected));
    }

    SECTION("Calls*(proc synonym, proc synonym)") {
        std::vector<Parameter> params1 = {Parameter("p1", ParameterType::PROCEDURE),
                                          Parameter("p2", ParameterType::PROCEDURE)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::CALLST, params1);

        auto res = readPkb.findRelationship(rs1);
        vector<vector<string>> expected = {{"proc1", "proc2"}, {"proc1", "proc3"}, {"proc2", "proc3"}};
        REQUIRE(equals(res, expected));
    }

    SECTION("Calls*(wildcard, proc synonym)") {
        std::vector<Parameter> params1 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("p2", ParameterType::PROCEDURE)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::CALLST, params1);

        auto res = readPkb.findRelationship(rs1);
        vector<vector<string>> expected = {{"proc1", "proc2"}, {"proc1", "proc3"}, {"proc2", "proc3"}};
        REQUIRE(equals(res, expected));
    }

    SECTION("Calls*(wildcard, wildcard)") {
        std::vector<Parameter> params1 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::CALLST, params1);

        auto res = readPkb.findRelationship(rs1);
        vector<vector<string>> expected = {{"proc1", "proc2"}, {"proc1", "proc3"}, {"proc2", "proc3"}};
        REQUIRE(equals(res, expected));
    }

    //    SECTION("Calls(p, p)") {
    //        std::vector<Parameter> params1 = {Parameter("p", AppConstants::PROCEDURE),
    //                                          Parameter("p", AppConstants::PROCEDURE)};
    //        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::CALLS, params1);
    //
    //        auto res = readPkb.findRelationship(rs1);
    //        vector<vector<string>> expected = {};
    //        REQUIRE(equals(res, expected));
    //    }
    //
    //    SECTION("Calls*(p, p)") {
    //        std::vector<Parameter> params1 = {Parameter("p", AppConstants::PROCEDURE),
    //                                          Parameter("p", AppConstants::PROCEDURE)};
    //        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::CALLST, params1);
    //
    //        auto res = readPkb.findRelationship(rs1);
    //        vector<vector<string>> expected = {};
    //        REQUIRE(equals(res, expected));
    //    }
}
