//#include "catch.hpp"
//#include "../../../spa/src/PKB/WritePKB.h"
//#include "../../../spa/src/PKB/ReadPKB.h"
//
//using namespace std;
//
//TEST_CASE("All nodes can be added") {
//    WritePKB writePkb;
//    ReadPKB readPkb;
//    PKB pkb;
//    PatternStorage pa;
//    pkb.patternStorage = &pa;
//    writePkb.setInstancePKB(pkb);
//    readPkb.setInstancePKB(pkb);
//
//    // c = 5 + 2 * c
//    /* Looks something like this:
//     *      assign
//     *    c        +
//     *          5     *
//     *              2   c
//     */
//    Node root = Node("assign", 1);
//    Node lhs = Node("c", 1);
//    Node n1 = Node("5", 1);
//    Node n2 = Node("+", 1);
//    Node n3 = Node("2", 1);
//    Node n4 = Node("*", 1);
//    Node n5 = Node("c", 1);
//    root.left = &lhs;
//    root.right = &n2;
//    n2.left = &n1;
//    n2.right = &n4;
//    n4.left = &n3;
//    n4.right = &n5;
//    pa.writePatternNode(root);
//
//    // a = b / c
//    /* Looks something like this:
//     *      assign
//     *    a        /
//     *          b     c
//     */
//    Node root2 = Node("assign", 5);
//    Node n6 = Node("a", 5);
//    Node n7 = Node("b", 5);
//    Node n8 = Node("/", 5);
//    Node n9 = Node("c", 5);
//    root2.left = &n6;
//    root2.right = &n8;
//    n8.left = &n7;
//    n8.right = &n9;
//    pa.writePatternNode(root2);
//
//    // a = d * 5
//    /* Looks something like this:
//     *      assign
//     *    a        *
//     *          d     5
//     */
//    Node root3 = Node("assign", 8);
//    Node n10 = Node("a", 8);
//    Node n11 = Node("d", 8);
//    Node n12 = Node("*", 8);
//    Node n13 = Node("5", 8);
//    root3.left = &n10;
//    root3.right = &n12;
//    n12.left = &n11;
//    n12.right = &n13;
//    pa.writePatternNode(root3);
//
//    bool res = true;
//    std::vector<StmtNum> allAssignStatements = pa.getAllAssignStatements();
//    res = res && allAssignStatements.size() == 3;
//
//    /*
//    std::sort(allAssignStatements.begin(), allAssignStatements.end());
//    // prints out allAssignStatements
//    std::copy(allAssignStatements.begin(),
//              allAssignStatements.end(),
//              std::ostream_iterator<int>(std::cout, " "));
//    std::cout << "\n";
//     */
//
//    REQUIRE(res);
//}
//
//TEST_CASE("Nodes added with LHS are retrieved correctly") {
//    WritePKB writePkb;
//    ReadPKB readPkb;
//    PKB pkb;
//    PatternStorage pa;
//    pkb.patternStorage = &pa;
//    writePkb.setInstancePKB(pkb);
//    readPkb.setInstancePKB(pkb);
//
//    // c = 5 + 2 * c
//    /* Looks something like this:
//     *      assign
//     *    c        +
//     *          5     *
//     *              2   c
//     */
//    Node root = Node("assign", 1);
//    Node lhs = Node("c", 1);
//    Node n1 = Node("5", 1);
//    Node n2 = Node("+", 1);
//    Node n3 = Node("2", 1);
//    Node n4 = Node("*", 1);
//    Node n5 = Node("c", 1);
//    root.left = &lhs;
//    root.right = &n2;
//    n2.left = &n1;
//    n2.right = &n4;
//    n4.left = &n3;
//    n4.right = &n5;
//    pa.writePatternNode(root);
//
//    // a = b / c
//    /* Looks something like this:
//     *      assign
//     *    a        /
//     *          b     c
//     */
//    Node root2 = Node("assign", 5);
//    Node n6 = Node("a", 5);
//    Node n7 = Node("b", 5);
//    Node n8 = Node("/", 5);
//    Node n9 = Node("c", 5);
//    root2.left = &n6;
//    root2.right = &n8;
//    n8.left = &n7;
//    n8.right = &n9;
//    pa.writePatternNode(root2);
//
//    // a = d * 5
//    /* Looks something like this:
//     *      assign
//     *    a        *
//     *          d     5
//     */
//    Node root3 = Node("assign", 8);
//    Node n10 = Node("a", 8);
//    Node n11 = Node("d", 8);
//    Node n12 = Node("*", 8);
//    Node n13 = Node("5", 8);
//    root3.left = &n10;
//    root3.right = &n12;
//    n12.left = &n11;
//    n12.right = &n13;
//    pa.writePatternNode(root3);
//
//    bool res = true;
//    std::vector<StmtNum> aAssignStatements = pa.getMatchingLHS("a");
//    std::vector<StmtNum> cAssignStatements = pa.getMatchingLHS("c");
//    res = res && aAssignStatements.size() == 2;
//    res = res && cAssignStatements.size() == 1;
//
//    /*
//    // prints out aAssignStatements
//    std::copy(aAssignStatements.begin(),
//              aAssignStatements.end(),
//              std::ostream_iterator<int>(std::cout, " "));
//    std::cout << "\n";
//    // prints out cAssignStatements
//    std::copy(cAssignStatements.begin(),
//              cAssignStatements.end(),
//              std::ostream_iterator<int>(std::cout, " "));
//    std::cout << "\n";
//    */
//
//    REQUIRE(res);
//}
//
