#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include "../../SP/AST/Node/AssignStatement.h"
#include "../../SP/Tokenizer.h"
#include "../../SP/Parser.h"
#include <stdio.h>
#include <iostream>

typedef int StmtNum;

// this is a stub
class Node {
public:
    Node* left;
    Node* right;
    std::string value;
    StmtNum line;

    Node(std::string str, StmtNum line) {
        this->value = str;
        this->line = line;
    }
};

class PatternStorage {
public:
    //TODO: change this to pointer and table to pointer later. figure out logic first
    virtual void writePatternNode(Node &assignNode);
    virtual void writePatternNode(std::string lhs, std::string rhspostfix);
    // A = b + c
    //pattern("A", _"b + ")

    // TODO: get the subtree builder algorithm (KIV: enhancement is to change it to string after demo1)
    std::string buildSubtree(std::string rhs);

    // pattern(_, _)
    virtual std::vector<StmtNum> getAllAssignStatements();

    // pattern("a", "v")
    virtual std::vector<StmtNum> getMatchingExact(std::string lhs, std::string rhs);

    // pattern("a", _"v")
    virtual std::vector<StmtNum> getMatchingRHSLeftWildcard(std::string lhs, std::string rhs);

    // pattern("a", "v"_)
    virtual std::vector<StmtNum> getMatchingRHSRightWildcard(std::string lhs, std::string rhs);

    // pattern("a", _"v"_)
    virtual std::vector<StmtNum> getMatchingRHSBothWildcard(std::string lhs, std::string rhs);

    // pattern("a", _)
    virtual std::vector<StmtNum> getMatchingLHS(std::string lhs);

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
    std::unordered_map<std::string, std::unordered_set<Node*>> lhsVariableAppearence_StmtNum;

    //TODO: what does pattern side take in from query? because if they give us pattern(_, _"x / y * a"_), how do I know which root to search?
    // i need to know how the order of operations occur right? Take the parser from SP side?
    /* this looks like the below btw:
     *          *
     *       /     a
     *    x   y
     */
};
