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

class QueryStub {
public:
    std::string lhs;
    std::string pattern;
};

struct hashFunction
{
    size_t operator()(const std::pair<int, std::unique_ptr<Expression>>& x) const
    {
        //        std::size_t h1 = std::hash<std::string>{}(x.second);
        std::size_t h2 = std::hash<double>{}(x.first);

        return h2;
    }
};

class PatternStorage {
public:

    std::vector<StmtNum> interpretQuery(QueryStub);

    virtual void writePattern(std::string lhs, StmtNum num, std::unique_ptr<Expression> pointer);

    std::unique_ptr<Expression> buildSubtree(std::string rhs);


    // pattern a (v, _)
    virtual std::vector<std::vector<std::string>>  getLHSAndStmtNum();

    // Select v pattern a (v, "v")
    virtual std::vector<std::vector<std::string>> getLHSAndStmtNumRHSNoWildcard(std::string rhs);

    // Select v pattern a (v, _"v"_)
    virtual std::vector<std::vector<std::string>> getLHSAndStmtNumRHSBothWildcard(std::string rhs);

    // utility function for debugging
//    virtual std::vector<std::pair<std::string, std::vector<std::string>>> getAll();

private:
    std::unordered_map<std::string, std::unordered_set<std::pair<int, std::unique_ptr<Expression>>, hashFunction>> lhs_stmtNum_rhsPostfix;

    // assign a; Select a pattern a ("a", "v")
    virtual std::vector<StmtNum> getMatchingExact(std::string lhs, std::string rhs);

    //  assign a; Select a pattern a ("a", _"v"_)
    virtual std::vector<StmtNum> getMatchingRHSBothWildcard(std::string lhs, std::string rhs);

    //  assign a; Select a pattern a ("a", _)
    virtual std::vector<StmtNum> getMatchingLHS(std::string lhs);

    //  assign a; Select a pattern a (_, v)
//    virtual std::vector<std::vector<std::string>>  getRHSAndStmtNum();

    //  assign a; Select a pattern a (_, "v")
    virtual std::vector<StmtNum> getMatchingLHSWildcardRHSNoWildcard(std::string rhs);

    //  assign a; Select a pattern a (_, _"v"_)
    virtual std::vector<StmtNum> getMatchingLHSWildcardRHSBothWildcard(std::string rhs);
};