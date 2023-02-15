#include "catch.hpp"
#include "../../../spa/src/PKB/storage/UsesStorage.h"


TEST_CASE("UsesStorage: writeUsesS") {
    UsesStorage us;

    StmtNum num1 = 11;
    std::unordered_set<Ent> input1 = {"a"};
    std::unordered_set<Ent> input1_1 = {"a", "b", "c"};
    us.writeUsesS(num1, input1);
    us.writeUsesS(num1, input1_1);

    StmtNum num2 = 20;
    std::unordered_set<Ent> input2 = {"a"};
    std::unordered_set<Ent> input2_1 = {"a", "x", "d"};
    us.writeUsesS(num2, input2);
    us.writeUsesS(num2, input2_1);

    SECTION("UsesStorage: getEnt(StmtNum num)") {
        std::unordered_set<Ent> res1 = us.getEnt(num1);
        REQUIRE(res1.size() == input1_1.size());

        std::unordered_set<Ent> res2 = us.getEnt(num2);
        REQUIRE(res2.size() == input2_1.size());
    }

    SECTION("UsesStorage: exists(StmtNum num, Ent var)") {
        REQUIRE(us.exists(num1, "a"));
        REQUIRE(!us.exists(num1, "d"));

        REQUIRE(us.exists(num2, "a"));
        REQUIRE(!us.exists(num2, "b"));
    }

    SECTION("UsesStorage: getStmtsFromEnt(Ent var)") {
        std::unordered_set<StmtNum> res1 = us.getStmtsFromEnt("a");
        REQUIRE(res1.size() == 2);

        std::unordered_set<StmtNum> res2 = us.getStmtsFromEnt("d");
        REQUIRE(res2.size() == 1);

        std::unordered_set<StmtNum> res3 = us.getStmtsFromEnt("b");
        REQUIRE(res3.size() == 1);

        std::unordered_set<StmtNum> res4 = us.getStmtsFromEnt("z");
        REQUIRE(res4.size() == 0);
    }

    SECTION("UsesStorage: getAllStmtEntPairs()") {
        std::pair<std::vector<StmtNum>, std::vector<std::string>> res = us.getAllStmtEntPairs();
//        for (auto i : res.first) {
//            std::cout << i << " ";
//        }
//        std::cout << "\n";
//        for (auto i : res.second) {
//            std::cout << i << " ";
//        }
        REQUIRE(res.first.size() == res.second.size());
        REQUIRE(res.first.size() == (input1_1.size() + input2_1.size()));
        REQUIRE(find(res.first.begin(), res.first.end(), num1) != res.first.end());
        REQUIRE(find(res.first.begin(), res.first.end(), num2) != res.first.end());
        REQUIRE(find(res.first.begin(), res.first.end(), 100000) == res.first.end());

        REQUIRE(find(res.second.begin(), res.second.end(), "a") != res.second.end());
        REQUIRE(find(res.second.begin(), res.second.end(), "x") != res.second.end());
        REQUIRE(find(res.second.begin(), res.second.end(), "z") == res.second.end());
    }
}

TEST_CASE("UsesStorage: writeUsesP") {
    UsesStorage us;

    ProcName proc1 = "proc1";
    std::unordered_set<Ent> input1 = {"a"};
    std::unordered_set<Ent> input1_1 = {"a", "b", "c"};
    us.writeUsesP(proc1, input1);
    us.writeUsesP(proc1, input1_1);

    ProcName proc2 = "proc2";
    std::unordered_set<Ent> input2 = {"a"};
    std::unordered_set<Ent> input2_1 = {"a", "x", "d"};
    us.writeUsesP(proc2, input2);
    us.writeUsesP(proc2, input2_1);

    SECTION("UsesStorage: getEnt(ProcName proc)") {
        std::unordered_set<Ent> res1 = us.getEnt(proc1);
        REQUIRE(res1.size() == input1_1.size());

        std::unordered_set<Ent> res2 = us.getEnt(proc2);
        REQUIRE(res2.size() == input2_1.size());
    }

    SECTION("UsesStorage: exists(ProcName proc, Ent var)") {
        REQUIRE(us.exists(proc1, "a"));
        REQUIRE(!us.exists(proc1, "d"));

        REQUIRE(us.exists(proc2, "a"));
        REQUIRE(!us.exists(proc2, "b"));
    }

    SECTION("UsesStorage: getProcsFromEnt(Ent var)") {
        std::unordered_set<ProcName> res1 = us.getProcsFromEnt("a");
        REQUIRE(res1.size() == 2);

        std::unordered_set<ProcName> res2 = us.getProcsFromEnt("d");
        REQUIRE(res2.size() == 1);

        std::unordered_set<ProcName> res3 = us.getProcsFromEnt("b");
        REQUIRE(res3.size() == 1);

        std::unordered_set<StmtNum> res4 = us.getStmtsFromEnt("z");
        REQUIRE(res4.size() == 0);
    }

    SECTION("UsesStorage: getAllProcEntPairs()") {
        std::pair<std::vector<ProcName>, std::vector<std::string>> res = us.getAllProcEntPairs();
//        for (auto i : res.first) {
//            std::cout << i << " ";
//        }
//        std::cout << "\n";
//        for (auto i : res.second) {
//            std::cout << i << " ";
//        }
        REQUIRE(res.first.size() == res.second.size());
        REQUIRE(res.first.size() == (input1_1.size() + input2_1.size()));
        REQUIRE(find(res.first.begin(), res.first.end(), proc1) != res.first.end());
        REQUIRE(find(res.first.begin(), res.first.end(), proc2) != res.first.end());
        REQUIRE(find(res.first.begin(), res.first.end(), "proc3") == res.first.end());

        REQUIRE(find(res.second.begin(), res.second.end(), "a") != res.second.end());
        REQUIRE(find(res.second.begin(), res.second.end(), "x") != res.second.end());
        REQUIRE(find(res.second.begin(), res.second.end(), "z") == res.second.end());
    }
}

