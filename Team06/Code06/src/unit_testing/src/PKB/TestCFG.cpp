#include "catch.hpp"
#include "../../../spa/src/PKB/storage/CFGStorage.h"

TEST_CASE("CFGStorage") {
    CFGStorage store;

    SECTION("writeCFG(ProcName procedure, CFGNodeStub &head): procedure does not exist") {
        StmtNum num = 1;
        CFGNodeStub node;
        store.writeCFG(num, node);

        auto root = store.getNode(2);
        REQUIRE(root == nullptr);
    }

    SECTION("writeCFG(ProcName procedure, CFGNodeStub &head): empty procedure") {
        StmtNum num = 1;
        CFGNodeStub node;
        store.writeCFG(num, node);

        auto root = store.getNode(num);
        auto range = root->range;
        auto children = root->children;

        REQUIRE(range.empty());
        REQUIRE(children.empty());
    }

    StmtNum num = 1;
    CFGNodeStub root;
    root.range = {1, 2, 3};
    CFGNodeStub node1;
    node1.range = {4};
    CFGNodeStub node2;
    node2.range = {5, 6};
    CFGNodeStub node3;
    node3.range = {7, 8, 9};
    CFGNodeStub node4;
    node4.range = {10, 11};

    node3.children = {&node4};
    node2.children = {&node4};
    node1.children = {&node3, &node2};
    root.children = {&node1};

    root.parents = {};
    node1.parents = {&root};
    node2.parents = {&node1};
    node3.parents = {&node1};
    node4.parents = {&node2, &node3};

    store.writeCFG(num, root);

    //TODO: onwards
    SECTION("writeCFG(ProcName procedure, CFGNodeStub &head): Program, no while loop") {
        auto root = store.getNode(num);
        auto range = root->range;
        auto children = root->children;
    }

}
