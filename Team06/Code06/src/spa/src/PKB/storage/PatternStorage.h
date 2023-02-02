#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

typedef int StmtNum;

class Node {
public:
    Node* left;
    Node* right;
    std::string value;
    StmtNum line;
};

class PatternStorage {
public:
    //TODO: change this to pointer and table to pointer later. figure out logic first
    virtual void writePatternNode(Node assignNode);

    // pattern(_, _)
    virtual std::vector<StmtNum> getMatchingAllAssignStatements();

    // pattern("a", "v")
    virtual std::vector<StmtNum> getMatchingRHSNoWildcard(std::string lhs, std::string rhs);

    // pattern("a", _"v")
    virtual std::vector<StmtNum> getMatchingRHSLeftWildcard(std::string lhs, std::string rhs);

    // pattern("a", "v"_)
    virtual std::vector<StmtNum> getMatchingRHSRightWildcard(std::string lhs, std::string rhs);

    // pattern("a", _"v"_)
    virtual std::vector<StmtNum> getMatchingRHSBothWildcard(std::string lhs);

    // pattern(_, "v")
    virtual std::vector<StmtNum> getMatchingLHSWildcardRHSNoWildcard(std::string rhs);

    // pattern(_, _"v")
    virtual std::vector<StmtNum> getMatchingLHSWildcardRHSLeftWildcard(std::string rhs);

    // pattern(_, "v"_)
    virtual std::vector<StmtNum> getMatchingLHSWildcardRHSRightWildcard(std::string rhs);

    // pattern(_, _"v"_)
    virtual std::vector<StmtNum> getMatchingLHSWildcardRHSBothWildcard(std::string rhs);
private:
    std::unordered_map<std::string, std::unordered_set<Node*>> rhsVariable_Nodes;
};
