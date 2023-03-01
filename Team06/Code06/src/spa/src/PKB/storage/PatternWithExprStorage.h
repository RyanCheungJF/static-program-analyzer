#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include "../../SP/AST/Node/AssignStatement.h"
#include "../../SP/Tokenizer.h"
#include "../../SP/Parser.h"
#include "../../../src/utils/AppConstants.h"

struct hashFunction
{
    size_t operator()(const std::pair<int, std::unique_ptr<Expression>>& x) const
    {
        //        std::size_t h1 = std::hash<std::string>{}(x.second);
        std::size_t h2 = std::hash<double>{}(x.first);

        return h2;
    }
};

class PatternWithExprStorage {
public:

    void writePattern(std::string lhs, StmtNum num, std::unique_ptr<Expression> pointer);

    std::unordered_set<std::pair<int, std::unique_ptr<Expression>>, hashFunction>* getPatternWithLHS(std::string lhs);

    std::unordered_map<std::string, std::unordered_set<std::pair<int, std::unique_ptr<Expression>>, hashFunction>>* getAll();

    // utility function for debugging
//    virtual std::vector<std::pair<std::string, std::vector<std::string>>> getAll();

private:
    std::unordered_map<std::string, std::unordered_set<std::pair<int, std::unique_ptr<Expression>>, hashFunction>> lhs_stmtNum_rhsPostfix;
};