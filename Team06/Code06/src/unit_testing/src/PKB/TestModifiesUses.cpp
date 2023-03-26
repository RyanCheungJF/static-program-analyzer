#include "../../../spa/src/PKB/storage/ModifiesUsesStorage.h"
#include "catch.hpp"

TEST_CASE("ModifiesUsesStorage: writeS") {
    ModifiesUsesStorage us;

    StmtNum num1 = 11;
    std::unordered_set<Ent> input1 = {"a"};
    std::unordered_set<Ent> input1_1 = {"a", "b", "c"};

    StmtNum num2 = 20;
    std::unordered_set<Ent> input2 = {"a"};
    std::unordered_set<Ent> input2_1 = {"a", "x", "d"};

    SECTION("ModifiesUsesStorage: exists(StmtNum num, Ent var) on empty tables") {
        REQUIRE(!us.exists(num1, "a"));
        REQUIRE(!us.exists(num1, "d"));

        REQUIRE(!us.exists(num2, "a"));
        REQUIRE(!us.exists(num2, "b"));
    }

    us.write(num1, input1);
    us.write(num1, input1_1);
    us.write(num2, input2);
    us.write(num2, input2_1);

    SECTION("ModifiesUsesStorage: getRightItems(StmtNum num)") {
        std::unordered_set<Ent> res1 = us.getRightItems(num1);
        REQUIRE(res1.size() == input1_1.size());

        std::unordered_set<Ent> res2 = us.getRightItems(num2);
        REQUIRE(res2.size() == input2_1.size());

        std::unordered_set<Ent> res3 = us.getRightItems(100000);
        REQUIRE(res3.empty());
    }

    SECTION("ModifiesUsesStorage: exists(StmtNum num, Ent var)") {
        REQUIRE(us.exists(num1, "a"));
        REQUIRE(!us.exists(num1, "d"));

        REQUIRE(us.exists(num2, "a"));
        REQUIRE(!us.exists(num2, "b"));
    }

    SECTION("ModifiesUsesStorage: getStmtNums(Ent var)") {
        std::unordered_set<StmtNum> res1 = us.getStmtNums("a");
        REQUIRE(res1.size() == 2);

        std::unordered_set<StmtNum> res2 = us.getStmtNums("d");
        REQUIRE(res2.size() == 1);

        std::unordered_set<StmtNum> res3 = us.getStmtNums("b");
        REQUIRE(res3.size() == 1);

        std::unordered_set<StmtNum> res4 = us.getStmtNums("z");
        REQUIRE(res4.empty());
    }

    SECTION("ModifiesUsesStorage: getAllStmtNums()") {
        std::unordered_set<StmtNum> res = us.getAllStmtNums();
        std::unordered_set<StmtNum> expected = {11, 20};
    }
}

TEST_CASE("ModifiesUsesStorage: writeP") {
    ModifiesUsesStorage us;

    ProcName proc1 = "proc1";
    std::unordered_set<Ent> input1 = {"a"};
    std::unordered_set<Ent> input1_1 = {"a", "b", "c"};

    ProcName proc2 = "proc2";
    std::unordered_set<Ent> input2 = {"a"};
    std::unordered_set<Ent> input2_1 = {"a", "x", "d"};

    SECTION("ModifiesUsesStorage: exists(ProcName proc, Ent var) on empty tables") {
        REQUIRE(!us.exists(proc1, "a"));
        REQUIRE(!us.exists(proc1, "d"));

        REQUIRE(!us.exists(proc2, "a"));
        REQUIRE(!us.exists(proc2, "b"));
    }

    us.write(proc1, input1);
    us.write(proc1, input1_1);
    us.write(proc2, input2);
    us.write(proc2, input2_1);

    SECTION("ModifiesUsesStorage: getRightItems(ProcName proc)") {
        std::unordered_set<Ent> res1 = us.getRightItems(proc1);
        REQUIRE(res1.size() == input1_1.size());

        std::unordered_set<Ent> res2 = us.getRightItems(proc2);
        REQUIRE(res2.size() == input2_1.size());

        std::unordered_set<Ent> res3 = us.getRightItems("proc3");
        REQUIRE(res3.size() == 0);
    }

    SECTION("ModifiesUsesStorage: exists(ProcName proc, Ent var)") {
        REQUIRE(us.exists(proc1, "a"));
        REQUIRE(!us.exists(proc1, "d"));

        REQUIRE(us.exists(proc2, "a"));
        REQUIRE(!us.exists(proc2, "b"));
    }

    SECTION("ModifiesUsesStorage: getProcsFromEnt(Ent var)") {
        std::unordered_set<ProcName> res1 = us.getProcs("a");
        REQUIRE(res1.size() == 2);

        std::unordered_set<ProcName> res2 = us.getProcs("d");
        REQUIRE(res2.size() == 1);

        std::unordered_set<ProcName> res3 = us.getProcs("b");
        REQUIRE(res3.size() == 1);

        std::unordered_set<ProcName> res4 = us.getProcs("z");
        REQUIRE(res4.size() == 0);
    }

    SECTION("ModifiesUsesStorage: getAllProcs()") {
        std::unordered_set<Ent> res = us.getAllProcs();
        std::unordered_set<Ent> expected = {"proc1", "proc2"};
        REQUIRE(res == expected);
    }
}
