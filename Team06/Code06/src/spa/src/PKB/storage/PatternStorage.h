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

class PatternStorage {
public:
    virtual void writePatternNode(std::string lhs, std::string rhspostfix);

    std::string buildSubtree(std::string rhs);

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

    // utility function for debugging
    virtual std::vector<std::pair<std::string, std::string>> getAllPostfixes();

private:
    std::unordered_map<std::string, std::unordered_set<std::string>> lhs_rhsPostfix;
};
