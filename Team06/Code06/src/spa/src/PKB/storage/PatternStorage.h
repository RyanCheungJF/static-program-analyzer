#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

typedef int StmtNum;

class Node {
public:
    Node* left;
    Node* right;
private:
    std::string value;
    StmtNum line;
};

class PatternStorage {
public:
    virtual void writePatternNode(Node assignNode);

    // pattern(_, _)
    virtual std::vector<StmtNum> getMatchingAllAssignStatements();

    // pattern("a", "v")
    virtual std::vector<StmtNum> getMatchingRHSNoWildcard(std::string lhs, std::string rhs);

    // pattern("a", _"v")
    virtual std::vector<Node> getMatchingRHSLeftWildcard(std::string lhs, std::string rhs);

    // pattern("a", "v"_)
    virtual std::vector<Node> getMatchingRHSRightWildcard(std::string lhs, std::string rhs);

    // pattern("a", _"v"_)
    virtual std::vector<Node> getMatchingRHSBothWildcard(std::string lhs);

    // pattern(_, "v")
    virtual std::vector<Node> getMatchingLHSWildcardRHSNoWildcard(std::string rhs);

    // pattern(_, _"v")
    virtual std::vector<Node> getMatchingLHSWildcardRHSLeftWildcard(std::string rhs);

    // pattern(_, "v"_)
    virtual std::vector<Node> getMatchingLHSWildcardRHSRightWildcard(std::string rhs);

    // pattern(_, _"v"_)
    virtual std::vector<Node> getMatchingLHSWildcardRHSBothWildcard(std::string rhs);
private:
//    std::unordered_map<Stmt, std::unordered_set<StmtNum>> stmt_stmtNum;
//    std::unordered_map<StmtNum, std::unordered_set<Stmt>> stmtNum_stmt;
};
